#pragma once

#include <fsm-editor/model/GraphElement.h>

#include <QPointF>

class ExportVisitor;

class GraphState : public GraphElement
{
public:
  virtual QString visit(ExportVisitor& visitor) const = 0;

  virtual QPointF getPosition() const = 0;

  virtual bool isStart() const = 0;
};
