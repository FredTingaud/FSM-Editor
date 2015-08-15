#include <fsm-editor/FSMEditor.h>
#include <fsm-editor/Settings.h>
#include <fsm-editor/io/FSMWriter.h>
#include <fsm-editor/io/FSMReader.h>

#include <QGridLayout>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>

const QString FSMEditor::LAST_DIR_KEY = "last_dir";
const QString FSMEditor::LAST_ZOOM = "last_zoom";
const QString FSMEditor::LAST_SPLITS = "last_splits";
const QString FSMEditor::LAST_GEOMETRY = "last_geometry";

FSMEditor::FSMEditor(Settings& settings)
  : settings_(settings)
  , scene_([&](const QString& name){return settings_.validateStateName(name); })
  , fsmView_(&scene_, this)
  , saveAction_(nullptr)
{
  Q_INIT_RESOURCE(resources);
  makeLuaEditor();
  addWidget(makeViewPanel());
  addWidget(editor_);

  modifiedChanged(true);
  setCurrentFile("");

  connect(&scene_, SIGNAL(command(QUndoCommand*)), SLOT(stackCommand(QUndoCommand*)));
  connect(&undoStack_, SIGNAL(cleanChanged(bool)), SLOT(modifiedChanged(bool)));
  loadSettings();
}

FSMEditor::~FSMEditor()
{
  saveSettings();
  Q_CLEANUP_RESOURCE(resources);
}

void FSMEditor::closeEvent(QCloseEvent *event)
{
  if (maybeSave())
  {
    event->accept();
  }
  else
  {
    event->ignore();
  }
}

bool FSMEditor::maybeSave()
{
  if (isWindowModified())
  {
    QMessageBox::StandardButton res = QMessageBox::warning(this, tr("Graph was modified"), tr("The current graph was modified since last save.\nDo you want to save changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
    if (res == QMessageBox::Cancel)
      return false;
    if (res == QMessageBox::Save)
      return save();
  }
  return true;
}

void FSMEditor::zoomIn()
{
  fsmView_.scale(1.2, 1.2);
}

void FSMEditor::zoomOut()
{
  fsmView_.scale(0.8, 0.8);
}

bool FSMEditor::newGraph()
{
  if (maybeSave())
  {
    setCurrentFile("");
    scene_.setNewGraph(Graph());
    return true;
  }
  return false;
}

void FSMEditor::stackCommand(QUndoCommand* command)
{
  undoStack_.push(command);
}

void FSMEditor::makeLuaEditor()
{
  editor_ = new QPlainTextEdit(this);
  editor_->setPlainText("function sample code");
  settings_.initializeCodeHighlighter(editor_->document());
  connect(&scene_, SIGNAL(codeChanged(const QString&)), SLOT(displaySetCode(const QString&)));
  connect(&scene_, SIGNAL(codeHidden()), SLOT(hideCode()));
  connect(editor_, SIGNAL(textChanged()), SLOT(transferCodeChanged()));
}

void FSMEditor::saveSettings()
{
  QSettings settings(settings_.getOrganizationName(), settings_.getApplicationName());
  settings.setValue(LAST_DIR_KEY, lastDir_);
  settings.setValue(LAST_ZOOM, fsmView_.transform().m11());
  settings.setValue(LAST_GEOMETRY, saveGeometry());
  settings.setValue(LAST_SPLITS, saveState());
}

void FSMEditor::loadSettings()
{
  QSettings settings(settings_.getOrganizationName(), settings_.getApplicationName());
  lastDir_ = settings.value(LAST_DIR_KEY, "").toString();
  qreal scale = settings.value(LAST_ZOOM, 1.).toDouble();
  fsmView_.scale(scale, scale);
  restoreGeometry(settings.value(LAST_GEOMETRY).toByteArray());
  restoreState(settings.value(LAST_SPLITS).toByteArray());
}

void FSMEditor::hideCode()
{
  editor_->clear();
  editor_->setEnabled(false);
}

void FSMEditor::modifiedChanged(bool undoClean)
{
  setWindowModified(!undoClean);
  if (saveAction_)
  {
    saveAction_->setEnabled(!undoClean);
  }
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
  fillToolbar(toolbar);
  vLayout->addWidget(toolbar);
  vLayout->addWidget(&fsmView_);
  viewPanel->setLayout(vLayout);
  return viewPanel;
}

void FSMEditor::fillToolbar(QToolBar* toolbar)
{
  createZoomActions(toolbar);
  createElementActions(toolbar);
  createUndoRedoActions(toolbar);
  createFileActions(toolbar);
}

void FSMEditor::createZoomActions(QToolBar* toolbar)
{
  QAction* zoomIn = toolbar->addAction(QIcon(":/ic_zoom_in.png"), tr("Zoom In"));
  QAction* zoomOut = toolbar->addAction(QIcon(":/ic_zoom_out.png"), tr("Zoom Out"));
  connect(zoomIn, SIGNAL(triggered()), SLOT(zoomIn()));
  connect(zoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
}

void FSMEditor::createElementActions(QToolBar* toolbar)
{
  toolbar->addSeparator();
  toolbar->addAction(scene_.getStartAction());
}

void FSMEditor::createUndoRedoActions(QToolBar* toolbar)
{
  toolbar->addSeparator();
  QAction* undo = undoStack_.createUndoAction(toolbar);
  undo->setShortcut(QKeySequence::Undo);
  toolbar->addAction(undo);
  QAction* redo = undoStack_.createRedoAction(toolbar);
  redo->setShortcut(QKeySequence::Redo);
  toolbar->addAction(redo);
}

void FSMEditor::createFileActions(QToolBar* toolbar)
{
  if (settings_.showFileActions())
  {
    toolbar->addSeparator();
    QAction* newAction = toolbar->addAction(tr("New"));
    newAction->setShortcut(QKeySequence::New);
    saveAction_ = toolbar->addAction(tr("Save"));
    saveAction_->setShortcut(QKeySequence::Save);
    QAction* openAction = toolbar->addAction(tr("Open"));
    openAction->setShortcut(QKeySequence::Open);

    connect(newAction, SIGNAL(triggered()), SLOT(newGraph()));
    connect(saveAction_, SIGNAL(triggered()), SLOT(save()));
    connect(openAction, SIGNAL(triggered()), SLOT(open()));
  }
}

bool FSMEditor::save()
{
  if (currentFile_.isEmpty())
  {
    return saveAs();
  }
  else
  {
    return save(currentFile_);
  }
}

bool FSMEditor::saveAs()
{
  QString fileName = QFileDialog::getSaveFileName(0, tr("Save Finite State Machine"), lastDir_, "*." + settings_.getExportExtension());
  if (fileName.isNull())
    return false;
  if (!fileName.endsWith(settings_.getExportExtension()))
  {
    fileName += "." + settings_.getExportExtension();
  }
  setCurrentFile(fileName);
  return save(fileName);
}

bool FSMEditor::save(const QString& fileName)
{
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;

  QTextStream out(&file);

  settings_.getWriter().write(scene_.graph(), out);
  undoStack_.setClean();
  return true;
}

void FSMEditor::setCurrentFile(QString fileName)
{
  if (fileName.isEmpty())
  {
    setWindowTitle(tr("FSM Editor[*]"));
  }
  else
  {
    lastDir_ = QFileInfo(fileName).absolutePath();
    setWindowTitle(tr("FSM Editor - %1[*]").arg(fileName));
  }
  currentFile_ = fileName;
}

bool FSMEditor::open()
{
  QString fileName = QFileDialog::getOpenFileName(0, tr("Open Finite State Machine"), lastDir_, "*." + settings_.getExportExtension());
  if (fileName.isNull())
    return false;
  return open(fileName);
}

bool FSMEditor::open(const QString& fileName)
{
  if (maybeSave())
  {
    setCurrentFile(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return false;

    QTextStream in(&file);

    scene_.setNewGraph(settings_.getReader().read(in));
    undoStack_.clear();
    return true;
  }
  return false;
}
