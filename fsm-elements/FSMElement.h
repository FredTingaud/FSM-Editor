#pragma once

#include <fsm-editor/model/GraphElement.h>

#include <QString>

class ExportVisitor;

class FSMElement : public GraphElement
{
public:
  virtual QString getCode() const override;

  virtual void setCode(const QString& code);
private:
  QString code_;
};
