#include <fsm-editor/undo/ChangeTransition.h>
#include <fsm-editor/FSMScene.h>

#include <fsm-editor/fsm-elements/State.h>

ChangeTransition::ChangeTransition(FSMScene* scene, const QString& origin, const QString& destination)
  : QUndoCommand(QString("Transition from %1 to %2").arg(origin).arg(destination))
  , scene_(scene)
  , origin_(origin)
  , destination_(destination)
{}

void ChangeTransition::undo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->RemoveTransitionTo(destination);
}

void ChangeTransition::redo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->transitionTo(destination);
}
