#include<fsm-editor/Settings.h>

#include <fsm-editor/io/FSMWriter.h>
#include <fsm-editor/io/FSMReader.h>

FSMReader& Settings::getReader()
{
  static FSMReader reader;
  return reader;
}

FSMWriter& Settings::getWriter()
{
  static FSMWriter writer;
  return writer;
}

void Settings::initializeCodeHighlighter(QTextDocument* document)
{}
