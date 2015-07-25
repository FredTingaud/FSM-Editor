#include <QApplication>

#include <fsm-editor/tests/DummyVisitor.h>
#include <fsm-editor/FSMEditor.h>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  DummyVisitor visitor;
  FSMEditor editor(visitor);
  editor.show();

  return app.exec();
}
