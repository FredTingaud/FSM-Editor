#pragma once

#include <QUndoCommand>

class FSMScene;
class DeleteTransition : public QUndoCommand
{
public:
  DeleteTransition(FSMScene* scene, const QString& origin, const QString& destination);

  virtual void undo() override;

  virtual void redo() override;

private:
  FSMScene* scene_;
  QString origin_;
  QString destination_;
  QString code_;
};
