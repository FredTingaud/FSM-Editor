#include<fsm-editor/FSMScene.h>

#include<fsm-editor/fsm-elements/State.h>
#include <fsm-editor/undo/AddStateCommand.h>

#include<QGraphicsSceneMouseEvent>
#include <QUndoCommand>

int FSMScene::index = 0;
const QColor FSMScene::BACKGROUND_COLOR = QColor(70, 70, 70);

FSMScene::FSMScene()
{
  setBackgroundBrush(QBrush(BACKGROUND_COLOR));
}

void FSMScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  command(new AddStateCommand(this, QString("State %1").arg(index++), event->scenePos()));
}

void FSMScene::pushCommand(QUndoCommand* undoCommand)
{
  command(undoCommand);
}
