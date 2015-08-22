#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/fsm-elements/Transition.h>
#include <fsm-editor/FSMScene.h>
#include <fsm-editor/ExportVisitor.h>
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
const QColor State::START_PEN_COLOR = QColor(180, 235, 100);
const QString State::START_ICON = ":/ic_flag.png";
const QString State::ERROR_ICON = ":/ic_error_outline.png";

State::State(const QString& title, const QPointF& position, std::function<void(QUndoCommand*)>&& pushStack)
  : QGraphicsRectItem(QRectF(0, 0, WIDTH, HEIGHT))
  , title_(title)
  , pushStack_(std::move(pushStack))
  , silent_(false)
  , dangling_(this)
  , start_(false)
{
  setPen(PEN_COLOR);
  setPos(position);
  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
  setAcceptHoverEvents(true);
  originalRect_ = rect();
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
  painter->drawText(originalRect_, Qt::AlignCenter, title_);
  if (start_)
  {
    QRectF right(WIDTH, 0, HEIGHT, HEIGHT);
    painter->fillRect(right, pen().color());
    QPixmap flag = QPixmap(START_ICON);
    painter->drawPixmap(right, flag, QRectF(0, 0, flag.width(), flag.height()));
  }
  if (isInError())
  {
    QRectF left(-HEIGHT, 0, HEIGHT, HEIGHT);
    painter->fillRect(left, Qt::red);
    QPixmap errorIcon = QPixmap(ERROR_ICON);
    painter->drawPixmap(left, errorIcon, QRectF(0, 0, errorIcon.width(), errorIcon.height()));
  }
}

void State::shrinkTextToFit(QPainter * painter)
{
  QFontMetrics metrics = painter->fontMetrics();
  QFont font = painter->font();
  while (!fitInRect(originalRect_, metrics.boundingRect(title_)) && font.pointSize() > 3)
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
    reactToPositionChange();
    pushStack_(new MoveStateCommand(scene(), title_, value.toPointF(), this));
  }
  else if (change == QGraphicsItem::ItemSceneHasChanged)
  {
    if (scene())
    {
      dangling_.updatePos();
      scene()->addItem(&dangling_);
    }
  }
  else if (change == QGraphicsItem::ItemSelectedChange)
  {
    if (value.toBool())
    {
      scene()->setCode(this, getCode());
    }
  }
  return result;
}

void State::reactToPositionChange()
{
  updateTransitionsPositions(transitions_);
  updateTransitionsPositions(pointingTransitions_);
  dangling_.updatePos();
}

QString State::name() const
{
  return title_;
}

void State::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  dangling_.setParentHovered(true);
  super::hoverEnterEvent(event);
}

void State::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  dangling_.setParentHovered(false);
  super::hoverLeaveEvent(event);
}

FSMScene* State::scene() const
{
  if (super::scene())
    return static_cast<FSMScene*>(super::scene());
  return nullptr;
}

void State::pushCommand(QUndoCommand* command)
{
  pushStack_(command);
}

void State::transitionTo(State* destination, const QString& code /*= ""*/)
{
  for (Transition* t : transitions_)
  {
    if (t->destination() == destination)
    {
      return;
    }
  }
  Transition* transition = new Transition(this, destination);
  transitions_.append(transition);
  scene()->addItem(transition);
  destination->setPointedBy(transition, true);
  transition->setCode(code);
  scene()->setSelectedItem(transition);
}

void State::removeTransitionTo(State* destination)
{
  Transition* t = getTransitionTo(destination);
  transitions_.removeOne(t);
  destination->setPointedBy(t, false);
  delete t;
}

void State::setPointedBy(Transition* transition, bool pointed)
{
  if (pointed)
  {
    pointingTransitions_.append(transition);
  }
  else
  {
    pointingTransitions_.removeOne(transition);
  }
}

QString State::getCode() const
{
  return FSMElement::getCode();
}

void State::setCode(const QString& code)
{
  FSMElement::setCode(code);
  setToolTip(code);
  scene()->setCode(this, getCode());
}

void State::setInError(const QString& error)
{
  FSMElement::setInError(error);
  updateRect();
}

void State::clearError()
{
  FSMElement::clearError();
  updateRect();
}

Transition* State::getTransitionByName(const QString& name) const
{
  for (Transition* transition : transitions_)
  {
    if (transition->name() == name)
      return transition;
  }
  return nullptr;
}

QString State::visit(ExportVisitor& visitor) const
{
  return visitor.exportElement(*this);
}

QList<Transition*> State::getAllRelatedTransitions() const
{
  QList<Transition*> result;
  result.append(transitions_);
  result.append(pointingTransitions_);
  return result;
}

QList<Transition*> State::getTransitions() const
{
  QList<Transition*> result;
  result.append(transitions_);
  return result;
}

void State::silentlySetPosition(const QPointF& position)
{
  silent_ = true;
  setPos(position);
  reactToPositionChange();
  silent_ = false;
}

QPointF State::getPosition() const
{
  return pos();
}

void State::setName(const QString& name)
{
  title_ = name;
  update();
}

bool State::isStart() const
{
  return start_;
}

void State::setStart(bool start)
{
  start_ = start;
  updateRect();
  update();
}

void State::updateRect()
{
  QRectF result = originalRect_;
  if (start_)
  {
    result.setWidth(result.width() + HEIGHT);
  }
  if (isInError())
  {
    result.setX(-HEIGHT);
  }
  if (result != rect())
  {
    setRect(result);
    reactToPositionChange();
  }
}

Transition* State::getTransitionTo(State* destination) const
{
  for (Transition* t : transitions_)
  {
    if (t->destination() == destination)
    {
      return t;
    }
  }
  return nullptr;
}

void State::updateTransitionsPositions(QList<Transition*>& transitions)
{
  for (Transition* child : transitions)
  {
    child->updatePos();
  }
}
