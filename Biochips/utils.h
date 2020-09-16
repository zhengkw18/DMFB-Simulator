#ifndef UTILS_H
#define UTILS_H
#include <QColor>
#include <drop.h>
#include <QMap>
#include <QSet>
#include <QTime>
#include <QQueue>
#include <QList>
namespace utils{
template <typename T>
T max(T a,T b)
{
    return a>b?a:b;
}
template <typename T>
T abs(T a,T b){
    T c=a-b;
    return c>0?c:-c;
}
}
struct Pos{
    int x,y;
    bool operator == (const Pos & value) const{
        return (value.x==x&&value.y==y);
    }
    bool operator < (const Pos & value) const{
        if(x<value.x)return true;
        if(x>value.x)return false;
        if(y<value.y)return true;
        if(y>value.y)return false;
        return false;
    }
};

struct Setting{
    int w,h;
    QVector<Pos> in;
    Pos out;
    bool washable;
    Pos w_in,w_out;
};
struct DropStatus{
    Pos pos;
    bool elliptical;
    bool vertical;
};

struct Scene{
    QMap<Drop*,DropStatus> dropstatus;
    QMap<Pos,QSet<int>> newcontaminations;
};

struct Movement{
    int type; //0~6 corresponding to Input/Output/Move/Split1/Split2/Merge1/Merge2
    Pos pos;
    Pos destination;
    bool upward;
};
inline uint qHash(const Pos key)
{
    return key.x + key.y;
}
#endif // UTILS_H
