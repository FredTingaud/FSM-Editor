#pragma once

/**
 * Model representation of a transition between two states of a Finite State Machine.
 */
#include <fsm-editor/model/GraphElement.h>

class GraphState;
class ExportVisitor;

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
