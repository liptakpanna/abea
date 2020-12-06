#ifndef QTEDGE_H
#define QTEDGE_H

#include <QGraphicsItem>
#include "src/Edge.hpp"

class QtNode;

//! [0]
class QtEdge : public QGraphicsItem, public Edge
{
public:
    QtEdge(QtNode *sourceQtNode, QtNode *destQtNode);

    QtNode *sourceQtNode() const;
    QtNode *destQtNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QtNode *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize = 10;
};
//! [0]

#endif // EDGE_H
