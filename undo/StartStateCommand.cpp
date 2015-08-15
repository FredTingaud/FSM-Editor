#include <fsm-editor/undo/StartStateCommand.h>

#include <fsm-editor/FSMScene.h>
#include <fsm-editor/fsm-elements/State.h>

StartStateCommand::StartStateCommand(FSMScene* scene, State* state)
  : QUndoCommand(QObject::tr("change start state to %1", "Append to Undo").arg(state->name()))
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
