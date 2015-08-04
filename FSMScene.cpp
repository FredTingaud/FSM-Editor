#include<fsm-editor/FSMScene.h>
#include<fsm-editor/ExportVisitor.h>

#include<fsm-editor/fsm-elements/State.h>
#include <fsm-editor/undo/AddStateCommand.h>
#include <fsm-editor/undo/RenameState.h>
#include <fsm-editor/undo/UpdateCode.h>

#include<QGraphicsSceneMouseEvent>
#include <QUndoCommand>

int FSMScene::index = 0;
const QColor FSMScene::BACKGROUND_COLOR = QColor(70, 70, 70);

FSMScene::FSMScene(std::function<QString(const QString&)> stateValidator)
  : editingElement_(nullptr)
  , stateValidator_(stateValidator)
{
  setBackgroundBrush(QBrush(BACKGROUND_COLOR));
  connect(this, SIGNAL(selectionChanged()), SLOT(checkSelection()));
}

void FSMScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  QString name = QString("State%1").arg(index++);
  while (states_.count(name) > 0)
  {
    name = QString("State%1").arg(index++);
  }
  command(new AddStateCommand(this, name, event->scenePos()));
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
  if (states_.count(name))
    return states_.at(name);
  return nullptr;
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

Graph FSMScene::graph() const
{
  Graph result;
  QList<GraphState*> everyStates;
  QList<GraphTransition*> everyTransitions;
  for (auto state : states_)
  {
    everyStates.append(state.second);
    for (auto transition : state.second->getTransitions())
    {
      everyTransitions.append(transition);
    }
  }
  result.setData(std::move(everyStates), std::move(everyTransitions));
  return result;
}

void FSMScene::setNewGraph(Graph&& graph)
{
  clear();
  for (auto state : graph.getAllStates())
  {
    auto res = addState(state->name(), state->getPosition());
    res->setCode(state->getCode());
  }
  for (auto transition : graph.getAllTransitions())
  {
    auto res = getState(transition->getOriginState());
    auto dest = getState(transition->getDestinationState());
    res->transitionTo(dest, transition->getCode());
  }
}

void FSMScene::setStateName(State* state, const QString& name)
{
  states_.erase(state->name());
  state->setName(name);
  states_.insert(std::pair<QString, State*>(name, state));
}

QString FSMScene::renameState(State* state, const QString& newName)
{
  if (states_.count(newName))
  {
    return tr("There already is a state called %1 in the chart.").arg(newName);
  }
  QString settingsError = stateValidator_(newName);
  if (!settingsError.isEmpty())
    return settingsError;
  pushCommand(new RenameState(this, state->name(), newName));
  return "";
}
