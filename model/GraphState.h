#pragma once

#include <fsm-editor/model/GraphElement.h>

#include <QPointF>

class ExportVisitor;

/**
 * @brief Model representation of a State of the Finite State Machine.
 *
 * Each state has associated code describing the behavior it represents.
 * It's equivalent on view side is State.
 *
 * @see State
 */
class GraphState : public GraphElement
{
public:
  /**
   * The export visitor can be used for model saving.
   */
  virtual QString visit(ExportVisitor& visitor) const = 0;

  /**
   * @return the state position in the graph. It is not a purely model method,
   * but it is simpler to make it persistent between sessions.
   */
  virtual QPointF getPosition() const = 0;

  /**
   * @return whether it is the start state. There is one start state for the graph
   * that is the state the finite state machine is in at initialization.
   */
  virtual bool isStart() const = 0;
};
