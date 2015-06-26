#pragma once

#include <functional>

#include <QGraphicsRectItem>
#include <QUndoCommand>

class State : public QGraphicsRectItem
{
  using super = QGraphicsRectItem;
public:
  State(QString title, const QPointF& position, std::function<void(QUndoCommand*)>&& pushStack);

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  static bool fitInRect(const QRectF& rect, const QRect& bounding);

  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

  void setSilentMove(bool silent);

  QString title() const;

private:
  static const qreal WIDTH;
  static const qreal HEIGHT;
  static const qreal H_MARGIN;
  static const qreal V_MARGIN;

private:
  QString title_;
  QString content_;
  std::function<void(QUndoCommand*)> pushStack_;
  bool silent_;
};
