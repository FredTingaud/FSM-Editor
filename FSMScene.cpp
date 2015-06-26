#include<fsm-editor/FSMScene.h>

#include<fsm-editor/fsm-elements/State.h>
#include <fsm-editor/undo/AddStateCommand.h>

#include<QGraphicsSceneMouseEvent>
#include <QUndoCommand>

int FSMScene::index = 0;

FSMScene::FSMScene()
{}

void FSMScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  command(new AddStateCommand(this, QString("State %1").arg(index++), event->scenePos()));
}

void FSMScene::pushCommand(QUndoCommand* undoCommand)
{
  command(undoCommand);
}
