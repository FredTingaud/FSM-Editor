#pragma once
#include <QString>

class FSMReader;
class FSMWriter;
class QTextDocument;

/**
 * @brief Override to customize the editor.
 *
 * This class passes the code generator and other parameters that make sense for
 * developer embedding the editor.
 * Default values are provided everywhere it makes sense.
 */
class Settings
{
public:
  /**
   * @return The writer used to save the FSM chart.
   */
  virtual FSMWriter& getWriter() = 0;

  /**
   * @return The reader used to load files written by the FSMWriter.
   */
  virtual FSMReader& getReader() = 0;

  /**
   * We use a QTextEdit to edit code. It can display code with syntax highlighting.
   */
  virtual void initializeCodeHighlighter(QTextDocument* document);

  /**
   * @return the file extension used for chart files. @Default is "fsm".
   */
  virtual QString getExportExtension() const;

  /**
   * @return the organization name used to store data in registry between sessions.
   */
  virtual QString getOrganizationName() const;

  /**
   * @return the application name used to store data in registry between sessions.
   */
  virtual QString getApplicationName() const;

  /**
   * Validates whether the new state name is correctly formed for export.
   * @return the error message if any. Empty string for no error.
   * @Default accepts everything.
   */
  virtual QString validateStateName(const QString& name);

  /**
  * Validates whether the code of an element is correctly formed for export.
  * @return the error message if any. Empty string for no error.
  * @Default validates whether the number of opening and closing curly braces are equal.
  */
  virtual QString validateCode(const QString& valid);

  /**
   * @return whether the top level menu is visible.
   * It could make sense to hide it when the editor is embedded.
   */
  virtual bool showMenu();

  /**
   * @return Whether the "New", "Open" and "Save" actions are accessible.
   * When the editor is embedded, it can be convenient to hide them and drive
   * open/save at a higher level.
   */
  virtual bool showFileActions();
};
