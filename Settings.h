#pragma once

#include <QString>

class FSMReader;
class FSMWriter;
class QTextDocument;
class Settings
{
public:
  virtual FSMReader& getReader();
  virtual FSMWriter& getWriter();
  virtual void initializeCodeHighlighter(QTextDocument* document);
  virtual QString getExportExtension() const;
  virtual QString getOrganizationName() const;
  virtual QString getApplicationName() const;
};
