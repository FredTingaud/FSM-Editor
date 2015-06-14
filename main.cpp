#include <QApplication>

#include <fsm-editor/FSMEditor.h>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  FSMEditor editor;
  editor.show();

  return app.exec();
}
