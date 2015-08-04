#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/fsm-elements/Transition.h>
#include <fsm-editor/FSMScene.h>
#include <fsm-editor/ExportVisitor.h>
#include <fsm-editor/undo/MoveStateCommand.h>
#include <fsm-editor/undo/DeleteStateCommand.h>

#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QInputDialog>
#include <QMessageBox>

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
  , dangling_(this)
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
    for (Transition* child : pointingTransitions_)
    {
      child->initPos();
    }
    dangling_.initPos();
    pushStack_(new MoveStateCommand(scene(), title_, value.toPointF(), this));
  }
  else if (change == QGraphicsItem::ItemSceneHasChanged)
  {
    if (scene())
    {
      dangling_.initPos();
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

void State::setSilentMove(bool silent)
{
  silent_ = silent;
}

QString State::name() const
{
  return title_;
}

void State::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Delete)
  {
    pushStack_(new DeleteStateCommand(scene(), this));
    event->accept();
  }
  else if (event->key() == Qt::Key_F2)
  {
    askRename();
  }
  else
  {
    super::keyPressEvent(event);
  }
}

void State::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  askRename();
}

void State::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  dangling_.setParentOvered(true);
  super::hoverEnterEvent(event);
}

void State::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  dangling_.setParentOvered(false);
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
  scene()->clearSelection();
  transition->setSelected(true);
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

Transition* State::getElement(const QString& name) const
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

QPointF State::getPosition() const
{
  return pos();
}

void State::setName(const QString& name)
{
  title_ = name;
  update();
}

void State::askRename()
{
  bool ok;
  QString newName = QInputDialog::getText(0, QObject::tr("Rename state %1").arg(name()), QObject::tr("Rename to:"), QLineEdit::Normal, title_, &ok);
  if (ok)
  {
    QString error = scene()->renameState(this, newName);
    if (!error.isEmpty())
    {
      QMessageBox::warning(0, QObject::tr("Couldn't rename to %1").arg(newName)
                           , QObject::tr("The state couldn't be renamed to %1 because of the following error:\n%2").arg(newName).arg(error));
    }
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
