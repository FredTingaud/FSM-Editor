#pragma once

#include <QList>

class GraphState;
class GraphTransition;

class Graph
{
public:
  void setData(QList<GraphState*>&& states, QList<GraphTransition*>&& transitions)
  {
    states_ = std::move(states);
    transitions_ = std::move(transitions);
  }

  const QList<GraphState*>& getAllStates() const
  {
    return states_;
  }

  const QList<GraphTransition*>& getAllTransitions() const
  {
    return transitions_;
  }

private:
  QList<GraphState*> states_;
  QList<GraphTransition*> transitions_;
};
