#pragma once

#include <QUndoCommand>
#include <QPointF>
#include <QList>

class FSMScene;
class State;
class DeleteTransition;
class DeleteStateCommand :public QUndoCommand
{
public:
  DeleteStateCommand(FSMScene* scene, State* state);

  virtual void undo() override;

  virtual void redo() override;

private:
  FSMScene* scene_;
  QString name_;
  QPointF pos_;
  QString code_;
  QList<DeleteTransition*> deleteTransitions_;
};
