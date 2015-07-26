#pragma once

#include <QString>

class GraphElement
{
public:
  virtual QString name() const = 0;
  virtual QString getCode() const = 0;
};
