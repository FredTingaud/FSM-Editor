#pragma once

#include <fsm-editor/model/GraphElement.h>

class GraphState;
class ExportVisitor;

/**
 * @brief Model representation of a transition between two states of a Finite State Machine.
 *
 * Each transition has associated code that describes on what condition the Machine passes from one state to the other.
 * It's equivalent on view side is Transition.
 * @see Transition
 */
class GraphTransition : public virtual GraphElement
{
public:
  /**
   * The export visitor can be used for model saving.
   */
  virtual QString visit(ExportVisitor& visitor) const = 0;

  /**
   * @return the name of the state our transition starts from.
   */
  virtual QString getOriginState() const = 0;
  /**
   * @return the name of the state our transition goes to.
   */
  virtual QString getDestinationState() const = 0;
};
