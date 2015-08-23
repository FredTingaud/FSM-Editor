#include <fsm-editor/settings/dummy/DummyWriter.h>

#include <fsm-editor/model/Graph.h>
#include <fsm-editor/model/GraphState.h>
#include <fsm-editor/model/GraphTransition.h>
#include <fsm-editor/settings/dummy/DummyVisitor.h>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

void DummyWriter::write(Graph&& scene, QTextStream& out)
{
  static DummyVisitor visitor;

  for (auto&& state : scene.getAllStates())
  {
    out << state->visit(visitor) << "\n";
  }
  for (auto&& transition : scene.getAllTransitions())
  {
    out << transition->visit(visitor) << "\n";
  }
}
