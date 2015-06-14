#include<fsm-editor/FSMScene.h>

#include<fsm-editor/fsm-elements/State.h>

#include<QGraphicsSceneMouseEvent>

int FSMScene::index = 0;

FSMScene::FSMScene()
{
  addItem(new State("Hello world", QPointF(0, 0)));
}

void FSMScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  addItem(new State(QString("State %1").arg(index++), event->scenePos()));
}
