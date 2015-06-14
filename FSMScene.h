#pragma once

#include <QGraphicsScene>

class QUndoCommand;

class FSMScene : public QGraphicsScene
{
  Q_OBJECT;
public:
  FSMScene();

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

  Q_SIGNAL void command(QUndoCommand* command);

private:
  static int index;
};
