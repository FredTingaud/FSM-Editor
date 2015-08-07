#pragma once

#include <QUndoCommand>
#include <QString>

class FSMScene;
class State;

class StartStateCommand : public QUndoCommand
{
public:
  StartStateCommand(FSMScene* scene, State* state);

  virtual void undo() override;

  virtual void redo() override;

private:
  FSMScene* scene_;
  QString previousStart_;
  QString nextStart_;
};
