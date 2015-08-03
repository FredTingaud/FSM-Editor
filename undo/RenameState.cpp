#include <fsm-editor/undo/RenameState.h>

#include <fsm-editor/FSMScene.h>

RenameState::RenameState(FSMScene* scene, const QString& oldName, const QString& newName)
  : QUndoCommand(QString("Rename state %1 to %2").arg(oldName).arg(newName))
  , scene_(scene)
  , oldName_(oldName)
  , newName_(newName)
{}

void RenameState::undo()
{
  State* state = scene_->getState(newName_);
  scene_->setStateName(state, oldName_);
}

void RenameState::redo()
{
  State* state = scene_->getState(oldName_);
  scene_->setStateName(state, newName_);
}
