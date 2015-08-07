#pragma once

#include <QGraphicsScene>
#include <fsm-editor/model/Graph.h>

#include <functional>

class QUndoCommand;
class State;
class Transition;
class FSMElement;
class ExportVisitor;
class QAction;

class FSMScene : public QGraphicsScene
{
  Q_OBJECT;
public:
  FSMScene(std::function<QString(const QString&)> stateValidator);

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

  void pushCommand(QUndoCommand* command);

  Q_SIGNAL void command(QUndoCommand* command);
  Q_SIGNAL void codeChanged(const QString& code);
  Q_SIGNAL void codeHidden();

  Q_SLOT void checkSelection();

  State* addState(const QString& name, const QPointF& pos);

  void setSelectedItem(QGraphicsItem* item);

  void removeState(const QString& name);
  State* getState(const QString& name) const;
  FSMElement* getElement(const QString& name) const;

  Transition* getTransition(const QString& name) const;

  QString renameState(State* state, const QString& newName);
  void setStateName(State* state, const QString& name);

  void setCode(FSMElement* element, const QString& code);
  void updateCode(const QString& code);

  void selectElement(const QString& element);

  /**
   * @return The state the graph starts with.
   */
  State* getStartState() const;

  /**
   * Set the state the graph starts with.
   */
  void setStartState(State* start);

  /**
   * Method called by undo command don't call it directly.
   */
  void changeStartState(State* start);

  /**
   * @return An action allowing to set selected state as start state.
   */
  QAction* getStartAction() const;

  Graph graph() const;
  void setNewGraph(Graph&& graph);

public:
  enum UNDO_IDS
  {
    UNDO_MOVE = 1,
    UNDO_CODE = 2
  };

private:
  /**
   * Slot method called by start action.
   */
  Q_SLOT void setSelectionAsStartState();

private:
  static int index;
  static const QColor BACKGROUND_COLOR;

private:
  std::map<QString, State*> states_;
  State* startingState_;
  FSMElement* editingElement_;
  std::function<QString(const QString&)> stateValidator_;
  QAction* startAct_;
};
