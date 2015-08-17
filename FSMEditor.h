#pragma once

#include <QMainWindow>

#include <fsm-editor/FSMScene.h>
#include <fsm-editor/FSMView.h>

#include <QUndoStack>

class QToolBar;
class QUndoCommand;
class QPlainTextEdit;
class Settings;
class QMenuBar;
class QMenu;
class QSplitter;

/**
 * @brief Main class of the FSM Editor project.
 *
 * Include this QWidget in your application to use it.
 * The FSMEditor takes a Settings object in its constructor, that allows to customize the
 * editor, depending on the use-case.
 */
class FSMEditor : public QMainWindow
{
  Q_OBJECT;
public:
  FSMEditor(Settings& settings);
  virtual ~FSMEditor();

  /**
   * Set whether the top level menu is visible.
   * This parameter is initially set from Settings object, but can be overridden here.
   */
  void setMenuVisible(bool visible);

  /**
   * Open a new empty graph.
   * @return Whether the new graph was actually opened.
   */
  Q_SLOT bool newGraph();

  /**
   * Call a file dialog and save in the selected file.
   * @return Whether the file was actually saved.
   */
  Q_SLOT bool saveAs();

  /**
   * Save in the current file if any, call "save as" otherwise.
   * @return Whether the file was actually saved.
   */
  Q_SLOT bool save();

  /**
   * Save in passed file.
   * @return Whether the file was actually saved.
   */
  Q_SLOT bool save(const QString& fileName);

  /**
   * Call a file dialog and open the selected file.
   * @return Whether the file was actually opened.
   */
  Q_SLOT bool open();

  /**
  * Open the passed file.
  * @return Whether the file was actually opened.
  */
  Q_SLOT bool open(const QString& fileName);

protected:
  virtual void closeEvent(QCloseEvent *event) override;

private:
  Q_SLOT void stackCommand(QUndoCommand* command);
  Q_SLOT void transferCodeChanged();
  Q_SLOT void displaySetCode(const QString& code);
  Q_SLOT void hideCode();
  Q_SLOT void modifiedChanged(bool undoClean);

  void setCurrentFile(QString fileName);

private:
  QWidget* makeViewPanel();
  void makeLuaEditor();

  void fillAllMenus(QToolBar* toolbar);

  void completeFileMenu(QMenu* fileMenu);

  void fillMenu(QToolBar* toolbar, QMenu* menu, const QList<std::tuple<QAction*, bool>>& actions);
  QList<std::tuple<QAction*, bool>> createElementActions();
  QList<std::tuple<QAction*, bool>> createUndoRedoActions();
  QList<std::tuple<QAction*, bool>> createZoomActions();
  QList<std::tuple<QAction*, bool>> createFileActions();

  void loadSettings();
  void saveSettings();

  /**
   * Make sure everything is saved or discarded on purpose, before whipping the scene.
   */
  bool maybeSave();

private:
  static const QString LAST_DIR_KEY;
  static const QString LAST_ZOOM;
  static const QString LAST_SPLITS;
  static const QString LAST_STATE;
  static const QString LAST_GEOMETRY;

private:
  Settings& settings_;
  FSMScene scene_;
  FSMView fsmView_;
  QSplitter* splitter_;

  QUndoStack undoStack_;
  QPlainTextEdit* editor_;
  QString lastDir_;
  QString currentFile_;
  QAction* saveAction_;
  QMenuBar* menu_;
};
