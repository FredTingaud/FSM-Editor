#pragma once

class Graph;
class QTextStream;

class FSMWriter
{
public:
  /**
   * Writes into given output stream the passed graph.
   */
  void write(Graph& scene, QTextStream& out);
};
