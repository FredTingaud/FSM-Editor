#include <fsm-editor/settings/dummy/DummyVisitor.h>

#include <fsm-editor/fsm-elements/State.h>
#include <fsm-editor/fsm-elements/Transition.h>

QString DummyVisitor::exportElement(const GraphState& state)
{
  QString starting = state.isStart() ? "Starting " : "";
  return QString("!! %5State %1 {\n%2\n} - %3:%4").arg(state.name()).arg(state.getCode()).arg(state.getPosition().x()).arg(state.getPosition().y()).arg(starting);
}

QString DummyVisitor::exportElement(const GraphTransition& transition)
{
  return QString("!! Transition from %1 to %2 {\n%3\n}").arg(transition.getOriginState()).arg(transition.getDestinationState()).arg(transition.getCode());
}
