#include <fsm-editor/FSMView.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

FSMView::FSMView(QGraphicsScene* scene, QWidget* parent)
  : super(scene, parent)
{
  setDragMode(QGraphicsView::RubberBandDrag);
}

void FSMView::keyPressEvent(QKeyEvent *event)
{
  super::keyPressEvent(event);
  if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
  {
    setScrollMode(true);
  }
}

void FSMView::keyReleaseEvent(QKeyEvent *event)
{
  super::keyReleaseEvent(event);
  if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
  {
    setScrollMode(false);
  }
}

void FSMView::zoomIn()
{
  scale(1.2, 1.2);
}

void FSMView::zoomOut()
{
  scale(0.8, 0.8);
}

qreal FSMView::currentZoom() const
{
  return transform().m11();
}

void FSMView::setZoom(qreal zoom)
{
  scale(zoom, zoom);
}

void FSMView::zoomView(int delta)
{
  if (delta > 0)
  {
    zoomIn();
  }
  else
  {
    zoomOut();
  }
}

void FSMView::drawBackground(QPainter * painter, const QRectF & rect)
{
  super::drawBackground(painter, rect);
  if (scene()->items().empty())
  {
    QRectF rect = mapToScene(visibleRegion().boundingRect()).boundingRect();
    painter->setPen(Qt::white);
    painter->drawText(rect, Qt::AlignCenter, tr("Double click to\ncreate a new state"));
  }
}

void FSMView::mousePressEvent(QMouseEvent* e)
{
  if (e->button() == Qt::MidButton)
  {
    setScrollMode(true);
    QMouseEvent* left = copyMouseEventLeft(e);
    super::mousePressEvent(left);
  }
  else
  {
    super::mousePressEvent(e);
  }
}

QMouseEvent* FSMView::copyMouseEventLeft(QMouseEvent* e)
{
  return new QMouseEvent(e->type(), e->localPos(), Qt::LeftButton, e->buttons() & Qt::LeftButton, e->modifiers());
}

void FSMView::setScrollMode(bool on)
{
  if (on)
  {
    setDragMode(QGraphicsView::ScrollHandDrag);
  }
  else
  {
    setDragMode(QGraphicsView::RubberBandDrag);
  }
}

void FSMView::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::MidButton)
  {
    setScrollMode(false);
    QMouseEvent* left = copyMouseEventLeft(e);
    super::mousePressEvent(left);
  }
  else
  {
    super::mouseReleaseEvent(e);
  }
}
