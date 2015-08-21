#pragma once

#include <fsm-editor/fsm-elements/FSMElement.h>
#include <fsm-editor/model/GraphState.h>
#include <QGraphicsRectItem>

#include <fsm-editor/fsm-elements/Transition.h>

#include <functional>

#include <QUndoCommand>

class FSMScene;

/**
 * @brief View representation of a State of the Finite State Machine.
 *
 * Each state has associated code describing the behavior it represents.
 * It's equivalent on model side is GraphState.
 *
 * @see GraphState
 */
class State : public QGraphicsRectItem, public FSMElement, public GraphState
{
  using super = QGraphicsRectItem;
public:
  State(const QString& title, const QPointF& position, std::function<void(QUndoCommand*)>&& pushStack);

  virtual ~State();

  void reactToPositionChange();

  virtual QString name() const override;

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

  /**
   * Push a command to the undo stack.
   */
  void pushCommand(QUndoCommand* command);

  /**
   * This method is called by the undo stack and shouldn't be called directly.
   * Create a transition to another state.
   */
  void transitionTo(State* destination, const QString& code = "");
  /**
  * This method is called by the undo stack and shouldn't be called directly.
  * Remove a transition to another state.
  */
  void removeTransitionTo(State* destination);

  /**
   * @return The transition from this state to @param destinationState. nullptr if it
   * doesn't exist.
   */
  Transition* getTransitionTo(State* destinationState) const;

  virtual QString getCode() const override;
  virtual void setCode(const QString& code) override;

  /**
   * @return The transition with passed name. nullptr if it is not present.
   */
  Transition* getTransitionByName(const QString& name) const;

  virtual QString visit(ExportVisitor& visitor) const override;

  /**
   * @return All transitions having this state as either source or destination.
   */
  QList<Transition*> getAllRelatedTransitions() const;

  /**
  * @return All transitions having this state as source.
  */
  QList<Transition*> getTransitions() const;

  /**
   * This method is called by the undo stack and shouldn't be called directly.
   * Change state position in the scene.
   */
  void silentlySetPosition(const QPointF& position);
  /**
   * @return The State position in the scene.
   */
  virtual QPointF getPosition() const override;

  /**
   * This method is called by the undo stack and shouldn't be called directly.
   * Change the name and update the text drawn on it.
   */
  void setName(const QString& name);

  /**
   * Sets whether this is a start state.
   */
  void setStart(bool start);

  /**
   * @return whether this is a start date.
   */
  virtual bool isStart() const override;

private:
  /**
   * Make sure the name is fully displayed in the state rectangle.
   */
  void shrinkTextToFit(QPainter * painter);

  /**
   * @return whether rect is completely inside bounding.
   */
  static bool fitInRect(const QRectF& rect, const QRect& bounding);

  /**
   * Redraw transitions after moving.
   */
  void updateTransitionsPositions(QList<Transition*>& transitions);

  /**
   * Add or remove a pointed-by transition.
   */
  void setPointedBy(Transition* transition, bool pointed);

  /**
   * @return cast scene.
   */
  FSMScene* scene() const;

public:
  static const QString START_ICON;

private:
  static const qreal WIDTH;
  static const qreal HEIGHT;
  static const qreal H_MARGIN;
  static const qreal V_MARGIN;
  static const QColor PEN_COLOR;
  static const QColor START_PEN_COLOR;

private:
  QString title_;
  std::function<void(QUndoCommand*)> pushStack_;
  bool silent_;
  Transition dangling_;
  QList<Transition*> transitions_;
  QList<Transition*> pointingTransitions_;
  bool start_;
  QRectF originalRect_;
};
