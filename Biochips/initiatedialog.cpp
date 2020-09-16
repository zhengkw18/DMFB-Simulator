#include "initiatedialog.h"
#include "ui_initiatedialog.h"

InitiateDialog::InitiateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitiateDialog)
{
    ui->setupUi(this);
    this->setFixedSize(600,400);
    this->setWashState(Qt::Unchecked);
    ui->spinBox->setRange(1,12);
    ui->spinBox_2->setRange(1,12);
    ui->spinBox_5->setRange(1,12);
    ui->spinBox_6->setRange(1,12);
    ui->spinBox_7->setRange(1,12);
    ui->spinBox_8->setRange(1,12);
    ui->spinBox_9->setRange(1,12);
    ui->spinBox_10->setRange(1,12);
    QRegExp regExp("^(([1-9]|1[0-2]){1},){0,}([1-9]|1[0-2]){1}$");
    QRegExpValidator *pRegExpValidator = new QRegExpValidator(regExp,this);
    ui->lineEdit->setValidator(pRegExpValidator);
    ui->lineEdit_2->setValidator(pRegExpValidator);
}

InitiateDialog::~InitiateDialog()
{
    delete ui;
}

Setting InitiateDialog::getSetting(){
    this->exec();
    return setting;
}

void InitiateDialog::on_checkBox_stateChanged(int arg1)
{
    setWashState(ui->checkBox->checkState());
}
//OK
void InitiateDialog::on_pushButton_2_clicked()
{
    int width,height,outputx,outputy,winputx,winputy,woutputx,woutputy;
    QVector<int> inputx,inputy;
    bool washable;
    width=ui->spinBox->value();
    height=ui->spinBox_2->value();
    QString ix=ui->lineEdit->text();
    QString iy=ui->lineEdit_2->text();
    QStringList ixl=ix.split(",", QString::SkipEmptyParts);
    QStringList iyl=iy.split(",", QString::SkipEmptyParts);
    if(ixl.size()!=iyl.size()){
        QMessageBox::critical(this, "Data unqualified", "Input num ambivalent.");
        return;
    }
    for(int i=0;i<ixl.size();i++){
        int ixi=ixl.at(i).toInt();
        inputx.push_back(ixi);
        int iyi=iyl.at(i).toInt();
        inputy.push_back(iyi);
    }
    outputx=ui->spinBox_5->value();
    outputy=ui->spinBox_6->value();
    winputx=ui->spinBox_7->value();
    winputy=ui->spinBox_8->value();
    woutputx=ui->spinBox_9->value();
    woutputy=ui->spinBox_10->value();
    washable=(ui->checkBox->checkState()==Qt::Checked);
    if(width<=3 && height<=3){
        QMessageBox::critical(this, "Data unqualified", "Width and height cannot be both less than or equal to 3.");
        return;
    }
    for(int i=0;i<inputx.size();i++){
        if((inputx[i]!=1&&inputx[i] !=width)&&(inputy[i]!=1&&inputy[i]!=height)){
            QMessageBox::critical(this, "Data unqualified", "Input shall be on the border.");
            return;
        }
    }

    if((outputx!=1&&outputx !=width)&&(outputy!=1&&outputy!=height)){
        QMessageBox::critical(this, "Data unqualified", "Output shall be on the border.");
        return;
    }
    for(int i=0;i<inputx.size();i++){
        if(inputx[i]==outputx&&inputy[i]==outputy){
            QMessageBox::critical(this, "Data unqualified", "Input and output shall differ in terms of position.");
            return;
        }
    }
    for(int i=0;i<inputx.size();i++){
        if(inputx[i]>width||outputx>width||inputy[i]>height||outputy>height){
            QMessageBox::critical(this, "Data unqualified", "Input and output shall be on the border.");
            return;
        }
    }

    if(washable){
        if((winputx!=1&&winputx !=width)&&(winputy!=1&&winputy!=height)){
            QMessageBox::critical(this, "Data unqualified", "Wash input shall be on the border.");
            return;
        }
        if((woutputx!=1&&woutputx !=width)&&(woutputy!=1&&woutputy!=height)){
            QMessageBox::critical(this, "Data unqualified", "Wash output shall be on the border.");
            return;
        }
        if(winputx==woutputx&&winputy==woutputy){
            QMessageBox::critical(this, "Data unqualified", "Wash input and wash output shall differ in terms of position.");
            return;
        }
        for(int i=0;i<inputx.size();i++){
            if(winputx==inputx[i]&&winputy==inputy[i]){
                QMessageBox::critical(this, "Data unqualified", "Wash input and input shall differ in terms of position.");
                return;
            }
        }

        if(winputx==outputx&&winputy==outputy){
            QMessageBox::critical(this, "Data unqualified", "Wash input and output shall differ in terms of position.");
            return;
        }
        for(int i=0;i<inputx.size();i++){
            if(woutputx==inputx[i]&&woutputy==inputy[i]){
                QMessageBox::critical(this, "Data unqualified", "Wash output and input shall differ in terms of position.");
                return;
            }
        }
        if(woutputx==outputx&&woutputy==outputy){
            QMessageBox::critical(this, "Data unqualified", "Wash output and output shall differ in terms of position.");
            return;
        }
        if(winputx>width||woutputx>width||winputy>height||woutputy>height){
            QMessageBox::critical(this, "Data unqualified", "Wash input and wash output shall be on the border.");
            return;
        }
    }

    setting.w=width;
    setting.h=height;
    setting.in.clear();
    for(int i=0;i<inputx.size();i++){
        Pos temppos;
        temppos.x=inputx[i];
        temppos.y=inputy[i];
        setting.in.push_back(temppos);
    }
    setting.out.x=outputx;
    setting.out.y=outputy;
    setting.w_in.x=winputx;
    setting.w_in.y=winputy;
    setting.w_out.x=woutputx;
    setting.w_out.y=woutputy;
    setting.washable=washable;
    this->close();
}
//Cancel
void InitiateDialog::on_pushButton_clicked()
{
    this->close();
}

void InitiateDialog::setWashState(Qt::CheckState state){
    washstate=state;
    if(state==Qt::Checked){
        ui->spinBox_7->setEnabled(true);
        ui->spinBox_8->setEnabled(true);
        ui->spinBox_9->setEnabled(true);
        ui->spinBox_10->setEnabled(true);
    }else{
        ui->spinBox_7->setEnabled(false);
        ui->spinBox_8->setEnabled(false);
        ui->spinBox_9->setEnabled(false);
        ui->spinBox_10->setEnabled(false);
    }
}
