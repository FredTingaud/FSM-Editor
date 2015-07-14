#pragma once

#include <QSplitter>

#include <fsm-editor/FSMScene.h>

#include <QGraphicsView>
#include <QUndoStack>

class QToolBar;
class QUndoCommand;
class QPlainTextEdit;

class FSMEditor : public QSplitter
{
  Q_OBJECT;
public:
  FSMEditor();

  Q_SLOT void zoomIn();
  Q_SLOT void zoomOut();

private:
  Q_SLOT void stackCommand(QUndoCommand* command);
  Q_SLOT void transferCodeChanged();
  Q_SLOT void displaySetCode(const QString& code);

private:
  QWidget* makeViewPanel();

  void createSceneActions(QToolBar* toolbar);

  void makeLuaEditor();

private:
  FSMScene scene_;
  QGraphicsView fsmView_;

  QUndoStack undoStack_;
  QPlainTextEdit* editor_;
};
