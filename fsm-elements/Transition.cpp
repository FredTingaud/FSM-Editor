#include <fsm-editor/fsm-elements/Transition.h>

#include <fsm-editor/fsm-elements/State.h>

#include <QPainter>

const qreal Transition::LINK_SIZE = 8;
const QColor Transition::LINK_COLOR = QColor(220, 80, 80);

Transition::Transition(State* origin)
  : origin_(origin)
  , destination_(nullptr)
  , hovered_(isUnderMouse())
  , parentHovered_(false)
{
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
  setPos(origin_->pos() + origin_->rect().topRight() - QPointF(LINK_SIZE, 0));
  updateVisibility();
}

QRectF Transition::boundingRect() const
{
  return QRectF(0, 0, LINK_SIZE, LINK_SIZE);
}

void Transition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  QPolygonF triangle;
  QPointF topRight = boundingRect().topRight();
  triangle << topRight << topRight + QPointF(-LINK_SIZE, 0) << topRight + QPointF(0, LINK_SIZE);
  painter->setBrush(LINK_COLOR);
  painter->drawPolygon(triangle);
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
  setVisible(parentHovered_ || hovered_);
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
