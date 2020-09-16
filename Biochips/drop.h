#ifndef DROP_H
#define DROP_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class Drop : public QObject
{
    Q_OBJECT
public:
    explicit Drop(QObject *parent,int id,QColor color);
    int id;
    QColor color;

signals:

public slots:
};

#endif // DROP_H
