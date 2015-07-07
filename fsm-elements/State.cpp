#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/fsm-elements/Transition.h>
#include <fsm-editor/FSMScene.h>
#include <fsm-editor/undo/MoveStateCommand.h>
#include <fsm-editor/undo/DeleteStateCommand.h>

#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

const qreal State::WIDTH = 50;
const qreal State::HEIGHT = 20;
const qreal State::H_MARGIN = 4;
const qreal State::V_MARGIN = 4;
const QColor State::PEN_COLOR = QColor(190, 190, 190);

State::State(const QString& title, const QPointF& position, std::function<void(QUndoCommand*)>&& pushStack)
  : QGraphicsRectItem(QRectF(0, 0, WIDTH, HEIGHT))
  , title_(title)
  , pushStack_(std::move(pushStack))
  , silent_(false)
{
  setPen(PEN_COLOR);
  setPos(position);
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
  setAcceptHoverEvents(true);
}

State::~State()
{
  for (Transition* transition : transitions_)
  {
    if (transition->scene())
    {
      transition->scene()->removeItem(transition);
    }
    delete transition;
  }
}

void State::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  super::paint(painter, option, widget);

  shrinkTextToFit(painter);
  painter->drawText(rect(), Qt::AlignCenter, title_);
}

void State::shrinkTextToFit(QPainter * painter)
{
  QFontMetrics metrics = painter->fontMetrics();
  QFont font = painter->font();
  while (!fitInRect(rect(), metrics.boundingRect(title_)) && font.pointSize() > 3)
  {
    font.setPointSize(font.pointSize() - 1);
    painter->setFont(font);
    metrics = painter->fontMetrics();
  }
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
    for (Transition* child : transitions_)
    {
      child->initPos();
    }
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

void State::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Delete)
  {
    pushStack_(new DeleteStateCommand(scene(), title_, pos()));
    event->accept();
  }
  else
  {
    super::keyPressEvent(event);
  }
}

void State::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  if (transitions_.isEmpty() || transitions_.last()->hasDestination())
  {
    Transition* danglingTransition = new Transition(this);
    transitions_.append(danglingTransition);
    scene()->addItem(danglingTransition);
  }
  transitions_.last()->setParentOvered(true);
  super::hoverEnterEvent(event);
}

void State::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  transitions_.last()->setParentOvered(false);
  super::hoverLeaveEvent(event);
}

FSMScene* State::scene() const
{
  if (super::scene())
    return static_cast<FSMScene*>(super::scene());
  return nullptr;
}
