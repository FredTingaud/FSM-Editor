#pragma once

class Graph;
class QTextStream;

class FSMWriter
{
public:
  /**
   * Writes into given output stream the passed graph.
   */
  virtual void write(Graph& scene, QTextStream& out) = 0;
};
