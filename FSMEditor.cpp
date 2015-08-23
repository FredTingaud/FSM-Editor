#include <fsm-editor/FSMEditor.h>
#include <fsm-editor/Settings.h>
#include <fsm-editor/io/FSMWriter.h>
#include <fsm-editor/io/FSMReader.h>
#include <fsm-editor/fsm-elements/FSMElement.h>

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
#include <QLabel>

const QString FSMEditor::LAST_DIR_KEY = "last_dir";
const QString FSMEditor::LAST_ZOOM = "last_zoom";
const QString FSMEditor::LAST_SPLITS = "last_splits";
const QString FSMEditor::LAST_STATE = "last_state";
const QString FSMEditor::LAST_GEOMETRY = "last_geometry";

FSMEditor::FSMEditor(Settings& settings)
  : settings_(settings)
  , fsmView_(&scene_, this)
  , saveAction_(nullptr)
{
  Q_INIT_RESOURCE(fsm_editor_resources);

  scene_.setNameValidator([&](const QString& name){return settings_.validateStateName(name); });
  scene_.setCopyWriter([&](Graph& g, QTextStream& stream){return settings_.getWriter().write(g, stream); });
  scene_.setCodeValidator([&](const QString& code) {return settings_.validateCode(code); });
  splitter_ = new QSplitter(Qt::Horizontal, this);
  splitter_->addWidget(makeViewPanel());
  splitter_->addWidget(makeCodeEditor());
  setCentralWidget(splitter_);

  modifiedChanged(true);
  setCurrentFile("");

  connect(&scene_, SIGNAL(command(QUndoCommand*)), SLOT(stackCommand(QUndoCommand*)));
  connect(&scene_, SIGNAL(openCommandGroup(const QString&)), SLOT(beginMacro(const QString&)));
  connect(&scene_, SIGNAL(closeCommandGroup()), SLOT(endMacro()));
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
    QMessageBox::StandardButton res = QMessageBox::warning(this, tr("Graph was modified"),
                                                           tr("The current graph was modified since last save.\nDo you want to save changes?"),
                                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
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

void FSMEditor::beginMacro(const QString& title)
{
  undoStack_.beginMacro(title);
}

void FSMEditor::endMacro()
{
  undoStack_.endMacro();
}

QWidget* FSMEditor::makeCodeEditor()
{
  QWidget* result = new QWidget(this);
  QLayout* layout = new QVBoxLayout();
  result->setLayout(layout);
  configureErrorLabel();
  layout->addWidget(errorLabel_);
  configureCodeEditor();
  layout->addWidget(editor_);
  connect(&scene_, SIGNAL(codeChanged(const QString&, const QString&)), SLOT(displaySetCode(const QString&, const QString&)));
  connect(&scene_, SIGNAL(codeHidden()), SLOT(hideCode()));
  connect(editor_, SIGNAL(textChanged()), SLOT(transferCodeChanged()));
  connect(&scene_, SIGNAL(zoomed(int)), &fsmView_, SLOT(zoomView(int)));
  return result;
}

void FSMEditor::configureCodeEditor()
{
  editor_ = new QPlainTextEdit(this);
  editor_->setPlainText("function sample code");
  settings_.initializeCodeHighlighter(editor_->document());
}

void FSMEditor::configureErrorLabel()
{
  errorLabel_ = new QLabel("");
  QFont f = errorLabel_->font();
  f.setBold(true);
  errorLabel_->setStyleSheet("color: red");
  errorLabel_->setFont(f);
}

void FSMEditor::saveSettings()
{
  QSettings settings(settings_.getOrganizationName(), settings_.getApplicationName());
  settings.setValue(LAST_DIR_KEY, lastDir_);
  settings.setValue(LAST_ZOOM, fsmView_.currentZoom());
  settings.setValue(LAST_GEOMETRY, saveGeometry());
  settings.setValue(LAST_STATE, saveState());
  settings.setValue(LAST_SPLITS, splitter_->saveState());
}

void FSMEditor::loadSettings()
{
  QSettings settings(settings_.getOrganizationName(), settings_.getApplicationName());
  lastDir_ = settings.value(LAST_DIR_KEY, "").toString();
  qreal scale = settings.value(LAST_ZOOM, 1.).toDouble();
  fsmView_.setZoom(scale);
  restoreGeometry(settings.value(LAST_GEOMETRY).toByteArray());
  restoreState(settings.value(LAST_STATE).toByteArray());
  splitter_->restoreState(settings.value(LAST_SPLITS).toByteArray());
}

void FSMEditor::hideCode()
{
  editor_->clear();
  editor_->setEnabled(false);
  errorLabel_->clear();
}

void FSMEditor::modifiedChanged(bool undoClean)
{
  setWindowModified(!undoClean);
  if (saveAction_)
  {
    saveAction_->setEnabled(!undoClean);
  }
}

void FSMEditor::displaySetCode(const QString& code, const QString& errorMessage)
{
  editor_->setEnabled(true);
  if (editor_->toPlainText() != code)
  {
    editor_->blockSignals(true);
    editor_->setPlainText(code);
    editor_->blockSignals(false);
  }
  errorLabel_->setText(errorMessage);
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
  QMenu * editionMenu = menu->addMenu(tr("&Edition"));
  fillMenu(toolbar, editionMenu, createUndoRedoActions());
  toolbar->addSeparator();
  fillMenu(toolbar, editionMenu, createCopyPasteActions());
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
  zoomIn->setShortcut(QKeySequence::ZoomIn);
  QAction* zoomOut = new QAction(QIcon(":/ic_zoom_out.png"), tr("Zoom &Out"), this);
  zoomOut->setShortcut(QKeySequence::ZoomOut);
  connect(zoomIn, SIGNAL(triggered()), &fsmView_, SLOT(zoomIn()));
  connect(zoomOut, SIGNAL(triggered()), &fsmView_, SLOT(zoomOut()));

  result << std::make_tuple(zoomOut, true) << std::make_tuple(zoomIn, true);
  return result;
}

QList<std::tuple<QAction*, bool>> FSMEditor::createElementActions()
{
  QList<std::tuple<QAction*, bool>> result;
  result << std::make_tuple(scene_.getStartAction(), true) << std::make_tuple(scene_.getDeleteAction(), true) << std::make_tuple(scene_.getRenameAction(), true);
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

QList<std::tuple<QAction*, bool>> FSMEditor::createCopyPasteActions()
{
  QList<std::tuple<QAction*, bool>> result;
  QAction* cutAct = new QAction(QIcon(":/ic_content_cut.png"), tr("Cut"), this);
  cutAct->setShortcut(QKeySequence::Cut);
  QAction* copyAct = new QAction(QIcon(":/ic_content_copy.png"), tr("Copy"), this);
  copyAct->setShortcut(QKeySequence::Copy);
  QAction* pasteAct = new QAction(QIcon(":/ic_content_paste.png"), tr("Paste"), this);
  pasteAct->setShortcut(QKeySequence::Paste);
  connect(cutAct, SIGNAL(triggered()), &scene_, SLOT(cut()));
  connect(copyAct, SIGNAL(triggered()), &scene_, SLOT(copy()));
  connect(pasteAct, SIGNAL(triggered()), &scene_, SLOT(paste()));
  result << std::make_tuple(cutAct, true) << std::make_tuple(copyAct, true) << std::make_tuple(pasteAct, true);
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

bool FSMEditor::checkValidity()
{
  FSMElement* errorElement = scene_.getErrorElement();
  if (errorElement != nullptr)
  {
    QMessageBox::critical(0, tr("Invalid graph element"), tr("The following graph element has invalid code: %1.\nMessage is:\n%2").arg(errorElement->name()).arg(errorElement->getErrorMessage()));
    return false;
  }
  return true;
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
  if (!checkValidity())
    return false;
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
