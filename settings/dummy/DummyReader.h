#pragma once

#include <fsm-editor/io/FSMReader.h>
#include <fsm-editor/model/Graph.h>

#include <QString>

class QTextStream;

class DummyReader : public FSMReader
{
public:
  /**
   * Read from an inputStream and outputs a graph.
   * @return The loaded graph.
   */
  virtual Graph read(QTextStream& inStream) override;

private:
  GraphState* readState(const QString& name, bool start, QTextStream& inStream);
  GraphTransition* readTransition(const QString& origin, const QString& destination, QTextStream& inStream);
};
