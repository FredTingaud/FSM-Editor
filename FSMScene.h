#include <QGraphicsScene>

class FSMScene : public QGraphicsScene
{
public:
  FSMScene();

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
  static int index;
};
