#include <fsm-editor/undo/DeleteTransition.h>
#include <fsm-editor/FSMScene.h>

#include <fsm-editor/fsm-elements/State.h>

DeleteTransition::DeleteTransition(FSMScene* scene, const QString& origin, const QString& destination)
  : QUndoCommand(QObject::tr("delete transition from %1 to %2", "Append to Undo").arg(origin).arg(destination))
  , scene_(scene)
  , origin_(origin)
  , destination_(destination)
{
  State* originState = scene_->getState(origin_);
  State* destinationState = scene_->getState(destination_);
  code_ = originState->getTransitionTo(destinationState)->getCode();
}

void DeleteTransition::undo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->transitionTo(destination, code_);
}

void DeleteTransition::redo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->removeTransitionTo(destination);
}
