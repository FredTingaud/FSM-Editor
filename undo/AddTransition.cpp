#include <fsm-editor/undo/AddTransition.h>
#include <fsm-editor/FSMScene.h>

#include <fsm-editor/fsm-elements/State.h>

AddTransition::AddTransition(FSMScene* scene, const QString& origin, const QString& destination)
  : QUndoCommand(QString("Transition from %1 to %2").arg(origin).arg(destination))
  , scene_(scene)
  , origin_(origin)
  , destination_(destination)
{}

void AddTransition::undo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->RemoveTransitionTo(destination);
}

void AddTransition::redo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->transitionTo(destination);
}
