#pragma once

#include <QUndoCommand>

class FSMScene;

class RenameState : public QUndoCommand
{
public:
  RenameState(FSMScene* scene, const QString& oldName, const QString& newName);

  virtual void undo() override;

  virtual void redo() override;

private:
  FSMScene* scene_;
  QString oldName_;
  QString newName_;
};
