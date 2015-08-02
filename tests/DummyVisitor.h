#pragma once

#include <fsm-editor/ExportVisitor.h>

class DummyVisitor : public ExportVisitor
{
public:
  virtual QString exportElement(const GraphState& state) override;
  virtual QString exportElement(const GraphTransition& transition) override;
};
