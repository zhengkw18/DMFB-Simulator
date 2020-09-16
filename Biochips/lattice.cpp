#include "lattice.h"

Lattice::Lattice(QWidget *parent,Setting setting,int pos1,int pos2) : QWidget(parent),setting(setting),w_inpos(pos1),w_outpos(pos2)
{
    sound_move=new QSound(":/sound/move.wav");
}
void Lattice::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    //draw the lattice
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);
    for(int i=0;i<=setting.h;i++){
        painter.drawLine(QPoint(0,60*i),QPoint(60*setting.w,60*i));
    }
    for(int i=0;i<=setting.w;i++){
        painter.drawLine(QPoint(60*i,0),QPoint(60*i,60*setting.h));
    }
    foreach(Pos pos,locked){
        QBrush brush(QColor(qRgb(255,99,71)));
        painter.setBrush(brush);
        painter.drawRect(60*pos.x-60,60*pos.y-60,60,60);
    }
    if(status>0){
        bool occupied[13][13];
        for(int i=0;i<13;i++){
            for(int j=0;j<13;j++){
                occupied[i][j]=false;
            }
        }
        //draw the drops
        foreach (Drop *drop, scenes[time].dropstatus.keys()) {
            QBrush brush(drop->color);
            painter.setBrush(brush);
            int centerx=60*scenes[time].dropstatus.value(drop).pos.x-30;
            int centery=60*scenes[time].dropstatus.value(drop).pos.y-30;
            occupied[scenes[time].dropstatus.value(drop).pos.x][scenes[time].dropstatus.value(drop).pos.y]=true;
            int width=50,height=50,deviax=25,deviay=25;
            if(scenes[time].dropstatus.value(drop).elliptical){
                if(scenes[time].dropstatus.value(drop).vertical){
                    height=160;
                    occupied[scenes[time].dropstatus.value(drop).pos.x][scenes[time].dropstatus.value(drop).pos.y-1]=true;
                    occupied[scenes[time].dropstatus.value(drop).pos.x][scenes[time].dropstatus.value(drop).pos.y+1]=true;
                    deviay=80;
                }else{
                    width=160;
                    occupied[scenes[time].dropstatus.value(drop).pos.x-1][scenes[time].dropstatus.value(drop).pos.y]=true;
                    occupied[scenes[time].dropstatus.value(drop).pos.x+1][scenes[time].dropstatus.value(drop).pos.y]=true;
                    deviax=80;
                }
            }
            painter.drawEllipse(centerx-deviax,centery-deviay,width,height);
        }
        if(time==validtime){
            foreach(Pos pos,contaminations[time].keys()){
                if(!occupied[pos.x][pos.y]){
                    painter.drawText(60*pos.x-30,60*pos.y-30,QString::number(contaminations[time].value(pos).size()));
                }
            }
        }else{
            foreach(Pos pos,contaminations[time].keys()){
                if(!occupied[pos.x][pos.y]){
                    int basex=60*pos.x-60;
                    int basey=60*pos.y-60;
                    foreach(int i,contaminations[time].value(pos)){
                        QBrush brush(drops[i]->color);
                        painter.setBrush(brush);
                        painter.drawEllipse(basex+pollutionpos[i].x-5,basey+pollutionpos[i].y-5,10,10);
                    }
                }
            }
        }
        if(status==2){
            if(!path.isEmpty()){
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                int delta[4][2]={{-30,0},{30,0},{0,-30},{0,30}};
                painter.drawLine(60*setting.w_in.x-30+delta[w_inpos][0],60*setting.w_in.y-30+delta[w_inpos][1],60*setting.w_in.x-30,60*setting.w_in.y-30);
                for(int i=0;i<path.size()-1;i++){
                    painter.drawLine(60*path[i].x-30,60*path[i].y-30,60*path[i+1].x-30,60*path[i+1].y-30);
                }
                painter.drawLine(60*setting.w_out.x-30+delta[w_outpos][0],60*setting.w_out.y-30+delta[w_outpos][1],60*setting.w_out.x-30,60*setting.w_out.y-30);
                Pos now=path[nowpath];
                QBrush brush(Qt::blue);
                painter.setBrush(brush);
                painter.drawRect(60*now.x-60,60*now.y-60,60,60);
            }
        }
    }
}

void Lattice::load(int vtime,QVector<Scene> ss,QMap<int,Drop*> ds){
    validtime=vtime;
    status=1;
    scenes=ss;
    drops=ds;
    foreach(int i,drops.keys()){
        Pos random;
        random.x=rand()%40+10;
        random.y=rand()%40+10;
        pollutionpos.insert(i,random);
    }
    contaminations.clear();
    contaminations.push_back(QMap<Pos,QSet<int>>());
}
void Lattice::setScene(int t){
    time=t;
    status=1;
    repaint();
    foreach (Drop *drop, scenes[time].dropstatus.keys()){
        if(!contaminations[time][scenes[time].dropstatus.value(drop).pos].empty()){
            if(contaminations[time][scenes[time].dropstatus.value(drop).pos].size()>1||!contaminations[time][scenes[time].dropstatus.value(drop).pos].contains(drop->id)){
                QMessageBox::critical(this, "Contamination", "Drop "+QString::number(drop->id)+" inevitably contaminated.");
            }
        }
    }
}
void Lattice::getContaminations(int t){
    QMap<Pos,QSet<int>> contamination=contaminations[t-1];
    foreach(Pos p,scenes[t].newcontaminations.keys()){
        if(contamination.contains(p)){
            foreach(int i,scenes[t].newcontaminations.value(p)){
                contamination[p].insert(i);
            }
        }else{
            contamination.insert(p,scenes[t].newcontaminations.value(p));
        }
    }
    if(contaminations.size()==t){
        contaminations.push_back(contamination);
    }else if(contaminations.size()>t){
        contaminations[t]=contamination;
    }
}
void Lattice::wash(){
    status=2;
    //caculate the path
    path.clear();
    int points[setting.w+2][setting.h+2];
    //0:valid 1:barrier 2:target
    for(int i=0;i<=setting.w+1;i++){
        for(int j=0;j<=setting.h+1;j++){
            points[i][j]=0;
        }
    }
    for(int i=0;i<=setting.w+1;i++){
        points[i][0]=1;
        points[i][setting.h+1]=1;
    }
    for(int j=0;j<=setting.h+1;j++){
        points[0][j]=1;
        points[setting.w+1][j]=1;
    }
    foreach(Drop *drop,scenes[time].dropstatus.keys()){
        int x=scenes[time].dropstatus.value(drop).pos.x;
        int y=scenes[time].dropstatus.value(drop).pos.y;
        int rangex=1,rangey=1;
        if(scenes[time].dropstatus.value(drop).elliptical){
            if(scenes[time].dropstatus.value(drop).vertical){
                rangey=2;
            }else{
                rangex=2;
            }
        }
        for(int i=x-rangex;i<=x+rangex;i++){
            for(int j=y-rangey;j<=y+rangey;j++){
                points[i][j]=1;
            }
        }
    }
    foreach(Pos pos,locked){
        points[pos.x][pos.y]=1;
    }
    foreach(Pos pos,contaminations[time].keys()){
        if(!contaminations[time].value(pos).empty()&&points[pos.x][pos.y]==0)points[pos.x][pos.y]=2;
    }
    int stepnum[setting.w+1][setting.h+1];
    for(int i=0;i<=setting.w;i++){
        for(int j=0;j<=setting.h;j++){
            stepnum[i][j]=10000;
        }
    }
    if(points[setting.w_in.x][setting.w_in.y]==1)return;
    QQueue<Pos> queue;
    queue.push_back(setting.w_in);
    stepnum[setting.w_in.x][setting.w_in.y]=0;
    int deltax[4]={1,-1,0,0},deltay[4]={0,0,1,-1};
    int maxstep=0;
    while(!queue.isEmpty()){
        Pos p=queue.head();
        queue.pop_front();
        for(int i=0;i<4;i++){
            int newx=p.x+deltax[i];
            int newy=p.y+deltay[i];
            if(points[newx][newy]!=1&&stepnum[newx][newy]==10000){
                stepnum[newx][newy]=stepnum[p.x][p.y]+1;
                maxstep=utils::max(maxstep,stepnum[newx][newy]);
                queue.push_back({newx,newy});
            }
        }
    }
    if(stepnum[setting.w_out.x][setting.w_out.y]==10000)return;
    QVector<Pos> nodes;//intermediate nodes
    for(int k=0;k<=maxstep;k++){
        for(int i=1;i<=setting.w;i++){
            for(int j=1;j<=setting.h;j++){
                Pos pppp={i,j};
                if(stepnum[i][j]==k&&points[i][j]==2&&!(pppp==setting.w_in)&&!(pppp==setting.w_out)){
                    nodes.push_back({i,j});
                }
            }
        }
    }
    nodes.push_front(setting.w_in);
    nodes.push_back(setting.w_out);
    if(nodes.size()<=2)return;
    int distances[nodes.size()][nodes.size()];
    for(int m=0;m<nodes.size();m++){
        for(int n=0;n<nodes.size();n++){
            if(m==n){
                distances[m][n]=0;
                continue;
            }
            int walked[setting.w+1][setting.h+1];
            for(int k=0;k<=setting.w;k++){
                for(int j=0;j<=setting.h;j++){
                    walked[k][j]=10000;
                }
            }
            QQueue<Pos> queue2;
            queue2.push_back(nodes[m]);
            walked[nodes[m].x][nodes[m].y]=0;
            bool flag=true;
            int deltax[4]={1,-1,0,0},deltay[4]={0,0,1,-1};
            while(flag){
                Pos p=queue2.head();
                queue2.pop_front();
                for(int k=0;k<4;k++){
                    int newx=p.x+deltax[k];
                    int newy=p.y+deltay[k];
                    if(points[newx][newy]!=1&&walked[newx][newy]==10000){
                        walked[newx][newy]=walked[p.x][p.y]+1;
                        if(nodes[n].x==newx&&nodes[n].y==newy){
                            flag=false;
                            break;
                        }
                        queue2.push_back({newx,newy});
                    }
                }
            }
            distances[m][n]=walked[nodes[n].x][nodes[n].y];
        }
    }
    QList<int> nodeslist;
    nodeslist<<0<<nodes.size()-1;
    for(int i=1;i<nodes.size()-1;i++){
        int insertpos=1;
        int minsum=10000;
        for(int j=1;j<nodeslist.size();j++){
            if(distances[i][nodeslist.at(j-1)]+distances[i][nodeslist.at(j)]-distances[nodeslist.at(j)][nodeslist.at(j-1)]<minsum){
                minsum=distances[i][nodeslist.at(j-1)]+distances[i][nodeslist.at(j)]-distances[nodeslist.at(j)][nodeslist.at(j-1)];
                insertpos=j;
            }
        }
        nodeslist.insert(insertpos,i);
    }
    QVector<Pos> newnodes;
    for(int i=0;i<nodeslist.size();i++){
        newnodes.push_back(nodes[nodeslist.at(i)]);
    }
    for(int i=0;i<newnodes.size()-1;i++){ //i to i+1
        path.push_back(newnodes[i]);
        bool walked[setting.w+1][setting.h+1];
        Pos last[setting.w+1][setting.h+1];
        for(int k=0;k<=setting.w;k++){
            for(int j=0;j<=setting.h;j++){
                walked[k][j]=false;
            }
        }
        QQueue<Pos> queue2;
        queue2.push_back(newnodes[i]);
        walked[newnodes[i].x][newnodes[i].y]=true;
        bool flag=true;
        int deltax[4]={1,-1,0,0},deltay[4]={0,0,1,-1};
        while(flag){
            Pos p=queue2.head();
            queue2.pop_front();
            for(int k=0;k<4;k++){
                int newx=p.x+deltax[k];
                int newy=p.y+deltay[k];
                if(points[newx][newy]!=1&&!walked[newx][newy]){
                    walked[newx][newy]=true;
                    last[newx][newy]=p;
                    if(newnodes[i+1].x==newx&&newnodes[i+1].y==newy){
                        flag=false;
                        break;
                    }
                    queue2.push_back({newx,newy});
                }
            }
        }
        QVector<Pos> subpath;
        Pos ppp=newnodes[i+1];
        while(!(last[ppp.x][ppp.y]==newnodes[i])){
            ppp=last[ppp.x][ppp.y];
            subpath.push_back(ppp);
        }
        for(int k=subpath.size()-1;k>=0;k--){
            path.push_back(subpath[k]);
        }
    }
    path.push_back(newnodes.last());
    for(int i=0;i<path.size();i++){
        nowpath=i;
        if(points[path[i].x][path[i].y]==2)contaminations[time][path[i]].clear();
        repaint();
        sound_move->play();
        QTime t;
        t.start();
        while(t.elapsed()<50)
            QCoreApplication::processEvents();
    }
}
void Lattice::mousePressEvent(QMouseEvent *event){
    if(status==1&&setting.washable){
        if(event->button()==Qt::RightButton){
            Pos pp;
            pp.x=event->x()/60+1;
            pp.y=event->y()/60+1;
            if(locked.contains(pp))locked.erase(locked.find(pp));
            else locked.insert(pp);
        }
    }
    repaint();
}
void Lattice::refresh(){
    status=1;
    repaint();
}
