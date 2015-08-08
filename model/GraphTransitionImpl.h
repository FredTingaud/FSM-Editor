#pragma once

/**
 * Minimalistic implementation of the GraphTransition.
 */
#include <fsm-editor/model/GraphTransition.h>

class GraphTransitionImpl : public GraphTransition
{
public:
  GraphTransitionImpl(const QString& origin, const QString& destination)
    : origin_(origin)
    , destination_(destination)
  {}

  virtual QString visit(ExportVisitor& visitor) const override
  {
    throw std::exception("The method or operation is not implemented.");
  }

  virtual QString getOriginState() const override
  {
    return origin_;
  }

  virtual QString getDestinationState() const override
  {
    return destination_;
  }

  virtual QString name() const override
  {
    return QString("Transition from %1 to %2").arg(origin_).arg(destination_);
  }

  void setCode(const QString& code)
  {
    code_ = code;
  }

  virtual QString getCode() const override
  {
    return code_;
  }

private:
  QString origin_;
  QString destination_;
  QString code_;
};
