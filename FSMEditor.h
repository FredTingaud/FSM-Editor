#pragma once

#include <QSplitter>

#include <fsm-editor/FSMScene.h>

#include <QGraphicsView>

class QToolBar;

class FSMEditor : public QSplitter
{
  Q_OBJECT;
public:
  FSMEditor();

  Q_SLOT void zoomIn();
  Q_SLOT void zoomOut();

private:
  QWidget* makeViewPanel();

  void createSceneActions(QToolBar* toolbar);

  QWidget* makeLuaEditor();

private:
  FSMScene scene_;
  QGraphicsView fsmView_;
};
