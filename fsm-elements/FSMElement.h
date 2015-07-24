#pragma once

#include <QString>

class ExportVisitor;

class FSMElement
{
public:
  QString getCode() const;

  virtual void setCode(const QString& code);

  virtual QString name() const = 0;

  virtual QString visit(ExportVisitor& visitor) const = 0;

private:
  QString code_;
};
