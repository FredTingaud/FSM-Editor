#pragma once

class FSMReader;
class FSMWriter;
class QTextDocument;
class Settings
{
public:
  virtual FSMReader& getReader();
  virtual FSMWriter& getWriter();
  virtual void initializeCodeHighlighter(QTextDocument* document);
};
