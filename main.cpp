#include <QApplication>

#include <fsm-editor/settings/DummySettings.h>
#include <fsm-editor/FSMEditor.h>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  DummySettings settings;
  FSMEditor editor(settings);
  editor.show();

  return app.exec();
}
