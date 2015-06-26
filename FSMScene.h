#pragma once

#include <QGraphicsScene>

class QUndoCommand;

class FSMScene : public QGraphicsScene
{
  Q_OBJECT;
public:
  FSMScene();

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

  void pushCommand(QUndoCommand* command);

  Q_SIGNAL void command(QUndoCommand* command);

public:
  enum UNDO_IDS
  {
    UNDO_MOVE = 1
  };

private:
  static int index;
  static const QColor BACKGROUND_COLOR;
};
