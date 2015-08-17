#pragma once

/**
 * FSMScene displays the graph. It manipulates both the graphical components and the model.
 * It is bad architecture, but it is the standard one in Qt.
 */
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

  using super = QGraphicsScene;
public:
  FSMScene(std::function<QString(const QString&)> stateValidator);

  /**
   * Overrides double click handling, to create a state when double clicking.
   */
  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

  /**
   * Push a command in the undo/redo stack.
   */
  void pushCommand(QUndoCommand* command);

  /**
   * Signal emitted when a command was pushed.
   */
  Q_SIGNAL void command(QUndoCommand* command);
  /**
   * Signal emitted when the current code is changed.
   */
  Q_SIGNAL void codeChanged(const QString& code);
  /**
   * Signal emitted when the current code is hidden.
   */
  Q_SIGNAL void codeHidden();
  /**
   * Signal emitted when switching to scroll mode.
   */
  Q_SIGNAL void switchScrollMode(bool on);
  /**
  * Signal emitted when zooming in/out.
  */
  Q_SIGNAL void zoomed(int delta);

  /**
   * Get a state by its name.
   */
  State* getState(const QString& name) const;
  /**
   * Get any graph element by its name.
   */
  FSMElement* getElement(const QString& name) const;
  /**
   * Get a transition by its name.
   */
  Transition* getTransition(const QString& name) const;

  /**
   * Change the name of the state, creating an undo command.
   */
  QString renameState(State* state, const QString& newName);

  /**
   * Sets the current element and its code without triggering the undo/redo stack.
   * Used when changing the selection or when undo/redo is already handled otherwise.
   */
  void setCode(FSMElement* element, const QString& code);

  /**
   * Change the code of the selection, passing through the undo/redo stack.
   */
  void updateCode(const QString& code);

  /**
   * Select an element by its name.
   */
  void selectElement(const QString& element);

  /**
   * Sets the selection to this item only and give it focus.
   */
  void setSelectedItem(QGraphicsItem* item);

  /**
   * @return The state the graph starts with.
   */
  State* getStartState() const;

  /**
   * Set the state the graph starts with.
   */
  void setStartState(State* start);

  /**
   * @return An action allowing to set selected state as start state.
   */
  QAction* getStartAction() const;

  /**
   * Classes outside the QGraphicsScene should handle model objects,
   * not directly the graphics item. We want to be able to change the
   * architecture in the future.
   * @return model version of the graph.
   */
  Graph graph() const;

  /**
   * Replaces current graph with the passed one.
   */
  void setNewGraph(Graph&& graph);

public:
  /**
   * Ids used by the QUndoCommands to merge some changes.
   */
  enum UNDO_IDS
  {
    UNDO_MOVE = 1,
    UNDO_CODE = 2
  };

protected:
  virtual void wheelEvent(QGraphicsSceneWheelEvent *event) override;

  virtual void keyPressEvent(QKeyEvent *event) override;

  virtual void keyReleaseEvent(QKeyEvent *event) override;

private:
  friend class AddStateCommand;
  friend class DeleteStateCommand;
  friend class RenameState;
  friend class StartStateCommand;
  /**
  * Method called only by the undo commands, don't call it directly.
  * Creates a new state in the scene.
  */
  State* addState(const QString& name, const QPointF& pos);
  /**
  * Method called only by the undo commands, don't call it directly.
  * Remove a state by its name.
  */
  void removeState(const QString& name);

  /**
  * Method called only by the undo commands, don't call it directly.
  */
  void changeStartState(State* start);

  /**
   * Method called only by the undo commands, to modify the state name.
   */
  void setStateName(State* state, const QString& name);

  /**
   * Slot method called by start action.
   */
  Q_SLOT void setSelectionAsStartState();

  /**
   * Update internal state on selection change.
   */
  Q_SLOT void checkSelection();

  /**
   * Clear everything in the scene and memory.
   */
  void clearAll();

private:
  static int index;
  static const QColor BACKGROUND_COLOR;

private:
  std::map<QString, State*> states_;
  State* startingState_;
  FSMElement* editingElement_;
  std::function<QString(const QString&)> stateValidator_;
  QAction* startAct_;
  QPointF pressPos_;
};
