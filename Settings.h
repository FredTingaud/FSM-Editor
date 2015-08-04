#pragma once

#include <QString>

class FSMReader;
class FSMWriter;
class QTextDocument;
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
};
