#include <fsm-editor/undo/AddTransition.h>
#include <fsm-editor/FSMScene.h>

#include <fsm-editor/fsm-elements/State.h>

AddTransition::AddTransition(FSMScene* scene, const QString& origin, const QString& destination)
  : QUndoCommand(QObject::tr("create transition from %1 to %2", "Append to Undo").arg(origin).arg(destination))
  , scene_(scene)
  , origin_(origin)
  , destination_(destination)
  , code_("")
{}

AddTransition::AddTransition(FSMScene* scene, const QString& origin, const QString& destination, const QString& code)
  : QUndoCommand(QObject::tr("create transition from %1 to %2", "Append to Undo").arg(origin).arg(destination))
  , scene_(scene)
  , origin_(origin)
  , destination_(destination)
  , code_(code)
{}

void AddTransition::undo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->removeTransitionTo(destination);
}

void AddTransition::redo()
{
  State* origin = scene_->getState(origin_);
  State* destination = scene_->getState(destination_);
  origin->transitionTo(destination, code_);
}
