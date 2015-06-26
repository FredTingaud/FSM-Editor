#pragma once

#include <QUndoCommand>
#include <QPointF>

class QGraphicsScene;
class State;

class MoveStateCommand : public QUndoCommand
{
public:
  MoveStateCommand(QGraphicsScene* scene, const QString& name, const QPointF& position, State* state);

  virtual void undo() override;

  virtual void redo() override;

  virtual int id() const override;

  virtual bool mergeWith(const QUndoCommand *other) override;

  State* findState(const QPointF& position);

private:
  bool first_;
  QGraphicsScene* scene_;
  QString name_;
  QPointF previousPos_;
  QPointF newPos_;
};
