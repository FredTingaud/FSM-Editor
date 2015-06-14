#include <fsm-editor/undo/AddStateCommand.h>

#include <fsm-editor/fsm-elements/State.h>

#include <QGraphicsScene>
#include <QTransform>

AddStateCommand::AddStateCommand(QGraphicsScene* scene, const QString& name, const QPointF& position)
  : QUndoCommand(QString("Create state %1").arg(name))
  , scene_(scene)
  , name_(name)
  , pos_(position)
{}

void AddStateCommand::undo()
{
  scene_->removeItem(scene_->itemAt(pos_, QTransform()));
}

void AddStateCommand::redo()
{
  scene_->addItem(new State(name_, pos_));
}
