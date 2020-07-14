#include "bird.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "define.h"
#include <iostream>
using namespace std;

//鸟跳动
qreal myEasingFunctions(qreal progress)
{
   return -BIRD_JUMP*(progress*progress-0.5*progress);

}

//鸟
bird::bird()
{
    status = 0;
    m_anim.setTargetObject(this);//设置目标对象
    m_anim.setPropertyName("pos");

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(changeStatus()));

    m_checkTimer = new QTimer(this);
    connect(m_checkTimer,SIGNAL(timeout()),this,SLOT(checkCollision()));
    m_checkTimer->start(20);
    isDied = false;

}

//边界
QRectF bird::boundingRect() const
{
    return BIRD_BOUNDING;
}

//画出鸟
void bird::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPixmap src(":/hero_1.png");

    painter->drawPixmap(BIRD_BOUNDING,src.copy(src.width()/4*status,src.height()/2,src.width()/4,src.height()/4),
                        QRectF(0,0,src.width()/4-20,src.height()/4-20));

}


//单击飞
void bird::ClickToFlay()
{
    if(isDied)
        return ;
    m_anim.stop();
    m_anim.setDuration(BIRD_FLAYTIME);
    m_anim.setStartValue(this->pos());//现在位置
    m_anim.setEndValue(this->pos()+QPoint(0,-20));
    m_curve.setCustomType(myEasingFunctions);

    m_anim.setEasingCurve(m_curve);
    m_anim.start();

    m_timer->start(BIRD_FLAYTIME/8);
    status = 0;

}

//让鸟动
void bird::changeStatus()
{
    status ++;
    this->scene()->views().value(0)->repaint();//画出鸟的动作
    if(status == 3)
    {
        m_timer->stop();
    }
}

//检测碰撞
void bird::checkCollision()
{
    if(this->collidingItems().size()!=0)//检测是否碰撞
    {
        CollisionToDeath();
    }
}

//鸟死亡
void bird::CollisionToDeath()
{
    m_timer->stop();
    m_checkTimer->stop();
    status = 3;
    this->scene()->views().value(0)->repaint();
    m_anim.stop();
    m_anim.setDuration(DEATH_TIME);
    m_anim.setStartValue(this->pos());
    m_anim.setEndValue(this->pos()+QPoint(0,20));
    m_anim.start();
    isDied = true;
    emit died();
}

//重新开始
void bird::reBegin()
{
    m_anim.stop();
    m_checkTimer->start();
    status = 0;
    isDied = false;
    this->scene()->views().value(0)->repaint();
}
