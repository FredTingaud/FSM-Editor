#pragma once

#include <QGraphicsRectItem>

class State : public QGraphicsRectItem
{
  using super = QGraphicsRectItem;
public:
  State(QString title, const QPointF& position);

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  static bool fitInRect(const QRectF& rect, const QRect& bounding);

private:
  static const qreal WIDTH;
  static const qreal HEIGHT;
  static const qreal H_MARGIN;
  static const qreal V_MARGIN;

private:
  QString title_;
  QString content_;
};
