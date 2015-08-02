#pragma once

#include <fsm-editor/model/GraphElement.h>

class GraphState;
class ExportVisitor;

class GraphTransition : public virtual GraphElement
{
public:
  virtual QString visit(ExportVisitor& visitor) const = 0;

  virtual QString getOriginState() const = 0;
  virtual QString getDestinationState() const = 0;
};
