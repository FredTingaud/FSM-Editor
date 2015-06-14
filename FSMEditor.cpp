#include <fsm-editor/FSMEditor.h>

#include <QGridLayout>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QAction>

FSMEditor::FSMEditor()
  : fsmView_(&scene_, this)
{
  addWidget(makeViewPanel());
  addWidget(makeLuaEditor());
}

void FSMEditor::zoomIn()
{
  fsmView_.scale(1.2, 1.2);
}

void FSMEditor::zoomOut()
{
  fsmView_.scale(0.8, 0.8);
}

QWidget* FSMEditor::makeLuaEditor()
{
  QPlainTextEdit* editor = new QPlainTextEdit(this);
  editor->setPlainText("function sample code");
  return editor;
}

QWidget* FSMEditor::makeViewPanel()
{
  QWidget* viewPanel = new QWidget(this);
  QLayout* vLayout = new QVBoxLayout;
  QToolBar* toolbar = new QToolBar(this);
  createSceneActions(toolbar);
  vLayout->addWidget(toolbar);
  vLayout->addWidget(&fsmView_);
  viewPanel->setLayout(vLayout);
  return viewPanel;
}

void FSMEditor::createSceneActions(QToolBar* toolbar)
{
  QAction* zoomIn = toolbar->addAction("+");
  QAction* zoomOut = toolbar->addAction("-");
  connect(zoomIn, SIGNAL(triggered()), SLOT(zoomIn()));
  connect(zoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
}
