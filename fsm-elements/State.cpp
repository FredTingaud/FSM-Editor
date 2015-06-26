#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/undo/MoveStateCommand.h>

#include <QPainter>
#include <QDebug>

const qreal State::WIDTH = 50;
const qreal State::HEIGHT = 20;
const qreal State::H_MARGIN = 4;
const qreal State::V_MARGIN = 4;

State::State(QString title, const QPointF& position, std::function<void(QUndoCommand*)>&& pushStack)
  : QGraphicsRectItem(QRectF(0, 0, WIDTH, HEIGHT))
  , silent_(false)
  , title_(title)
  , pushStack_(std::move(pushStack))
{
  setPos(position);
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
}

void State::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  super::paint(painter, option, widget);
  QFontMetrics metrics = painter->fontMetrics();
  QFont font = painter->font();
  while (!fitInRect(rect(), metrics.boundingRect(title_)) && font.pointSize() > 3)
  {
    font.setPointSize(font.pointSize() - 1);
    painter->setFont(font);
    metrics = painter->fontMetrics();
  }
  painter->drawText(rect(), Qt::AlignCenter, title_);
}

bool State::fitInRect(const QRectF& rect, const QRect& bounding)
{
  return rect.width() > bounding.width() + H_MARGIN && rect.height() > bounding.height() + V_MARGIN;
}

QVariant State::itemChange(GraphicsItemChange change, const QVariant &value)
{
  QVariant result = super::itemChange(change, value);
  if (!silent_ && change == QGraphicsItem::ItemPositionChange)
  {
    pushStack_(new MoveStateCommand(scene(), title_, value.toPointF(), this));
  }
  return result;
}

void State::setSilentMove(bool silent)
{
  silent_ = silent;
}

QString State::title() const
{
  return title_;
}
