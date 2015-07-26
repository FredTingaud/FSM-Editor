#pragma once

#include <fsm-editor/model/GraphElement.h>

class ExportVisitor;

class GraphState : public GraphElement
{
public:
  virtual QString visit(ExportVisitor& visitor) const = 0;
};