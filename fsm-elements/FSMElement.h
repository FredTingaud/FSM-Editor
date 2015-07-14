#pragma once

#include <QString>

class FSMElement
{
public:
  QString getCode() const;

  virtual void setCode(const QString& code);

  virtual QString name() const = 0;
private:
  QString code_;
};
