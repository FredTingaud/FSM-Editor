#pragma once

#include <QGraphicsScene>

class QUndoCommand;
class State;

class FSMScene : public QGraphicsScene
{
  Q_OBJECT;
public:
  FSMScene();

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

  void pushCommand(QUndoCommand* command);

  Q_SIGNAL void command(QUndoCommand* command);

  void addState(const QString& name, const QPointF& pos);
  void removeState(const QString& name);
  State* getState(const QString& name) const;
public:
  enum UNDO_IDS
  {
    UNDO_MOVE = 1
  };

private:
  static int index;
  static const QColor BACKGROUND_COLOR;

private:
  std::map<QString, State*> states_;
};
