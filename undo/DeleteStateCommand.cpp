#include <fsm-editor/undo/DeleteStateCommand.h>
#include <fsm-editor/undo/DeleteTransition.h>
#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/FSMScene.h>

#include <QTransform>

DeleteStateCommand::DeleteStateCommand(FSMScene* scene, State* state)
  : QUndoCommand(QObject::tr("delete state %1", "Append to Undo").arg(state->name()))
  , scene_(scene)
  , name_(state->name())
  , pos_(state->pos())
  , code_(state->getCode())
{
  for (Transition* t : state->getAllRelatedTransitions())
  {
    if (t->hasDestination())
    {
      deleteTransitions_.append(new DeleteTransition(scene, t->origin()->name(), t->destination()->name()));
    }
  }
}

void DeleteStateCommand::redo()
{
  for (auto command : deleteTransitions_)
  {
    command->redo();
  }
  scene_->removeState(name_);
}

void DeleteStateCommand::undo()
{
  auto state = scene_->addState(name_, pos_);
  for (auto command : deleteTransitions_)
  {
    command->undo();
  }
  state->setCode(code_);
}
