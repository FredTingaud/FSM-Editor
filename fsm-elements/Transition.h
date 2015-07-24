#pragma once

#include <fsm-editor/fsm-elements/FSMElement.h>

#include<QGraphicsItem>

class State;
class FSMScene;
class Transition : public QGraphicsItem, public FSMElement
{
  using super = QGraphicsItem;
public:
  Transition(State* origin);

  Transition(State* origin, State* destination);

  void initialize();

  void initPos();

  bool isDangling() const;

  virtual QRectF boundingRect() const override;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  QList<QPolygonF> calculateShape() const;

  QPointF destinationPoint() const;

  bool hasDestination() const;

  State* origin() const;
  State* destination() const;

  void setParentOvered(bool hovered);

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

  virtual void keyPressEvent(QKeyEvent *event) override;

  void setCode(const QString& code);
  virtual QString name() const override;

  virtual QString visit(ExportVisitor& visitor) const override;

private:

  QList<QPolygonF> calculatePluggedArrow(QPainterPath& result, const QPolygonF& triangle) const;
  QList<QPolygonF> calculateDangling(QPainterPath& result, const QPolygonF& triangle) const;
  QList<QPolygonF> calculateMovingArrow(QPainterPath result, const QPolygonF& triangle) const;

  void addBentLine(const QPointF& originPoint, const QLineF& line, const QPointF& destPoint, QList<QPolygonF>& polys) const;

  QPointF getIntersection(const QRectF& rect) const;

  void updateVisibility();

  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

  FSMScene* fsmScene() const;

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
