#pragma once

#include <fsm-editor/model/Graph.h>

#include <QString>

class QTextStream;

/**
 * @brief Generates a graph from a saved file.
 *
 * It is the counterpart of FSMWriter.
 * @see FSMWriter.
 */
class FSMReader
{
public:
  /**
   * Read from an inputStream and outputs a graph.
   * @return The loaded graph.
   * @see Graph.
   */
  virtual Graph read(QTextStream& inStream) = 0;
};
