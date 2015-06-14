#pragma once

#include <QSplitter>

#include <fsm-editor/FSMScene.h>

#include <QGraphicsView>
#include <QUndoStack>

class QToolBar;
class QUndoCommand;

class FSMEditor : public QSplitter
{
  Q_OBJECT;
public:
  FSMEditor();

  Q_SLOT void zoomIn();
  Q_SLOT void zoomOut();

  Q_SLOT void stackCommand(QUndoCommand* command);
private:
  QWidget* makeViewPanel();

  void createSceneActions(QToolBar* toolbar);

  QWidget* makeLuaEditor();

private:
  FSMScene scene_;
  QGraphicsView fsmView_;

  QUndoStack undoStack_;
};
