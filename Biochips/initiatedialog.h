#ifndef INITIATEDIALOG_H
#define INITIATEDIALOG_H

#include <QDialog>
#include <utils.h>
#include <QMessageBox>

namespace Ui {
class InitiateDialog;
}

class InitiateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitiateDialog(QWidget *parent = nullptr);
    ~InitiateDialog();
    Setting getSetting();

private slots:


    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::InitiateDialog *ui;
    Setting setting;
    void setWashState(Qt::CheckState);
    Qt::CheckState washstate;
};

#endif // INITIATEDIALOG_H
