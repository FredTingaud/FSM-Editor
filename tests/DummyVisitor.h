#pragma once

#include <fsm-editor/ExportVisitor.h>

class DummyVisitor : public ExportVisitor
{
public:
  virtual QString exportElement(const State& state) override;
  virtual QString exportElement(const Transition& transition) override;
};
