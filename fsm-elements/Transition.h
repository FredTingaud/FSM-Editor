#pragma once

#include<QGraphicsItem>

class State;

class Transition : public QGraphicsItem
{
  using super = QGraphicsItem;
public:
  Transition(State* origin);

  void initPos();

  bool isDangling() const;

  virtual QRectF boundingRect() const override;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  QList<QPolygonF> calculateShape() const;

  QPointF destinationPoint() const;

  bool hasDestination() const;
  State* destination() const;

  void setParentOvered(bool hovered);

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:

  QList<QPolygonF> calculatePluggedArrow(QPainterPath& result, const QPolygonF& triangle) const;
  QList<QPolygonF> calculateDangling(QPainterPath& result, const QPolygonF& triangle) const;
  QList<QPolygonF> calculateMovingArrow(QPainterPath result, const QPolygonF& triangle) const;

  void addBentLine(const QPointF& originPoint, const QLineF& line, const QPointF& destPoint, QList<QPolygonF>& polys) const;

  QPointF getIntersection(const QRectF& rect) const;

  void updateVisibility();

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
