#pragma once

#include <fsm-editor/model/GraphState.h>

#include <QString>
#include <QPointF>

/**
 * @brief Minimalistic implementation of the GraphState.
 */
class GraphStateImpl : public GraphState
{
public:
  GraphStateImpl(const QString& name, bool start = false)
    : name_(name)
    , start_(start)
  {}

  virtual QString visit(ExportVisitor& visitor) const override
  {
    throw std::exception("The method or operation is not implemented.");
  }

  void setPosition(const QPointF pos)
  {
    pos_ = pos;
  }

  virtual QPointF getPosition() const override
  {
    return pos_;
  }

  virtual QString name() const override
  {
    return name_;
  }

  void setCode(const QString code)
  {
    code_ = code;
  }

  virtual QString getCode() const override
  {
    return code_;
  }

  virtual bool isStart() const override
  {
    return start_;
  }

private:
  QString name_;
  bool start_;
  QPointF pos_;
  QString code_;
};
