#ifndef QTNODE_H
#define QTNODE_H

#include <QGraphicsItem>
#include <QVector>
#include "src/Vertex.hpp"

class QtEdge;
class GraphWidget;

//! [0]
class QtNode : public QGraphicsItem, public Vertex
{
public:
    QtNode(GraphWidget *graphWidget);

    void addQtEdge(QtEdge *edge);
    QVector<QtEdge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void calculateForces();
    bool advancePosition();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void paintChosen();
    void paintDefault();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<QtEdge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
    QColor fillColor = Qt::blue;
};
//! [0]

#endif // NODE_H
