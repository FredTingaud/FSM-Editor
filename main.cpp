#include <QApplication>

#include <fsm-editor/Settings.h>
#include <fsm-editor/FSMEditor.h>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  Settings settings;
  FSMEditor editor(settings);
  editor.show();

  return app.exec();
}
