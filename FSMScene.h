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
class QTextStream;

/**
 * @brief FSMScene displays the graph.
 *
 * It manipulates both the graphical components and the model.
 * It is bad architecture, but it is the standard one in Qt.
 */
class FSMScene : public QGraphicsScene
{
  Q_OBJECT;

  using super = QGraphicsScene;
public:
  FSMScene();

  /**
   * Set the custom name validator, passed by the settings.
   */
  void setNameValidator(std::function<QString(const QString&)> stateValidator);

  /**
   * Set the writer used by copy method to create string version of clipboard copy.
   */
  void setCopyWriter(std::function<void(Graph&, QTextStream&)> copyWriter);

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
   * Signal emitted when multiple commands are going to be pushed,
   * that are going to be squashed together.
   */
  Q_SIGNAL void openCommandGroup(const QString& title);
  /**
  * Signal emitted after multiple commands were pushed,
  * that should be squashed together.
  */
  Q_SIGNAL void closeCommandGroup();
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
   * @return An action deleting the selection.
   */
  QAction* getDeleteAction() const;

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

  /**
   * Copy selection to the clipboard both as an internal format graph and
   * as a string using the FSMWriter.
   */
  Q_SLOT void copy();

  /**
   * Copy selection to the clipboard both as an internal format graph and
   * as a string using the FSMWriter, then delete it from the scene.
   */
  Q_SLOT void cut();

  /**
   * Paste the clipboard content if it contains a graph.
   */
  Q_SLOT void paste();

  /**
   * Delete selected states and transitions.
   */
  Q_SLOT void deleteSelection();

  void deleteSelectionLists(QList<State*> &deletedStates, QList<Transition*> &deletedTransitions);

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

  /**
   * Read a graph and output it in a output stream.
   */
  void writeGraph(QDataStream& out, const Graph& graph);

  /**
   * Create a subgraph from an input stream.
   */
  void readGraph(QDataStream& in);

  /**
   * Create a transition from an input stream.
   */
  void readTransition(QDataStream& in, QMap<QString, QString> &names);

  /**
   * Replace name if it is contained in the dictionary.
   */
  QString replaceNameIfNeeded(QMap<QString, QString>& names, const QString& inputName);

  /**
   * Create a state from an input stream.
   */
  void readState(QDataStream& in, QMap<QString, QString> &names);

  /**
   * Copy the selection in a model graph.
   */
  Graph copyGraph() const;

  void copySelectedTransitions(QList<GraphTransition*> &everyTransitions) const;

  void copySelectedStates(QList<GraphState*> &everyStates) const;

  QString copyTextVersion(Graph selection);

  void fillSelectionLists(QList<State*> &deletedStates, QList<Transition*> &deletedTransitions);

private:
  static int index;
  static const QColor BACKGROUND_COLOR;
  static const QString STATE_STR;
  static const QString TRANSITION_STR;

private:
  std::map<QString, State*> states_;
  std::function<QString(const QString&)> stateValidator_;
  std::function<void(Graph&, QTextStream&)> copyWriter_;
  State* startingState_;
  FSMElement* editingElement_;
  QAction* startAct_;
  QAction* deleteAct_;
  QPointF pressPos_;
};
