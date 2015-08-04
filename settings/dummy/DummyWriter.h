#pragma once

#include <fsm-editor/io/FSMWriter.h>

class Graph;
class QTextStream;

class DummyWriter : public FSMWriter
{
public:
  /**
   * Writes into given output stream the passed graph.
   */
  virtual void write(Graph& scene, QTextStream& out) override;
};
