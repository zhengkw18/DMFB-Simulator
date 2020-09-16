#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    action_startup=new QAction(this);
    action_import=new QAction(this);
    action_about=new QAction(this);
    action_reset=new QAction(this);
    action_play=new QAction(this);
    action_stop=new QAction(this);
    action_last=new QAction(this);
    action_next=new QAction(this);
    action_startup->setText("Initiate");
    action_startup->setIcon(QIcon(":/image/settings.png"));
    action_startup->setToolTip("Initiate the settings");
    action_import->setText("Import file");
    action_import->setIcon(QIcon(":/image/file.png"));
    action_import->setToolTip("Import file as simulation orders");
    action_about->setText("About");
    action_about->setToolTip("Original info and intention about this program");
    action_reset->setText("Reset");
    action_reset->setIcon(QIcon(":/image/reset.png"));
    action_reset->setToolTip("Return to the initial status");
    action_play->setText("Play");
    action_play->setIcon(QIcon(":/image/play.png"));
    action_play->setToolTip("Display the simulation process step by step");
    action_stop->setText("Stop");
    action_stop->setIcon(QIcon(":/image/cancal.png"));
    action_stop->setToolTip("Terminate the simulation process");
    action_last->setText("Last step");
    action_last->setIcon(QIcon(":/image/left.png"));
    action_last->setToolTip("Return to the last step");
    action_next->setText("Next step");
    action_next->setIcon(QIcon(":/image/right.png"));
    action_next->setToolTip("Proceed to the next step");

    ui->menu->addAction(action_startup);
    ui->menu->addAction(action_import);
    ui->mainToolBar->addAction(action_startup);
    ui->mainToolBar->addAction(action_import);
    ui->menu_2->addAction(action_about);
    ui->menuAction->addAction(action_reset);
    ui->menuAction->addAction(action_play);
    ui->menuAction->addAction(action_stop);
    ui->menuAction->addAction(action_last);
    ui->menuAction->addAction(action_next);
    ui->mainToolBar->addAction(action_reset);
    ui->mainToolBar->addAction(action_play);
    ui->mainToolBar->addAction(action_stop);
    ui->mainToolBar->addAction(action_last);
    ui->mainToolBar->addAction(action_next);
    QObject::connect(action_about, &QAction::triggered,this,[=](){
        QMessageBox::about(this,"About DMBS","A rough QT product for course design.");
    });
    this->setFixedSize(400,150);
    this->ui->label->setGeometry(20,0,222,32);
    this->ui->label_2->setGeometry(260,0,182,32);

    this->sound_move=new QSound(":/sound/move.wav",this);
    this->sound_merge=new QSound(":/sound/merge.wav",this);
    this->sound_split1=new QSound(":/sound/split1.wav",this);
    this->sound_split2=new QSound(":/sound/split2.wav",this);

    QObject::connect(action_startup,SIGNAL(triggered()),this,SLOT(Initiate()));
    QObject::connect(action_import,SIGNAL(triggered()),this,SLOT(Load()));
    QObject::connect(action_next,SIGNAL(triggered()),this,SLOT(nextstep()));
    QObject::connect(action_last,SIGNAL(triggered()),this,SLOT(laststep()));
    QObject::connect(action_reset,SIGNAL(triggered()),this,SLOT(reset()));
    QObject::connect(action_play,SIGNAL(triggered()),this,SLOT(play()));
    QObject::connect(action_stop,SIGNAL(triggered()),this,SLOT(stop()));

    label_output=new QLabel(this);
    label_washinput=new QLabel(this);
    label_waste=new QLabel(this);

    label_output->setText("Output");
    label_washinput->setText("Wash\nInput");
    label_waste->setText("Waste");
    label_output->setFrameShape (QFrame::Box);
    label_output->setStyleSheet("border-width: 1px;border-style: solid;border-color:black;background-color:violet;");
    label_output->setAlignment(Qt::AlignCenter);
    label_washinput->setFrameShape (QFrame::Box);
    label_washinput->setStyleSheet("border-width: 1px;border-style: solid;border-color:black;background-color:lightblue;");
    label_washinput->setAlignment(Qt::AlignCenter);
    label_waste->setFrameShape (QFrame::Box);
    label_waste->setStyleSheet("border-width: 1px;border-style: solid;border-color:black;background-color:sienna;");
    label_waste->setAlignment(Qt::AlignCenter);
    this->setStatus(0);
}
void MainWindow::setStatus(int id){
    if(id==0){
        ui->label->setText("Present time: Not Valid");
        ui->label_2->setText("Status: Vacant");
        this->action_startup->setEnabled(true);
        this->action_import->setEnabled(false);
        this->action_reset->setEnabled(false);
        this->action_stop->setEnabled(false);
        this->action_play->setEnabled(false);
        this->action_next->setEnabled(false);
        this->action_last->setEnabled(false);
        label_output->setVisible(false);
        label_washinput->setVisible(false);
        label_waste->setVisible(false);
    }else if(id==1){
        ui->label->setText("Present time: Not Valid");
        ui->label_2->setText("Status: Setted");
        this->action_startup->setEnabled(true);
        this->action_import->setEnabled(true);
        this->action_reset->setEnabled(false);
        this->action_stop->setEnabled(false);
        this->action_play->setEnabled(false);
        this->action_next->setEnabled(false);
        this->action_last->setEnabled(false);
    }else if(id==2){
        ui->label->setText("Present time: "+QString::number(time)+"/"+QString::number(validtime));
        ui->label_2->setText("Status: Loaded");
        this->action_startup->setEnabled(true);
        this->action_import->setEnabled(true);
        this->action_reset->setEnabled(true);
        this->action_stop->setEnabled(false);
        this->action_play->setEnabled(true);
        this->action_next->setEnabled(true);
        this->action_last->setEnabled(true);
    }else if(id==3){
        ui->label_2->setText("Status: Playing");
        this->action_startup->setEnabled(false);
        this->action_import->setEnabled(false);
        this->action_reset->setEnabled(false);
        this->action_stop->setEnabled(true);
        this->action_play->setEnabled(false);
        this->action_next->setEnabled(false);
        this->action_last->setEnabled(false);
    }else if(id==4){
        ui->label_2->setText("Status: Washing");
        this->action_startup->setEnabled(false);
        this->action_import->setEnabled(false);
        this->action_reset->setEnabled(false);
        this->action_stop->setEnabled(false);
        this->action_play->setEnabled(false);
        this->action_next->setEnabled(false);
        this->action_last->setEnabled(false);
    }
}
void MainWindow::Initiate(){
    InitiateDialog *dlg=new InitiateDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);//设置对话框关闭后，自动销毁
    dlg->setWindowModality(Qt::WindowModal);
    Setting set = dlg->getSetting();
    if(set.w>0&&set.h>0){
        setting=set;
        status=1;
        setStatus(1);
        scenes.clear();
        drops.clear();
        totaltime=0;
        validtime=0;
        for(int i=0;i<200;i++){
            movements[i].clear();
        }
        counter=0;
        time=-1;
        Pos newpos2,newpos3,newpos4;
        QVector<Pos> newpos1;
        for(int i=0;i<label_inputs.size();i++){
            delete label_inputs[i];
        }
        label_inputs.clear();
        for(int i=0;i<setting.in.size();i++){
            newpos1.push_back(getWidgePos(transferPos(setting.in[i])));
            QLabel* qlabel=new QLabel(this);
            qlabel->setText("Input");
            qlabel->setFrameShape (QFrame::Box);
            qlabel->setStyleSheet("border-width: 1px;border-style: solid;border-color:black;background-color:green;");
            qlabel->setAlignment(Qt::AlignCenter);
            label_inputs.push_back(qlabel);
            qlabel->setVisible(false);
            qlabel->setGeometry(newpos1[i].x,newpos1[i].y,60,60);
        }
        newpos2=getWidgePos(transferPos(setting.out));
        newpos3=getWidgePos(transferPos(setting.w_in));
        w_inpos=temppos;
        newpos4=getWidgePos(transferPos(setting.w_out));
        w_outpos=temppos;
        if(lattice!=nullptr)delete lattice;
        lattice=new Lattice(this,set,w_inpos,w_outpos);
        lattice->setVisible(false);
        lattice->setGeometry(80,190,60*setting.w,60*setting.h);
        label_output->setVisible(false);
        label_washinput->setVisible(false);
        label_waste->setVisible(false);

        label_output->setGeometry(newpos2.x,newpos2.y,60,60);
        label_washinput->setGeometry(newpos3.x,newpos3.y,60,60);
        label_waste->setGeometry(newpos4.x,newpos4.y,60,60);
        lattice->show();
        for(int i=0;i<label_inputs.size();i++){
            label_inputs[i]->setVisible(true);
        }
        label_output->setVisible(true);
        if(setting.washable){
            label_washinput->setVisible(true);
            label_waste->setVisible(true);
        }
        this->setFixedSize(utils::max<int>(400,100+60*(setting.w+2)),150+60*(setting.h+2));
    }

}
void MainWindow::Load(){
    QString strFile = QFileDialog::getOpenFileName(this,"Choose a txt file containing orders","C:","text files(*txt)");
    if(strFile.isEmpty()||strFile.isNull())return;
    QFile f(strFile);
    if (!f.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, "Loading Failed", "Could not open file.");
        return;
    }
    status=2;
    scenes.clear();
    drops.clear();
    totaltime=0;
    validtime=0;
    for(int i=0;i<200;i++){
        movements[i].clear();
    }
    counter=0;
    time=-1;
    QTextStream in(&f);

    while(!in.atEnd()){
        transferOrdertoMovements(in.readLine());
    }
    totaltime++;
    if(processSimulation()){
        QMessageBox::information(this, "Simulating Success", "Total time: "+QString::number(validtime));
    }else{
        QMessageBox::critical(this, "Simulating Terminated", "Valid time: "+QString::number(validtime));
    }
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    foreach(Drop *drop,drops){
        drop->color=QColor::fromHsl(qrand()%360,qrand()%256,qrand()%200);
    }
    lattice->load(validtime,scenes,drops);
    time=0;
    jumpto(0);
    setStatus(2);
}
MainWindow::~MainWindow()
{
    delete ui;
}

Pos MainWindow::getWidgePos(Pos pos){
    Pos newpos;
    newpos.x=20+60*pos.x;
    newpos.y=130+60*pos.y;
    return newpos;
}

Pos MainWindow::transferPos(Pos pos){
    Pos newpos;
    if(pos.x==1){
        //L
        newpos.x=pos.x-1;
        newpos.y=pos.y;
        temppos=0;
    }else if(pos.x==setting.w){
        //R
        newpos.x=pos.x+1;
        newpos.y=pos.y;
        temppos=1;
    }else if(pos.y==1){
        //U
        newpos.x=pos.x;
        newpos.y=pos.y-1;
        temppos=2;
    }else{
        //D
        newpos.x=pos.x;
        newpos.y=pos.y+1;
        temppos=3;
    }
    return newpos;
}

void MainWindow::transferOrdertoMovements(QString order){
    order=order.left(order.size()-1);
    QStringList list1=order.split(" ");
    QString keyword=list1.at(0);
    QStringList list2=list1.at(1).split(",");
    int time=list2.at(0).toInt();
    if(keyword=="Input"){
        Movement m;
        m.type=0;
        m.pos={list2.at(1).toInt(),list2.at(2).toInt()};
        movements[time].push_back(m);
        totaltime=utils::max(totaltime,time);
    }else if(keyword=="Output"){
        Movement m;
        m.type=1;
        m.pos={list2.at(1).toInt(),list2.at(2).toInt()};
        movements[time].push_back(m);
        totaltime=utils::max(totaltime,time);
    }else if(keyword=="Move"){
        Movement m;
        m.type=2;
        m.pos={list2.at(1).toInt(),list2.at(2).toInt()};
        m.destination={list2.at(3).toInt(),list2.at(4).toInt()};
        movements[time].push_back(m);
        totaltime=utils::max(totaltime,time);
    }else if(keyword=="Mix"){
        for(int i=1;i<list2.size()/2;i++){
            Movement m;
            m.type=2;
            m.pos={list2.at(2*i-1).toInt(),list2.at(2*i).toInt()};
            m.destination={list2.at(2*i+1).toInt(),list2.at(2*i+2).toInt()};
            movements[time+i-1].push_back(m);
            totaltime=utils::max(totaltime,time+i-1);
        }
    }else if(keyword=="Split"){
        Movement m1,m2;
        m1.type=3;
        m2.type=4;
        Pos pos={list2.at(1).toInt(),list2.at(2).toInt()};
        m1.pos=pos;
        m2.pos=pos;
        m1.upward=(list2.at(1).toInt()==list2.at(3).toInt());
        movements[time].push_back(m1);
        movements[time+1].push_back(m2);
        totaltime=utils::max(totaltime,time+1);
    }else if(keyword=="Merge"){
        Movement m1,m2;
        m1.type=5;
        m2.type=6;
        Pos pos={(list2.at(1).toInt()+list2.at(3).toInt())/2,(list2.at(2).toInt()+list2.at(4).toInt())/2};
        m1.pos=pos;
        m2.pos=pos;
        m1.upward=(list2.at(1).toInt()==list2.at(3).toInt());
        m2.upward=m1.upward;
        movements[time].push_back(m1);
        movements[time+1].push_back(m2);
        totaltime=utils::max(totaltime,time+1);
    }
}

bool MainWindow::processSimulation(){
    //return the validity of the scenes
    Scene empty;
    scenes.push_back(empty);
    for(int i=1;i<=totaltime;i++){
        //generate scene[i] on the fundaion of scene[i-1] and movements[i-1]
        QMap<Pos,Drop*> lastmap;
        foreach(Drop* drop,scenes[i-1].dropstatus.keys()){
            lastmap.insert(scenes[i-1].dropstatus.value(drop).pos,drop);
        }
        Scene newscene=scenes[i-1];
        newscene.newcontaminations.clear();
        //process movements of drops and contaminations
        foreach(Movement move,movements[i-1]){
            if(move.type==0){
                //Input
                bool inputvalid=false;
                foreach(Pos pos,setting.in){
                    if(pos==move.pos){
                        inputvalid=true;
                        break;
                    }
                }
                if(inputvalid){
                    Drop *newdrop=new Drop(this,counter,Qt::black);
                    drops.insert(counter,newdrop);
                    counter++;
                    newscene.dropstatus.insert(newdrop,{move.pos,false,false});
                    newscene.newcontaminations[move.pos].insert(newdrop->id);
                }else{
                    QMessageBox::critical(this, "Simulating Failed", "Input error.");
                    return false;
                }
            }else if(move.type==1){
                //Output
                if(lastmap.contains(move.pos)&&move.pos==setting.out){
                    for(auto it=newscene.dropstatus.begin();it!=newscene.dropstatus.end();it++){
                        if(it.value().pos==move.pos){
                            newscene.dropstatus.erase(it);
                            break;
                        }
                    }
                }else{
                    QMessageBox::critical(this, "Simulating Failed", "Output error.");
                    return false;
                }
            }else if(move.type==2){
                //Move
                if(lastmap.contains(move.pos)&&isadjacent(move.pos,move.destination)){
                    newscene.dropstatus[lastmap.value(move.pos)].pos=move.destination;
                    newscene.newcontaminations[move.destination].insert(lastmap.value(move.pos)->id);
                }else{
                    QMessageBox::critical(this, "Simulating Failed", "Move or mix error.");
                    return false;
                }
            }else if(move.type==3){
                //Split1
                if(lastmap.contains(move.pos)){
                    newscene.dropstatus[lastmap.value(move.pos)].elliptical=true;
                    newscene.dropstatus[lastmap.value(move.pos)].vertical=move.upward;
                }else{
                    QMessageBox::critical(this, "Simulating Failed", "Split error.");
                    return false;
                }
            }else if(move.type==4){
                //Split2
                if(lastmap.contains(move.pos)&&newscene.dropstatus.value(lastmap.value(move.pos)).elliptical){
                    Pos pos1(move.pos),pos2(move.pos);
                    if(newscene.dropstatus.value(lastmap.value(move.pos)).vertical){
                        pos1.y--;
                        pos2.y++;
                    }else{
                        pos1.x--;
                        pos2.x++;
                    }
                    Drop *newdrop1=new Drop(this,counter,Qt::black);
                    drops.insert(counter,newdrop1);
                    counter++;
                    newscene.dropstatus.insert(newdrop1,{pos1,false,false});
                    newscene.newcontaminations[pos1].insert(newdrop1->id);
                    Drop *newdrop2=new Drop(this,counter,Qt::black);
                    drops.insert(counter,newdrop2);
                    counter++;
                    newscene.dropstatus.insert(newdrop2,{pos2,false,false});
                    newscene.newcontaminations[pos2].insert(newdrop2->id);

                    for(auto it=newscene.dropstatus.begin();it!=newscene.dropstatus.end();it++){
                        if(it.value().pos==move.pos){
                            newscene.dropstatus.erase(it);
                            break;
                        }
                    }
                }else{
                    QMessageBox::critical(this, "Simulating Failed", "Split error.");
                    return false;
                }
            }else if(move.type==5){
                //Merge1
                Pos pos1(move.pos),pos2(move.pos);
                if(move.upward){
                    pos1.y--;
                    pos2.y++;
                }else{
                    pos1.x--;
                    pos2.x++;
                }
                if(lastmap.contains(pos1)&&lastmap.contains(pos2)){
                    Drop *newdrop=new Drop(this,counter,Qt::black);
                    drops.insert(counter,newdrop);
                    counter++;
                    newscene.dropstatus.insert(newdrop,{move.pos,true,move.upward});
                    newscene.newcontaminations[move.pos].insert(newdrop->id);
                    for(auto it=newscene.dropstatus.begin();it!=newscene.dropstatus.end();it++){
                        if(it.value().pos==pos1){
                            newscene.dropstatus.erase(it);
                            break;
                        }
                    }
                    for(auto it=newscene.dropstatus.begin();it!=newscene.dropstatus.end();it++){
                        if(it.value().pos==pos2){
                            newscene.dropstatus.erase(it);
                            break;
                        }
                    }
                }else{
                    QMessageBox::critical(this, "Simulating Failed", "Merge error.");
                    return false;
                }
            }else if(move.type==6){
                //Merge2
                if(lastmap.contains(move.pos)&&newscene.dropstatus.value(lastmap.value(move.pos)).elliptical&&newscene.dropstatus.value(lastmap.value(move.pos)).vertical==move.upward){
                    newscene.dropstatus[lastmap.value(move.pos)].elliptical=false;
                }else{
                    QMessageBox::critical(this, "Simulating Failed", "Merge error.");
                    return false;
                }
            }
        }
        //test the validity of this scene
        //in the border
        foreach(Drop* drop,newscene.dropstatus.keys()){
            if(!intheborder(newscene.dropstatus.value(drop).pos)){
                QMessageBox::critical(this, "Simulating Failed", "Border exceeded.");
                return false;
            }
        }
        //static and dynamic constraint
        foreach(Drop* drop,newscene.dropstatus.keys()){
            foreach(Drop* drop1,newscene.dropstatus.keys()){
                if(drop!=drop1){
                    if(newscene.dropstatus.value(drop).elliptical){
                        Pos pos1(newscene.dropstatus.value(drop).pos),pos2(newscene.dropstatus.value(drop).pos);
                        if(newscene.dropstatus.value(drop).vertical){
                            pos1.y--;
                            pos2.y++;
                        }else{
                            pos1.x--;
                            pos2.x++;
                        }
                        if(!farenough(pos1,newscene.dropstatus.value(drop1).pos)){
                            QMessageBox::critical(this, "Simulating Failed", "Static constraint violation.");
                            return false;
                        }
                        if(!farenough(pos2,newscene.dropstatus.value(drop1).pos)){
                            QMessageBox::critical(this, "Simulating Failed", "Static constraint violation.");
                            return false;
                        }
                    }else{
                        if(!farenough(newscene.dropstatus.value(drop).pos,newscene.dropstatus.value(drop1).pos)){
                            QMessageBox::critical(this, "Simulating Failed", "Static constraint violation.");
                            return false;
                        }
                    }
                }

            }
            foreach(Drop* drop2,scenes[i-1].dropstatus.keys()){
                if(drop!=drop2){
                    if(!newscene.dropstatus.value(drop).elliptical){
                        Pos pos=newscene.dropstatus.value(drop).pos;
                        if(!scenes[i-1].dropstatus.value(drop2).elliptical){
                            Pos pos20(newscene.dropstatus.value(drop2).pos);
                            if(!farenough(pos,pos20)){
                                QMessageBox::critical(this, "Simulating Failed", "Static constraint violation.");
                                return false;
                            }
                        }
                    }
                }
            }
        }

        //if valid, add newscene and modify validtime
        scenes.push_back(newscene);
        validtime=i;
    }
    return true;
}

bool MainWindow::isadjacent(Pos pos1,Pos pos2){
    int dx,dy;
    dx=utils::abs(pos1.x,pos2.x);
    dy=utils::abs(pos1.y,pos2.y);
    return (dx==1&&dy==0)||(dx==0&&dy==1);
}
bool MainWindow::farenough(Pos pos1,Pos pos2){
    int dx,dy;
    dx=utils::abs(pos1.x,pos2.x);
    dy=utils::abs(pos1.y,pos2.y);
    return dx>1||dy>1;
}
bool MainWindow::intheborder(Pos pos){
    return pos.x>0&&pos.x<=setting.w&&pos.y>0&&pos.y<=setting.h;
}
void MainWindow::jumpto(int i){
    if(i<0)i=0;
    if(i>validtime)i=validtime;
    if(i==0)action_last->setEnabled(false);
    if(i==validtime){
        action_next->setEnabled(false);
        action_play->setEnabled(false);
    }
    if(status==2){
        if(i!=0)action_last->setEnabled(true);
        if(i!=validtime){
            action_next->setEnabled(true);
            action_play->setEnabled(true);
        }
    }
    ui->label->setText("Present time: "+QString::number(time)+"/"+QString::number(validtime));
    if(setting.washable&&i!=validtime){
        int restore=status;
        if(i!=0)lattice->getContaminations(i);
        lattice->setScene(i);
        QTime t;
        t.start();
        while(t.elapsed()<300)
            QCoreApplication::processEvents();
        status=4;
        setStatus(4);
        lattice->wash();
        lattice->refresh();
        status=restore;
        setStatus(status);
    }else{
        if(i!=0)lattice->getContaminations(i);
        lattice->setScene(i);
    }
    if(i==0)action_last->setEnabled(false);
    if(i==validtime){
        action_next->setEnabled(false);
        action_play->setEnabled(false);
    }
    if(status==2){
        if(i!=0)action_last->setEnabled(true);
        if(i!=validtime){
            action_next->setEnabled(true);
            action_play->setEnabled(true);
        }
    }
}
void MainWindow::nextstep(){
    time++;
    foreach(Movement move,movements[time-1]){
        if(move.type==2){
            sound_move->play();
        }else if(move.type==3){
            sound_split1->play();
        }else if(move.type==4){
            sound_split2->play();
        }else if(move.type==6){
            sound_merge->play();
        }
    }
    jumpto(time);
}
void MainWindow::laststep(){
    time--;
    jumpto(time);
}
void MainWindow::reset(){
    time=0;
    jumpto(0);
}
void MainWindow::play(){
    setStatus(3);
    status=3;
    while(time<validtime&&status==3){
        QTime t;
        t.start();
        while(t.elapsed()<300)
            QCoreApplication::processEvents();
        nextstep();
    }
    stop();
}
void MainWindow::stop(){
    setStatus(2);
    status=2;
    jumpto(time);
}
