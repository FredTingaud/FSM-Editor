#include<fsm-editor/FSMScene.h>

#include<fsm-editor/fsm-elements/State.h>
#include <fsm-editor/undo/AddStateCommand.h>
#include <fsm-editor/undo/UpdateCode.h>

#include<QGraphicsSceneMouseEvent>
#include <QUndoCommand>

int FSMScene::index = 0;
const QColor FSMScene::BACKGROUND_COLOR = QColor(70, 70, 70);

FSMScene::FSMScene()
  : editingElement_(nullptr)
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

void FSMScene::addState(const QString& name, const QPointF& pos)
{
  State* item = new State(name, pos, [&](QUndoCommand* command){this->pushCommand(command); });
  states_.insert(std::pair<QString, State*>(name, item));
  addItem(item);
}

void FSMScene::removeState(const QString& name)
{
  State* item = states_.at(name);
  states_.erase(states_.find(name));
  removeItem(item);
  delete item;
}

State* FSMScene::getState(const QString& name) const
{
  return states_.at(name);
}

void FSMScene::setCode(FSMElement* element, const QString& code)
{
  editingElement_ = element;
  Q_EMIT codeChanged(code);
}

void FSMScene::updateCode(const QString& code)
{
  pushCommand(new UpdateCode(this, editingElement_->name(), code));
}
