#pragma once

#include <QString>

class State;
class Transition;

class ExportVisitor
{
public:
  virtual QString exportElement(const State& state) = 0;

  virtual QString exportElement(const Transition& transition) = 0;
};
