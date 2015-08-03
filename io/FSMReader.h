#pragma once

#include <fsm-editor/model/Graph.h>

#include <QString>

class QTextStream;

class FSMReader
{
public:
  /**
   * Read from an inputStream and outputs a graph.
   * @return The loaded graph.
   */
  virtual Graph read(QTextStream& inStream);

private:
  GraphState* readState(const QString& name, QTextStream& inStream);
  GraphTransition* readTransition(const QString& origin, const QString& destination, QTextStream& inStream);
};
