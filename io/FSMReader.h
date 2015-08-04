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
  virtual Graph read(QTextStream& inStream) = 0;
};
