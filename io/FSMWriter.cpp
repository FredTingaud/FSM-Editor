#include <fsm-editor/io/FSMWriter.h>

#include <fsm-editor/FSMScene.h>
#include <fsm-editor/tests/DummyVisitor.h>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

void FSMWriter::write(FSMScene& scene)
{
  static DummyVisitor visitor;

  QFile file = QFileDialog::getSaveFileName();
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << scene.generateExport(visitor);
}
