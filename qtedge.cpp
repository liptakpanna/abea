#include "qtedge.h"
#include "qtnode.h"

#include <QPainter>
#include <QtMath>

//! [0]
QtEdge::QtEdge(QtNode *sourceQtNode, QtNode *destQtNode)
    : source(sourceQtNode), dest(destQtNode)
{
    setAcceptedMouseButtons(Qt::NoButton);
    source->addQtEdge(this);
    dest->addQtEdge(this);
    adjust();
}
//! [0]

//! [1]
QtNode *QtEdge::sourceQtNode() const
{
    return source;
}

QtNode *QtEdge::destQtNode() const
{
    return dest;
}
//! [1]

//! [2]
void QtEdge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}
//! [2]

//! [3]
QRectF QtEdge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [3]

//! [4]
void QtEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
//! [4]

//! [5]

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);
    painter->setPen(QPen(Qt::black, 1));
    QRect background(line.center().x(), line.center().y(), 25,15);
    painter->drawRect(background);
    QBrush backgroundColor(Qt::white);
    painter->fillRect(background, backgroundColor);
    painter->setPen(QPen(Qt::red, 1));
    painter->drawText(background,Qt::AlignCenter, QString::number(this->getPropagationProbability()));
//! [5]

//! [6]
    // Draw the arrows

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
//! [6]
