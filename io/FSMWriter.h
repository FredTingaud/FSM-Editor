#pragma once

class Graph;
class QTextStream;

class FSMWriter
{
public:
  void write(Graph& scene, QTextStream& out);
};
