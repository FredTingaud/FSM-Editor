#pragma once
#include <fsm-editor/Settings.h>

class DummySettings : public Settings
{
public:
  FSMReader& getReader();
  FSMWriter& getWriter();
  QString validateStateName(const QString& name);
};
