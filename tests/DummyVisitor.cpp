#include <fsm-editor/tests/DummyVisitor.h>

#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/fsm-elements/Transition.h>

QString DummyVisitor::exportElement(const GraphState& state)
{
  return QString("!! State %1 {\n%2\n} - %3:%4").arg(state.name()).arg(state.getCode()).arg(state.getPosition().x()).arg(state.getPosition().y());
}

QString DummyVisitor::exportElement(const GraphTransition& transition)
{
  return QString("!! Transition from %1 to %2 {\n%3\n}").arg(transition.getOriginState()).arg(transition.getDestinationState()).arg(transition.getCode());
}
