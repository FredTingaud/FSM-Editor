#include <fsm-editor/tests/DummyVisitor.h>

#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/fsm-elements/Transition.h>

QString DummyVisitor::exportElement(const State& state)
{
  return "new State {\n" + state.getCode() + "\n}";
}

QString DummyVisitor::exportElement(const Transition& transition)
{
  return "new Transition from " + transition.origin()->name() + " to " + transition.destination()->name() + "{\n" + transition.getCode() + "\n}";
}
