#pragma once

#include <QUndoCommand>

class FSMScene;
class AddTransition : public QUndoCommand
{
public:
  AddTransition(FSMScene* scene, const QString& origin, const QString& destination);
  AddTransition(FSMScene* scene, const QString& origin, const QString& destination, const QString& code);

  virtual void undo() override;

  virtual void redo() override;

private:
  FSMScene* scene_;
  QString origin_;
  QString destination_;
  QString code_;
};
