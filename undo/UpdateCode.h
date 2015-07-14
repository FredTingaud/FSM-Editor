#pragma once

#include <QUndoCommand>

class FSMScene;

class UpdateCode : public QUndoCommand
{
public:
  UpdateCode(FSMScene* scene, const QString& element, const QString& newCode);

  virtual void undo() override;

  virtual void redo() override;

  virtual int id() const override;

  virtual bool mergeWith(const QUndoCommand *other) override;
private:
  FSMScene* scene_;
  QString element_;
  QString previousCode_;
  QString nextCode_;
};
