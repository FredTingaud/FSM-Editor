#include<fsm-editor/FSMScene.h>
#include<fsm-editor/ExportVisitor.h>

#include<fsm-editor/fsm-elements/State.h>
#include<fsm-editor/model/GraphStateImpl.h>
#include<fsm-editor/model/GraphTransitionImpl.h>
#include <fsm-editor/undo/AddStateCommand.h>
#include <fsm-editor/undo/AddTransition.h>
#include <fsm-editor/undo/DeleteStateCommand.h>
#include <fsm-editor/undo/DeleteTransition.h>
#include <fsm-editor/undo/RenameState.h>
#include <fsm-editor/undo/StartStateCommand.h>
#include <fsm-editor/undo/UpdateCode.h>

#include <QGraphicsSceneMouseEvent>
#include <QUndoCommand>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>

int FSMScene::index = 0;
const QColor FSMScene::BACKGROUND_COLOR = QColor(70, 70, 70);
const QString FSMScene::STATE_STR = "state";
const QString FSMScene::TRANSITION_STR = "transition";

FSMScene::FSMScene()
  : editingElement_(nullptr)
  , startingState_(nullptr)
  , startAct_(new QAction(QIcon(State::START_ICON), tr("Start"), this))
  , deleteAct_(new QAction(QIcon(":/ic_delete.png"), tr("Delete"), this))
  , renameAct_(new QAction(QIcon(":/ic_label.png"), tr("Rename"), this))
{
  startAct_->setEnabled(false);
  deleteAct_->setShortcut(QKeySequence::Delete);
  deleteAct_->setEnabled(false);
  renameAct_->setShortcut(QKeySequence("F2"));
  renameAct_->setEnabled(false);
  setBackgroundBrush(QBrush(BACKGROUND_COLOR));
  connect(this, SIGNAL(selectionChanged()), SLOT(checkSelection()));
  connect(startAct_, SIGNAL(triggered()), SLOT(setSelectionAsStartState()));
  connect(deleteAct_, SIGNAL(triggered()), SLOT(deleteSelection()));
  connect(renameAct_, SIGNAL(triggered()), SLOT(askRenameSelection()));
}

void FSMScene::setNameValidator(std::function<QString(const QString&)> stateValidator)
{
  stateValidator_ = stateValidator;
}

void FSMScene::setCodeValidator(std::function<QString(const QString&)> codeValidator)
{
  codeValidator_ = codeValidator;
}

void FSMScene::setCopyWriter(std::function<void(Graph&&, QTextStream&)> copyWriter)
{
  copyWriter_ = copyWriter;
}

void FSMScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  if (items(event->scenePos()).isEmpty())
  {
    QString name = QString("State%1").arg(index++);
    while (states_.count(name) > 0)
    {
      name = QString("State%1").arg(index++);
    }
    command(new AddStateCommand(this, name, event->scenePos()));
  }
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
    startAct_->setEnabled(false);
    deleteAct_->setEnabled(false);
    Q_EMIT codeHidden();
  }
  else
  {
    State* state = dynamic_cast<State*>(selectedItems().at(0));
    startAct_->setEnabled(state != nullptr);
    renameAct_->setEnabled(state != nullptr);
    deleteAct_->setEnabled(true);
  }
}

State* FSMScene::addState(const QString& name, const QPointF& pos)
{
  State* item = new State(name, pos, [&](QUndoCommand* command){this->pushCommand(command); });
  states_.insert(std::pair<QString, State*>(name, item));
  addItem(item);
  setSelectedItem(item);
  if (states_.size() == 1)
  {
    changeStartState(item);
  }
  return item;
}

void FSMScene::setSelectedItem(QGraphicsItem* item)
{
  clearSelection();
  item->setSelected(true);
  setFocusItem(item);
}

void FSMScene::removeState(const QString& name)
{
  State* item = states_.at(name);
  states_.erase(states_.find(name));
  removeItem(item);
  delete item;
  if (item == startingState_)
  {
    startingState_ = nullptr;
  }
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
    auto element = it.second->getTransitionByName(name);
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
  QString errorMessage = codeValidator_(code);
  if (errorMessage.isEmpty())
  {
    editingElement_->clearError();
  }
  else
  {
    editingElement_->setInError(errorMessage);
  }
  Q_EMIT codeChanged(code, errorMessage);
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
    setSelectedItem(item);
  }
}

State* FSMScene::getStartState() const
{
  return startingState_;
}

void FSMScene::setStartState(State* state)
{
  pushCommand(new StartStateCommand(this, state));
}

void FSMScene::changeStartState(State* start)
{
  if (startingState_)
  {
    startingState_->setStart(false);
  }
  startingState_ = start;
  if (startingState_)
  {
    startingState_->setStart(true);
  }
}

Graph FSMScene::graph() const
{
  Graph result;
  std::list<std::unique_ptr<GraphState>> everyStates;
  std::list<std::unique_ptr<GraphTransition>> everyTransitions;
  for (auto state : states_)
  {
    everyStates.push_back(state.second->modelObject());
    for (auto transition : state.second->getTransitions())
    {
      everyTransitions.push_back(transition->modelObject());
    }
  }
  result.setData(std::move(everyStates), std::move(everyTransitions));
  return result;
}

Graph FSMScene::copyGraph() const
{
  Graph result;
  std::list<std::unique_ptr<GraphState>> everyStates;
  std::list<std::unique_ptr<GraphTransition>> everyTransitions;
  copySelectedStates(everyStates);
  copySelectedTransitions(everyTransitions);
  result.setData(std::move(everyStates), std::move(everyTransitions));
  return result;
}

void FSMScene::copySelectedTransitions(std::list<std::unique_ptr<GraphTransition>>& everyTransitions) const
{
  for (auto state : states_)
  {
    for (auto transition : state.second->getTransitions())
    {
      if (transition->isSelected())
      {
        everyTransitions.push_back(transition->modelObject());
      }
    }
  }
}

void FSMScene::copySelectedStates(std::list<std::unique_ptr<GraphState>>& everyStates) const
{
  for (auto state : states_)
  {
    if (state.second->isSelected())
    {
      everyStates.push_back(state.second->modelObject(-selectionArea().boundingRect().topLeft()));
    }
  }
}

void FSMScene::clearAll()
{
  states_.clear();
  clear();
  startingState_ = nullptr;
  startAct_->setEnabled(false);
}

void FSMScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    Q_EMIT zoomed(event->delta());
  }
  else
  {
    super::wheelEvent(event);
  }
}

void FSMScene::setNewGraph(Graph&& graph)
{
  clearAll();
  for (auto&& state : graph.getAllStates())
  {
    auto res = addState(state->name(), state->getPosition());
    res->setCode(state->getCode());
    if (state->isStart())
    {
      setStartState(res);
    }
  }
  for (auto&& transition : graph.getAllTransitions())
  {
    auto res = getState(transition->getOriginState());
    auto dest = getState(transition->getDestinationState());
    res->transitionTo(dest, transition->getCode());
  }
}

void FSMScene::copy()
{
  QClipboard* clipboard = QApplication::clipboard();
  QByteArray byteArray;
  QDataStream out(&byteArray, QIODevice::WriteOnly);
  Graph selection = copyGraph();
  writeGraph(out, selection);
  QMimeData* data = new QMimeData();
  data->setData("fsm/graph", byteArray);
  data->setText(copyTextVersion(std::move(selection)));
  clipboard->setMimeData(data);
}

void FSMScene::cut()
{
  copy();
  deleteSelection();
}

QString FSMScene::copyTextVersion(Graph&& selection)
{
  QString textVersion;
  QTextStream out(&textVersion, QIODevice::WriteOnly);
  copyWriter_(std::move(selection), out);
  return textVersion;
}

void FSMScene::deleteSelection()
{
  bool started = false;
  QList<State*> deletedStates;
  QList<Transition*> deletedTransitions;
  fillSelectionLists(deletedStates, deletedTransitions);
  deleteSelectionLists(deletedStates, deletedTransitions);
}

Q_SLOT void FSMScene::askRenameSelection()
{
  State* state = dynamic_cast<State*>(selectedItems().first());
  if (state)
  {
    askRename(state);
  }
}

void FSMScene::askRename(State* state)
{
  bool ok;
  QString newName = QInputDialog::getText(0, QObject::tr("Rename state %1").arg(state->name()), QObject::tr("Rename to:"), QLineEdit::Normal, state->name(), &ok);
  if (ok  && newName != state->name())
  {
    QString error = renameState(state, newName);
    if (!error.isEmpty())
    {
      QMessageBox::warning(0, QObject::tr("Couldn't rename to %1").arg(newName)
                           , QObject::tr("The state couldn't be renamed to %1 because of the following error:\n%2").arg(newName).arg(error));
    }
  }
}

void FSMScene::fillSelectionLists(QList<State*> &deletedStates, QList<Transition*> &deletedTransitions)
{
  for (QGraphicsItem* item : selectedItems())
  {
    State* state = dynamic_cast<State*>(item);
    if (state)
    {
      deletedStates.push_back(state);
    }
    else
    {
      Transition* transition = dynamic_cast<Transition*>(item);
      if (transition)
      {
        deletedTransitions.push_back(transition);
      }
    }
  }
}

void FSMScene::deleteSelectionLists(QList<State*> &deletedStates, QList<Transition*> &deletedTransitions)
{
  if (!deletedStates.isEmpty() || !deletedTransitions.isEmpty())
  {
    Q_EMIT openCommandGroup(tr("Delete selection"));
    for (Transition* transition : deletedTransitions)
    {
      pushCommand(new DeleteTransition(this, transition->getOriginState(), transition->getDestinationState()));
    }
    for (State* state : deletedStates)
    {
      pushCommand(new DeleteStateCommand(this, state));
    }
    Q_EMIT closeCommandGroup();
  }
}

FSMElement* FSMScene::getErrorElement() const
{
  for (auto statePair : states_)
  {
    State* state = statePair.second;
    if (state->isInError())
      return state;
    for (Transition* transition : state->getTransitions())
    {
      if (transition->isInError())
      {
        return transition;
      }
    }
  }
  return nullptr;
}

void FSMScene::writeGraph(QDataStream& out, const Graph& graph)
{
  QString type;
  for (auto&& g : graph.getAllStates())
  {
    type = STATE_STR;
    out << type << g->name() << g->getCode() << g->getPosition() << g->isStart();
  }
  for (auto&& t : graph.getAllTransitions())
  {
    type = TRANSITION_STR;
    out << type << t->getCode() << t->getOriginState() << t->getDestinationState();
  }
}

void FSMScene::paste()
{
  QClipboard* clipboard = QApplication::clipboard();
  auto data = clipboard->mimeData();
  if (data->hasFormat("fsm/graph"))
  {
    QByteArray byteArray = data->data("fsm/graph");
    Q_EMIT(openCommandGroup(tr("Paste clipboard")));
    QDataStream in(&byteArray, QIODevice::ReadOnly);
    readGraph(in);
    Q_EMIT(closeCommandGroup());
  }
}

void FSMScene::readGraph(QDataStream& in)
{
  QMap<QString, QString> names;
  while (!in.atEnd())
  {
    QString type;
    in >> type;
    if (type == STATE_STR)
    {
      readState(in, names);
    }
    else if (type == TRANSITION_STR)
    {
      readTransition(in, names);
    }
    else
    {
      qFatal(qPrintable("unknown type " + type));
    }
  }
}

void FSMScene::readState(QDataStream& in, QMap<QString, QString> &names)
{
  QString name;
  QString code;
  QPointF position;
  bool isStart;
  in >> name >> code >> position >> isStart;
  QString nextName = name;
  while (states_.count(nextName))
  {
    nextName = "copy_" + nextName;
  }
  names[name] = nextName;
  pushCommand(new AddStateCommand(this, nextName, position));
  pushCommand(new UpdateCode(this, nextName, code));
}

void FSMScene::readTransition(QDataStream& in, QMap<QString, QString> &names)
{
  QString code;
  QString destination;
  QString origin;
  in >> code >> origin >> destination;

  origin = replaceNameIfNeeded(names, origin);
  destination = replaceNameIfNeeded(names, destination);

  pushCommand(new AddTransition(this, origin, destination, code));
}

QString FSMScene::replaceNameIfNeeded(QMap<QString, QString>& names, const QString& inputName)
{
  if (names.contains(inputName))
  {
    return names[inputName];
  }
  return inputName;
}

void FSMScene::setSelectionAsStartState()
{
  State* state = dynamic_cast<State*>(selectedItems().at(0));
  setStartState(state);
}

QAction* FSMScene::getStartAction() const
{
  return startAct_;
}

QAction* FSMScene::getDeleteAction() const
{
  return deleteAct_;
}

QAction* FSMScene::getRenameAction() const
{
  return renameAct_;
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
