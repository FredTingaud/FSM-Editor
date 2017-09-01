#pragma once

#include <list>
#include <memory>

#include <fsm-editor/model/GraphState.h>
#include <fsm-editor/model/GraphTransition.h>

/**
 * @brief Model class describing the Finite State Machine.
 *
 * This class holds the GraphElements and is used to describe the FSM, for code generation and graph loading.
 */
class Graph
{
public:
  Graph()
  {}

  Graph(Graph&& other)
    : states_(std::move(other.states_))
    , transitions_(std::move(other.transitions_))
  {}

  /**
   * Set all graph elements held by the Graph.
   */
  void setData(std::list<std::unique_ptr<GraphState>>&& states, std::list<std::unique_ptr<GraphTransition>>&& transitions)
  {
    states_ = std::move(states);
    transitions_ = std::move(transitions);
  }

  /**
   * Pass a const reference to the GraphStates of the graph.
   */
  const std::list<std::unique_ptr<GraphState>>& getAllStates() const
  {
    return states_;
  }

  /**
   * Pass a const reference to the GraphTransitions of the graph.
   */
  const std::list<std::unique_ptr<GraphTransition>>& getAllTransitions() const
  {
    return transitions_;
  }

  Graph& operator=(Graph&& other)
  {
    if (this != &other)
    {
      states_ = std::move(other.states_);
      transitions_ = std::move(other.transitions_);
    }
    return *this;
  }

private:
  std::list<std::unique_ptr<GraphState>> states_;
  std::list<std::unique_ptr<GraphTransition >> transitions_;
};
