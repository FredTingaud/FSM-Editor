#include<fsm-editor/Settings.h>

#include <fsm-editor/io/FSMWriter.h>
#include <fsm-editor/io/FSMReader.h>

#include <QCoreApplication>

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

QString Settings::getExportExtension() const
{
  return "fsm";
}

QString Settings::getOrganizationName() const
{
  QString res = QCoreApplication::organizationName();
  if (res.isEmpty())
  {
    res = "FSM Editor Corp.";
  }
  return res;
}

QString Settings::getApplicationName() const
{
  QString res = QCoreApplication::applicationName();
  if (res.isEmpty())
  {
    res = "FSM Editor";
  }
  return res;
}
