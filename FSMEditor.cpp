#include <fsm-editor/FSMEditor.h>
#include <fsm-editor/Settings.h>
#include <fsm-editor/io/FSMWriter.h>
#include <fsm-editor/io/FSMReader.h>

#include <QMenuBar>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>
#include <QSplitter>

const QString FSMEditor::LAST_DIR_KEY = "last_dir";
const QString FSMEditor::LAST_ZOOM = "last_zoom";
const QString FSMEditor::LAST_SPLITS = "last_splits";
const QString FSMEditor::LAST_STATE = "last_state";
const QString FSMEditor::LAST_GEOMETRY = "last_geometry";

FSMEditor::FSMEditor(Settings& settings)
  : settings_(settings)
  , scene_([&](const QString& name){return settings_.validateStateName(name); })
  , fsmView_(&scene_, this)
  , saveAction_(nullptr)
{
  Q_INIT_RESOURCE(fsm_editor_resources);
  makeLuaEditor();
  splitter_ = new QSplitter(Qt::Horizontal, this);
  splitter_->addWidget(makeViewPanel());
  splitter_->addWidget(editor_);
  setCentralWidget(splitter_);

  modifiedChanged(true);
  setCurrentFile("");

  connect(&scene_, SIGNAL(command(QUndoCommand*)), SLOT(stackCommand(QUndoCommand*)));
  connect(&undoStack_, SIGNAL(cleanChanged(bool)), SLOT(modifiedChanged(bool)));
  loadSettings();
}

FSMEditor::~FSMEditor()
{
  saveSettings();
  Q_CLEANUP_RESOURCE(fsm_editor_resources);
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

void FSMEditor::setMenuVisible(bool visible)
{
  menuBar()->setVisible(visible);
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
  connect(&scene_, SIGNAL(switchScrollMode(bool)), SLOT(scrollModeSwitched(bool)));
  connect(&scene_, SIGNAL(zoomed(int)), SLOT(zoomView(int)));
}

void FSMEditor::saveSettings()
{
  QSettings settings(settings_.getOrganizationName(), settings_.getApplicationName());
  settings.setValue(LAST_DIR_KEY, lastDir_);
  settings.setValue(LAST_ZOOM, fsmView_.transform().m11());
  settings.setValue(LAST_GEOMETRY, saveGeometry());
  settings.setValue(LAST_STATE, saveState());
  settings.setValue(LAST_SPLITS, splitter_->saveState());
}

void FSMEditor::loadSettings()
{
  QSettings settings(settings_.getOrganizationName(), settings_.getApplicationName());
  lastDir_ = settings.value(LAST_DIR_KEY, "").toString();
  qreal scale = settings.value(LAST_ZOOM, 1.).toDouble();
  fsmView_.scale(scale, scale);
  restoreGeometry(settings.value(LAST_GEOMETRY).toByteArray());
  restoreState(settings.value(LAST_STATE).toByteArray());
  splitter_->restoreState(settings.value(LAST_SPLITS).toByteArray());
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

void FSMEditor::scrollModeSwitched(bool scroll)
{
  if (scroll)
  {
    fsmView_.setDragMode(QGraphicsView::ScrollHandDrag);
  }
  else
  {
    fsmView_.setDragMode(QGraphicsView::NoDrag);
  }
}

void FSMEditor::zoomView(int delta)
{
  if (delta > 0)
  {
    zoomIn();
  }
  else
  {
    zoomOut();
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
  fillAllMenus(addToolBar(tr("Quick access")));

  vLayout->addWidget(&fsmView_);
  viewPanel->setLayout(vLayout);
  return viewPanel;
}

void FSMEditor::fillAllMenus(QToolBar* toolbar)
{
  QMenuBar* menu = menuBar();
  if (settings_.showFileActions())
  {
    QMenu * fileMenu = menu->addMenu(tr("&File"));
    fillMenu(toolbar, fileMenu, createFileActions());
    completeFileMenu(fileMenu);
    toolbar->addSeparator();
  }
  fillMenu(toolbar, menu->addMenu(tr("&Edition")), createUndoRedoActions());
  toolbar->addSeparator();
  fillMenu(toolbar, menu->addMenu(tr("&View")), createZoomActions());
  toolbar->addSeparator();
  fillMenu(toolbar, menu->addMenu(tr("&Graph elements")), createElementActions());
}

void FSMEditor::completeFileMenu(QMenu* fileMenu)
{
  fileMenu->addSeparator();
  QAction* closeAction = new QAction(tr("E&xit"), this);
  closeAction->setShortcut(QKeySequence::Quit);
  connect(closeAction, SIGNAL(triggered()), SLOT(close()));
  fileMenu->addAction(closeAction);
}

void FSMEditor::fillMenu(QToolBar* toolbar, QMenu* menu, const QList<std::tuple<QAction*, bool>>& actions)
{
  for (auto action : actions)
  {
    menu->addAction(std::get<0>(action));
    if (std::get<1>(action))
    {
      toolbar->addAction(std::get<0>(action));
    }
  }
}

QList<std::tuple<QAction*, bool>> FSMEditor::createZoomActions()
{
  QList<std::tuple<QAction*, bool>> result;
  QAction* zoomIn = new QAction(QIcon(":/ic_zoom_in.png"), tr("Zoom &In"), this);
  QAction* zoomOut = new QAction(QIcon(":/ic_zoom_out.png"), tr("Zoom &Out"), this);
  connect(zoomIn, SIGNAL(triggered()), SLOT(zoomIn()));
  connect(zoomOut, SIGNAL(triggered()), SLOT(zoomOut()));

  result << std::make_tuple(zoomIn, true) << std::make_tuple(zoomOut, true);
  return result;
}

QList<std::tuple<QAction*, bool>> FSMEditor::createElementActions()
{
  QList<std::tuple<QAction*, bool>> result;
  result << std::make_tuple(scene_.getStartAction(), true);
  return result;
}

QList<std::tuple<QAction*, bool>> FSMEditor::createUndoRedoActions()
{
  QList<std::tuple<QAction*, bool>> result;
  QAction* undo = undoStack_.createUndoAction(this);
  undo->setIcon(QIcon(":/ic_undo.png"));
  undo->setShortcut(QKeySequence::Undo);
  QAction* redo = undoStack_.createRedoAction(this);
  redo->setShortcut(QKeySequence::Redo);
  redo->setIcon(QIcon(":/ic_redo.png"));

  result << std::make_tuple(undo, true) << std::make_tuple(redo, true);
  return result;
}

QList<std::tuple<QAction*, bool>> FSMEditor::createFileActions()
{
  QList<std::tuple<QAction*, bool>> result;
  QAction* newAction = new QAction(QIcon(":/ic_insert_drive_file.png"), tr("New"), this);
  newAction->setShortcut(QKeySequence::New);
  saveAction_ = new QAction(QIcon(":/ic_save.png"), tr("Save"), this);
  saveAction_->setShortcut(QKeySequence::Save);
  QAction* saveAsAction = new QAction("Save &as...", this);
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  QAction* openAction = new QAction(QIcon(":/ic_folder_open.png"), tr("Open"), this);
  openAction->setShortcut(QKeySequence::Open);

  connect(newAction, SIGNAL(triggered()), SLOT(newGraph()));
  connect(saveAction_, SIGNAL(triggered()), SLOT(save()));
  connect(saveAsAction, SIGNAL(triggered()), SLOT(saveAs()));
  connect(openAction, SIGNAL(triggered()), SLOT(open()));

  result << std::make_tuple(newAction, true)
    << std::make_tuple(saveAction_, true)
    << std::make_tuple(saveAsAction, false)
    << std::make_tuple(openAction, true);
  return result;
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
