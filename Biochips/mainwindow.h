#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSound>
#include <lattice.h>
#include <QVector>
#include <drop.h>
#include <QMap>
#include <QSet>
#include <utils.h>
#include <initiatedialog.h>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void Initiate();
    void Load();
    void nextstep();
    void laststep();
    void reset();
    void play();
    void stop();
signals:

private slots:


private:
    Ui::MainWindow *ui;
    int status;//0~3 corresponding to Vacant/Setted/Loaded/Playing/Washing
    QAction *action_startup,*action_import,*action_about,*action_reset,*action_play,*action_stop,*action_last,*action_next;
    QSound *sound_move,*sound_split1,*sound_split2,*sound_merge;
    QLabel *label_output,*label_washinput,*label_waste;
    QVector<QLabel*> label_inputs;
    void jumpto(int i);
    int time=-1;
    Lattice* lattice=nullptr;
    void setStatus(int id);
    QVector<Scene> scenes;
    Setting setting;
    int counter=0;
    QMap<int,Drop*> drops;
    Pos getWidgePos(Pos pos);
    Pos transferPos(Pos pos);
    int totaltime=0;
    int validtime=0;
    QVector<Movement> movements[200];
    void transferOrdertoMovements(QString order);
    bool processSimulation();
    bool isadjacent(Pos pos1,Pos pos2);
    bool farenough(Pos pos1,Pos pos2);
    bool intheborder(Pos pos);
    int w_inpos,w_outpos;
    int temppos;
};
#endif // MAINWINDOW_H
