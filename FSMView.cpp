#include <fsm-editor/FSMView.h>

#include <QKeyEvent>

FSMView::FSMView(QGraphicsScene* scene, QWidget* parent)
  : super(scene, parent)
{}

void FSMView::keyPressEvent(QKeyEvent *event)
{
  super::keyPressEvent(event);
  if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
  {
    setDragMode(QGraphicsView::ScrollHandDrag);
  }
}

void FSMView::keyReleaseEvent(QKeyEvent *event)
{
  super::keyReleaseEvent(event);
  if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
  {
    setDragMode(QGraphicsView::NoDrag);
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
