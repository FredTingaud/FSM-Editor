#pragma once

#include <QUndoCommand>
#include <QPointF>

class FSMScene;

class DeleteStateCommand :public QUndoCommand
{
public:
  DeleteStateCommand(FSMScene* scene, const QString& name, const QPointF& position);

  virtual void undo() override;

  virtual void redo() override;

private:
  FSMScene* scene_;
  QString name_;
  QPointF pos_;
};
