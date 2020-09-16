#ifndef LATTICE_H
#define LATTICE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <utils.h>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QSound>
#include <QApplication>
#include <QMessageBox>

class Lattice : public QWidget
{
    Q_OBJECT
public:
    explicit Lattice(QWidget *parent,Setting setting,int pos1,int pos2);
    void load(int vtime,QVector<Scene> ss,QMap<int,Drop*> ds);
    void setScene(int t);
    void wash();
    void getContaminations(int t);
    void refresh();
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent * event) override;
signals:

public slots:

private:
    int status=0;//0/1/2 Empty/Scene/WashScene
    int validtime=0;
    int time=0;
    Setting setting;
    QVector<Scene> scenes;
    QMap<int,Drop*> drops;
    QMap<int,Pos> pollutionpos;
    QVector<QMap<Pos,QSet<int>>> contaminations;
    QVector<Pos> path;
    QSet<Pos> locked;
    int w_inpos,w_outpos;
    int nowpath;
    QSound *sound_move;
};

#endif // LATTICE_H
