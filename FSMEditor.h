#pragma once

#include <QSplitter>

#include <fsm-editor/FSMScene.h>

#include <QGraphicsView>
#include <QUndoStack>

class QToolBar;
class QUndoCommand;
class QPlainTextEdit;
class Settings;
class FSMEditor : public QSplitter
{
  Q_OBJECT;
public:
  FSMEditor(Settings& settings);

  Q_SLOT void zoomIn();
  Q_SLOT void zoomOut();

private:
  Q_SLOT void stackCommand(QUndoCommand* command);
  Q_SLOT void transferCodeChanged();
  Q_SLOT void displaySetCode(const QString& code);
  Q_SLOT void hideCode();
  Q_SLOT void save();

private:
  QWidget* makeViewPanel();

  void createSceneActions(QToolBar* toolbar);

  void makeLuaEditor();

private:
  FSMScene scene_;
  QGraphicsView fsmView_;

  QUndoStack undoStack_;
  QPlainTextEdit* editor_;
  Settings& settings_;
};
