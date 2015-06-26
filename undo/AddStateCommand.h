#pragma once

#include <QUndoCommand>
#include <QPointF>

class FSMScene;
class AddStateCommand : public QUndoCommand
{
public:
  AddStateCommand(FSMScene* scene, const QString& name, const QPointF& position);

  virtual void undo() override;

  virtual void redo() override;

private:
  FSMScene* scene_;
  QString name_;
  QPointF pos_;
};