#include <fsm-editor/undo/MoveStateCommand.h>
#include <fsm-editor/FSMScene.h>
#include <fsm-editor/fsm-elements/State.h>

MoveStateCommand::MoveStateCommand(FSMScene* scene, const QString& name, const QPointF& position, State* state)
  : QUndoCommand(QString("Move state %1").arg(name))
  , first_(true)
  , scene_(scene)
  , name_(name)
  , previousPos_(state->pos())
  , newPos_(position)
{}

void MoveStateCommand::undo()
{
  State* state = scene_->getState(name_);
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
    State* state = scene_->getState(name_);
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
