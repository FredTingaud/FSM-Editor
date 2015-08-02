#pragma once

#include <fsm-editor/model/Graph.h>

#include <QString>

class QTextStream;

class FSMReader
{
public:
  virtual Graph read(QTextStream& inStream);
  GraphState* readState(const QString& name, QTextStream& inStream);
  GraphTransition* readTransition(const QString& origin, const QString& destination, QTextStream& inStream);
};
