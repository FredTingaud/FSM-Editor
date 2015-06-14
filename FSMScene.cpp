#include<fsm-editor/FSMScene.h>

#include<fsm-editor/fsm-elements/State.h>

FSMScene::FSMScene()
{
  addItem(new State("Hello world"));
}
