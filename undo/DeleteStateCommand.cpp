#include <fsm-editor/undo/DeleteStateCommand.h>
#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/FSMScene.h>

#include <QTransform>

DeleteStateCommand::DeleteStateCommand(FSMScene* scene, const QString& name, const QPointF& position)
  : QUndoCommand(QString("Delete state %1").arg(name))
  , scene_(scene)
  , name_(name)
  , pos_(position)
{}

void DeleteStateCommand::redo()
{
  scene_->removeItem(scene_->itemAt(pos_, QTransform()));
}

void DeleteStateCommand::undo()
{
  FSMScene* copy = scene_;
  scene_->addItem(new State(name_, pos_, [=](QUndoCommand* command){copy->pushCommand(command); }));
}
