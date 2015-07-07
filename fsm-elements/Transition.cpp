#include <fsm-editor/fsm-elements/Transition.h>

#include <fsm-editor/FSMScene.h>
#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/undo/ChangeTransition.h>

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

const qreal Transition::LINK_SIZE = 8;
const qreal Transition::ARC = 25;
const QColor Transition::LINK_COLOR = QColor(220, 80, 80);
const QColor Transition::LINK_BORDER = QColor(210, 40, 40);

Transition::Transition(State* origin, State* destination)
  : origin_(origin)
  , destination_(destination)
{
  initialize();
}

Transition::Transition(State* origin)
  : origin_(origin)
  , destination_(nullptr)
  , hovered_(isUnderMouse())
  , parentHovered_(false)
{
  initialize();
}

void Transition::initialize()
{
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
  initPos();
  updateVisibility();
}

void Transition::initPos()
{
  setPos(origin_->pos());
}

bool Transition::isDangling() const
{
  return destination_ == nullptr && movingPos_.isNull();
}

QRectF Transition::boundingRect() const
{
  QRectF result;
  QList<QPolygonF> shape = calculateShape();
  for (QPolygonF poly : shape)
  {
    result = result.united(poly.boundingRect());
  }
  return result;
}

void Transition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  painter->setBrush(LINK_COLOR);
  painter->setPen(LINK_BORDER);
  QList<QPolygonF> shape = calculateShape();
  for (QPolygonF poly : shape)
  {
    painter->drawPolygon(poly);
  }
  calculateShape();
}

QList<QPolygonF> Transition::calculateShape() const
{
  QPainterPath result;
  QPolygonF triangle;
  triangle << QPointF(-LINK_SIZE, -LINK_SIZE / 2) << QPointF(0, 0) << QPointF(-LINK_SIZE, LINK_SIZE / 2);
  if (isDangling())
  {
    return calculateDangling(result, triangle);
  }
  else if (movingPos_.isNull())
  {
    return calculatePluggedArrow(result, triangle);
  }
  else
  {
    return calculateMovingArrow(result, triangle);
  }
}

QList<QPolygonF> Transition::calculatePluggedArrow(QPainterPath &result, const QPolygonF& triangle) const
{
  QList<QPolygonF> polys;
  QPointF originPoint = getIntersection(mapFromItem(origin_, origin_->rect()).boundingRect());
  QPointF destPoint = getIntersection(mapFromItem(destination_, destination_->rect()).boundingRect());
  QLineF line(originPoint, destPoint);
  addBentLine(originPoint, line, destPoint, polys);
  result.addPolygon(triangle);
  QTransform transformation;
  transformation.translate(destPoint.x(), destPoint.y());
  transformation.rotate(-line.angle() + ARC);
  polys << result.toSubpathPolygons(transformation);
  return polys;
}

void Transition::addBentLine(const QPointF& originPoint, const QLineF &line, const QPointF& destPoint, QList<QPolygonF>& polys) const
{
  QPainterPath arc;
  arc.moveTo(originPoint);
  QPointF p1(QLineF::fromPolar(line.length() / 3, line.angle() + ARC).translated(originPoint).p2());
  QPointF p2(QLineF::fromPolar(line.length() / 3, line.angle() + 180 - ARC).translated(destPoint).p2());
  arc.cubicTo(p1, p2, destPoint);
  arc.cubicTo(p2, p1, originPoint);
  polys << arc.toSubpathPolygons(QTransform());
}

QList<QPolygonF> Transition::calculateMovingArrow(QPainterPath result, const QPolygonF& triangle) const
{
  QList<QPolygonF> polys;
  QPointF originPoint = mapFromItem(origin_, origin_->rect().topRight());
  QPointF destPoint = mapFromScene(movingPos_);
  QLineF line(originPoint, destPoint);
  QPolygonF linePoly;
  linePoly << originPoint << destPoint;
  polys << linePoly;
  result.addPolygon(triangle);
  QTransform transformation;
  transformation.translate(destPoint.x(), destPoint.y());
  transformation.rotate(-line.angle());
  polys << result.toSubpathPolygons(transformation);
  return polys;
}

QList<QPolygonF> Transition::calculateDangling(QPainterPath &result, const QPolygonF& triangle) const
{
  QTransform transformation;
  transformation.translate(origin_->rect().width(), 0.);
  transformation.rotate(-45);
  result.addPolygon(triangle);
  return result.toSubpathPolygons(transformation);
}

QPointF Transition::getIntersection(const QRectF& rect) const
{
  QPointF res;
  QLineF wholeLine(mapFromItem(origin_, origin_->rect().center()), destinationPoint());
  QLineF top(rect.topLeft(), rect.topRight());
  if (wholeLine.intersect(top, &res) == QLineF::BoundedIntersection)
    return res;
  QLineF left(rect.topLeft(), rect.bottomLeft());
  if (wholeLine.intersect(left, &res) == QLineF::BoundedIntersection)
    return res;
  QLineF right(rect.topRight(), rect.bottomRight());
  if (wholeLine.intersect(right, &res) == QLineF::BoundedIntersection)
    return res;
  QLineF bottom(rect.bottomLeft(), rect.bottomRight());
  if (wholeLine.intersect(bottom, &res))
    return res;
  Q_ASSERT_X(false, "Transition::getIntersection", "Given rectangle doesn't intersect with line");
  return rect.center();
}

QPointF Transition::destinationPoint() const
{
  if (movingPos_.isNull())
    return mapFromItem(destination_, destination_->rect().center());
  return mapFromScene(movingPos_);
}

bool Transition::hasDestination() const
{
  return destination_ != nullptr;
}

State* Transition::destination() const
{
  return destination_;
}

void Transition::setParentOvered(bool hovered)
{
  parentHovered_ = hovered;
  updateVisibility();
}

void Transition::updateVisibility()
{
  setVisible(destination_ != nullptr || parentHovered_ || hovered_);
}

void Transition::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  hovered_ = true;
  updateVisibility();
  super::hoverEnterEvent(event);
}

void Transition::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  hovered_ = false;
  updateVisibility();
  super::hoverLeaveEvent(event);
}

void Transition::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  super::mouseMoveEvent(event);
  prepareGeometryChange();
  movingPos_ = event->scenePos();
}

void Transition::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  movingPos_ = QPointF();
  QList<QGraphicsItem*> items = scene()->collidingItems(this);
  for (QGraphicsItem* item : items)
  {
    State* child = dynamic_cast<State*>(item);
    if (child != nullptr)
    {
      origin_->pushCommand(new ChangeTransition(static_cast<FSMScene*>(scene()), origin_->title(), child->title()));
      initPos();
      break;
    }
  }
  super::mouseReleaseEvent(event);
}
