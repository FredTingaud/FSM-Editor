#pragma once

#include <fsm-editor/fsm-elements/FSMElement.h>
#include <fsm-editor/model/GraphTransition.h>

#include<QGraphicsItem>

class State;
class FSMScene;

/**
 * @brief View representation of a transition between two states of a Finite State Machine.
 *
 * Each transition has associated code that describes on what condition the Machine passes from one state to the other.
 * It's equivalent on model side is GraphTransition.
 *
 * @see GraphTransition
 */
class Transition : public QGraphicsItem, public FSMElement, public GraphTransition
{
  using super = QGraphicsItem;
public:
  Transition(State* origin);

  Transition(State* origin, State* destination);

  /**
   * Reposition the transition next, after a state move.
   */
  void updatePos();

  /**
   * @return whether the transition is dangling or connected
   * either to a State or to the mouse during a move.
   */
  bool isDangling() const;

  /**
   * @return the position pointed by the arrow, while dragging.
   */
  QPointF destinationPoint() const;

  /**
   * @return Whether this transition has a destination state.
   */
  bool hasDestination() const;

  /**
   * @return the origin state.
   */
  State* origin() const;

  /**
   * @return the destination state. nullptr if none.
   */
  State* destination() const;

  /**
   * Sets whether the mouse is above its parent state.
   * In dangling mode, this will decide whether the arrow is visible.
   */
  void setParentHovered(bool hovered);

  virtual QRectF boundingRect() const override;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  virtual void keyPressEvent(QKeyEvent *event) override;

  virtual QString getCode() const override;
  virtual void setCode(const QString& code) override;
  virtual QString name() const override;

  virtual QString visit(ExportVisitor& visitor) const override;

  virtual QPainterPath shape() const override;
private:
  void initialize();

  /**
   * Calculate the shape of the transition.
   */
  QList<QPolygonF> calculateShape() const;

  QList<QPolygonF> calculatePluggedArrow(QPainterPath& result, const QPolygonF& triangle) const;
  QList<QPolygonF> calculateDangling(QPainterPath& result, const QPolygonF& triangle) const;
  QList<QPolygonF> calculateMovingArrow(QPainterPath result, const QPolygonF& triangle) const;

  void addBentLine(const QPointF& originPoint, const QLineF& line, const QPointF& destPoint, QList<QPolygonF>& polys) const;

  QPointF getIntersection(const QRectF& rect) const;

  void updateVisibility();

  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

  FSMScene* fsmScene() const;

  virtual QString getOriginState() const override;

  virtual QString getDestinationState() const override;

private:
  static const qreal LINK_SIZE;
  static const qreal ARC;
  static const QColor LINK_COLOR;
  static const QColor LINK_BORDER;

private:
  State* origin_;
  State* destination_;
  QPointF movingPos_;
  bool hovered_;
  bool parentHovered_;
};
