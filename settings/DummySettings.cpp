#include<fsm-editor/settings/DummySettings.h>

#include <fsm-editor/settings/dummy/DummyWriter.h>
#include <fsm-editor/settings/dummy/DummyReader.h>

#include <QCoreApplication>
#include <QRegExp>

FSMReader& DummySettings::getReader()
{
  static DummyReader reader;
  return reader;
}

FSMWriter& DummySettings::getWriter()
{
  static DummyWriter writer;
  return writer;
}

QString DummySettings::validateStateName(const QString& name)
{
  if (name.contains(QRegExp("\\s")))
    return QObject::tr("State name shouldn't contain white spaces.");
  return "";
}
