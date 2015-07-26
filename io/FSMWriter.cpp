#include <fsm-editor/io/FSMWriter.h>

#include <fsm-editor/model/Graph.h>
#include <fsm-editor/model/GraphState.h>
#include <fsm-editor/model/GraphTransition.h>
#include <fsm-editor/tests/DummyVisitor.h>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

void FSMWriter::write(Graph& scene)
{
  static DummyVisitor visitor;

  QFile file = QFileDialog::getSaveFileName();
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  for (GraphState* state : scene.getAllStates())
  {
    out << state->visit(visitor) << "\n";
  }
  for (GraphTransition* transition : scene.getAllTransitions())
  {
    out << transition->visit(visitor) << "\n";
  }
}
