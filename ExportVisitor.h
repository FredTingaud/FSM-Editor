#pragma once
#include <QString>

class GraphState;
class GraphTransition;

/**
 * @brief Utility class, to simplify code generation.
 *
 * ExportVisitor can visit both states and transitions.
 * Override it to use Visitor pattern in your code generation.
 */
class ExportVisitor
{
public:
  virtual QString exportElement(const GraphState& state) = 0;

  virtual QString exportElement(const GraphTransition& transition) = 0;
};
