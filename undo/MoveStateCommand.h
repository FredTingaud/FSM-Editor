#pragma once

#include <QUndoCommand>
#include <QPointF>

class FSMScene;
class State;

class MoveStateCommand : public QUndoCommand
{
public:
  MoveStateCommand(FSMScene* scene, const QString& name, const QPointF& position, State* state);

  virtual void undo() override;

  virtual void redo() override;

  virtual int id() const override;

  virtual bool mergeWith(const QUndoCommand *other) override;

private:
  bool first_;
  FSMScene* scene_;
  QString name_;
  QPointF previousPos_;
  QPointF newPos_;
};
