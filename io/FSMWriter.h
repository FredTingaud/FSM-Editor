#pragma once

class Graph;
class QTextStream;

/**
 * @brief Generates code from the edited graph.
 *
 * It is the counterpart of FSMReader.
 * @see FSMReader.
 */
class FSMWriter
{
public:
  /**
   * Writes into given output stream the passed graph.
   */
  virtual void write(Graph&& scene, QTextStream& out) = 0;
};
