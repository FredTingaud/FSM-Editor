#include <fsm-editor/undo/StartStateCommand.h>

#include <fsm-editor/FSMScene.h>
#include <fsm-editor/fsm-elements/State.h>

StartStateCommand::StartStateCommand(FSMScene* scene, State* state)
  : QUndoCommand(QObject::tr("Change start state to %1").arg(state->name()))
  , scene_(scene)
  , previousStart_(scene->getStartState()->name())
  , nextStart_(state->name())
{}

void StartStateCommand::undo()
{
  scene_->changeStartState(scene_->getState(previousStart_));
}

void StartStateCommand::redo()
{
  scene_->changeStartState(scene_->getState(nextStart_));
}
