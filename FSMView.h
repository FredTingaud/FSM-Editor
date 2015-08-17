#pragma  once

#include <QGraphicsView>
#include <QGraphicsScene>

class FSMView : public QGraphicsView
{
  Q_OBJECT;
  using super = QGraphicsView;
public:
  FSMView(QGraphicsScene* scene, QWidget* parent);

  /**
   * Zoom in, in the graph view.
   */
  Q_SLOT void zoomIn();
  /**
   * Zoom out, in the graph view.
   */
  Q_SLOT void zoomOut();
  /**
   * Called by the wheel events from the scene.
   */
  Q_SLOT void zoomView(int delta);

  /**
   * This method should only be useful to save and load the zoom between sessions.
   */
  qreal currentZoom() const;

  /**
  * This method should only be useful to save and load the zoom between sessions.
  */
  void setZoom(qreal zoom);
protected:
  virtual void keyPressEvent(QKeyEvent *event) override;

  virtual void keyReleaseEvent(QKeyEvent *event) override;
};
