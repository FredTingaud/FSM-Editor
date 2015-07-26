#include <fsm-editor/FSMEditor.h>
#include <fsm-editor/Settings.h>
#include <fsm-editor/io/FSMWriter.h>

#include <QGridLayout>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

FSMEditor::FSMEditor(Settings& settings)
  : fsmView_(&scene_, this)
  , settings_(settings)
{
  makeLuaEditor();
  addWidget(makeViewPanel());
  addWidget(editor_);

  connect(&scene_, SIGNAL(command(QUndoCommand*)), SLOT(stackCommand(QUndoCommand*)));
}

void FSMEditor::zoomIn()
{
  fsmView_.scale(1.2, 1.2);
}

void FSMEditor::zoomOut()
{
  fsmView_.scale(0.8, 0.8);
}

void FSMEditor::stackCommand(QUndoCommand* command)
{
  undoStack_.push(command);
}

void FSMEditor::makeLuaEditor()
{
  editor_ = new QPlainTextEdit(this);
  editor_->setPlainText("function sample code");
  connect(&scene_, SIGNAL(codeChanged(const QString&)), SLOT(displaySetCode(const QString&)));
  connect(&scene_, SIGNAL(codeHidden()), SLOT(hideCode()));
  connect(editor_, SIGNAL(textChanged()), SLOT(transferCodeChanged()));
}

void FSMEditor::hideCode()
{
  editor_->clear();
  editor_->setEnabled(false);
}

void FSMEditor::displaySetCode(const QString& code)
{
  editor_->setEnabled(true);
  if (editor_->toPlainText() != code)
  {
    editor_->blockSignals(true);
    editor_->setPlainText(code);
    editor_->blockSignals(false);
  }
}

void FSMEditor::transferCodeChanged()
{
  scene_.updateCode(editor_->toPlainText());
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
  toolbar->addSeparator();
  QAction* undo = undoStack_.createUndoAction(toolbar);
  undo->setShortcut(QKeySequence::Undo);
  toolbar->addAction(undo);
  QAction* redo = undoStack_.createRedoAction(toolbar);
  redo->setShortcut(QKeySequence::Redo);
  toolbar->addAction(redo);
  toolbar->addSeparator();
  QAction* exportAction = toolbar->addAction("Export");
  exportAction->setShortcut(QKeySequence::Save);
  connect(zoomIn, SIGNAL(triggered()), SLOT(zoomIn()));
  connect(zoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
  connect(exportAction, SIGNAL(triggered()), SLOT(save()));
}

void FSMEditor::save()
{
  settings_.getWriter().write(scene_.graph());
}
