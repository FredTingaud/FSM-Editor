#pragma once

/**
 * Model class holding the GraphElements.
 */
#include <QList>

class GraphState;
class GraphTransition;

class Graph
{
public:
  /**
   * Set all graph elements held by the Graph.
   */
  void setData(QList<GraphState*>&& states, QList<GraphTransition*>&& transitions)
  {
    states_ = std::move(states);
    transitions_ = std::move(transitions);
  }

  /**
   * Pass a const reference to the GraphStates of the graph.
   */
  const QList<GraphState*>& getAllStates() const
  {
    return states_;
  }

  /**
   * Pass a const reference to the GraphTransitions of the graph.
   */
  const QList<GraphTransition*>& getAllTransitions() const
  {
    return transitions_;
  }

private:
  QList<GraphState*> states_;
  QList<GraphTransition*> transitions_;
};
