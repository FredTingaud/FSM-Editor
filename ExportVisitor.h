#pragma once

#include <QString>

class GraphState;
class GraphTransition;

class ExportVisitor
{
public:
  virtual QString exportElement(const GraphState& state) = 0;

  virtual QString exportElement(const GraphTransition& transition) = 0;
};
