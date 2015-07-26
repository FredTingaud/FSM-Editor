#include<fsm-editor/FSMScene.h>
#include<fsm-editor/ExportVisitor.h>

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
  connect(this, SIGNAL(selectionChanged()), SLOT(checkSelection()));
}

void FSMScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  command(new AddStateCommand(this, QString("State %1").arg(index++), event->scenePos()));
}

void FSMScene::pushCommand(QUndoCommand* undoCommand)
{
  command(undoCommand);
}

void FSMScene::checkSelection()
{
  if (selectedItems().isEmpty())
  {
    editingElement_ = nullptr;
    Q_EMIT codeHidden();
  }
}

State* FSMScene::addState(const QString& name, const QPointF& pos)
{
  State* item = new State(name, pos, [&](QUndoCommand* command){this->pushCommand(command); });
  states_.insert(std::pair<QString, State*>(name, item));
  addItem(item);
  clearSelection();
  item->setSelected(true);
  return item;
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

FSMElement* FSMScene::getElement(const QString& name) const
{
  State* state = getState(name);
  if (state == nullptr)
  {
    return getTransition(name);
  }
  return state;
}

Transition* FSMScene::getTransition(const QString& name) const
{
  for (auto&& it : states_)
  {
    auto element = it.second->getElement(name);
    if (element != nullptr)
    {
      return element;
    }
  }
  return nullptr;
}

void FSMScene::setCode(FSMElement* element, const QString& code)
{
  editingElement_ = element;
  Q_EMIT codeChanged(code);
}

void FSMScene::updateCode(const QString& code)
{
  if (editingElement_ != nullptr)
  {
    pushCommand(new UpdateCode(this, editingElement_->name(), code));
  }
}

QString FSMScene::generateExport(ExportVisitor& visitor)
{
  QString result;
  QList<Transition*> everyTransitions;
  for (auto state : states_)
  {
    result += visitor.exportElement(*state.second) + "\n";
    everyTransitions.append(state.second->getTransitions());
  }
  for (auto transition : everyTransitions)
  {
    result += visitor.exportElement(*transition) + "\n";
  }
  return result;
}

void FSMScene::selectElement(const QString& element)
{
  QGraphicsItem* item = getState(element);
  if (item == nullptr)
  {
    item = getTransition(element);
  }
  if (selectedItems().count() != 1 || selectedItems()[0] != item)
  {
    clearSelection();
    item->setSelected(true);
  }
}
