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

  QPointF getIntersection(const QRectF& rect) const;

  bool hasDestination() const;
  State* destination() const;

  void setParentOvered(bool hovered);

  void updateVisibility();

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  static const QColor LINK_COLOR;
  static const QColor LINK_BORDER;
  static const qreal LINK_SIZE;

private:
  State* origin_;
  State* destination_;
  bool hovered_;
  bool parentHovered_;
};
