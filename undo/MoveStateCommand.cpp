#include <fsm-editor/undo/MoveStateCommand.h>
#include <fsm-editor/FSMScene.h>
#include <fsm-editor/fsm-elements/State.h>

MoveStateCommand::MoveStateCommand(QGraphicsScene* scene, const QString& name, const QPointF& position, State* state)
  : QUndoCommand(QString("Move state %1").arg(name))
  , first_(true)
  , scene_(scene)
  , name_(name)
  , previousPos_(state->pos())
  , newPos_(position)
{}

void MoveStateCommand::undo()
{
  State* state = findState(newPos_);
  state->setSilentMove(true);
  state->setPos(previousPos_);
  state->setSilentMove(false);
}

void MoveStateCommand::redo()
{
  if (first_)
  {
    first_ = false;
  }
  else
  {
    State* state = findState(previousPos_);
    state->setSilentMove(true);
    state->setPos(newPos_);
    state->setSilentMove(false);
  }
}

int MoveStateCommand::id() const
{
  return FSMScene::UNDO_IDS::UNDO_MOVE;
}

bool MoveStateCommand::mergeWith(const QUndoCommand *other)
{
  if (other->id() != id()
      || static_cast<const MoveStateCommand*>(other)->previousPos_ != newPos_
      || static_cast<const MoveStateCommand*>(other)->name_ != name_
      )
      return false;
  newPos_ = static_cast<const MoveStateCommand*>(other)->newPos_;
  return true;
}

State* MoveStateCommand::findState(const QPointF& position)
{
  QGraphicsItem* item = scene_->itemAt(position, QTransform());
  State* state = dynamic_cast<State*>(item);
  if (state != nullptr && state->title() == name_)
  {
    return state;
  }
  throw std::exception(qPrintable("State not found: " + name_));
}
