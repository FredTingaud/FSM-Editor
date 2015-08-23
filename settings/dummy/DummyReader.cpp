#include <fsm-editor/settings/dummy/DummyReader.h>

#include <fsm-editor/model/GraphStateImpl.h>
#include <fsm-editor/model/GraphTransitionImpl.h>

#include <QTextStream>

Graph DummyReader::read(QTextStream& inStream)
{
  Graph result;
  QRegExp stateRegexp("!! (Starting )?State ([A-Za-z0-9_\\-]+) \\{");
  QRegExp transitionRegexp("!! Transition from ([A-Za-z0-9_\\-]+) to ([A-Za-z0-9_\\-]+) \\{");
  std::list<std::unique_ptr<GraphState>> states;
  std::list<std::unique_ptr<GraphTransition>> transitions;
  while (!inStream.atEnd())
  {
    QString line = inStream.readLine();
    if (line.contains(stateRegexp))
    {
      states.emplace_back(readState(stateRegexp.cap(2), !stateRegexp.cap(1).isEmpty(), inStream));
    }
    else if (line.contains(transitionRegexp))
    {
      transitions.emplace_back(readTransition(transitionRegexp.cap(1), transitionRegexp.cap(2), inStream));
    }
  }
  result.setData(std::move(states), std::move(transitions));
  return std::move(result);
}

std::unique_ptr<GraphState> DummyReader::readState(const QString& name, bool start, QTextStream& inStream)
{
  std::unique_ptr<GraphStateImpl> state = std::make_unique<GraphStateImpl>(name, start);
  int openedBrackets = 1;
  QString code;
  QRegExp pos("\\} \\- ([0-9.\\-]+)\\:([0-9.\\-]+)");
  while (openedBrackets > 0 && !inStream.atEnd())
  {
    QString line = inStream.readLine();
    if (line.contains("{"))
    {
      ++openedBrackets;
    }
    if (line.contains("}"))
    {
      --openedBrackets;
    }
    if (openedBrackets > 0)
    {
      code += line + "\n";
    }
    else
    {
      if (line.contains(pos))
      {
        state->setCode(code);
        state->setPosition(QPointF(pos.cap(1).toDouble(), pos.cap(2).toDouble()));
      }
      else
      {
        Q_ASSERT_X(false, __FUNCTION__, "Malformed state contains no position");
      }
    }
  }
  return std::move(state);
}

std::unique_ptr<GraphTransition> DummyReader::readTransition(const QString& origin, const QString& destination, QTextStream& inStream)
{
  std::unique_ptr<GraphTransitionImpl> transition = std::make_unique<GraphTransitionImpl>(origin, destination);
  int openedBrackets = 1;
  QString code;
  while (openedBrackets > 0 && !inStream.atEnd())
  {
    QString line = inStream.readLine();
    if (line.contains("{"))
    {
      ++openedBrackets;
    }
    if (line.contains("}"))
    {
      --openedBrackets;
    }
    if (openedBrackets > 0)
    {
      code += line;
    }
  }
  transition->setCode(code);
  return std::move(transition);
}
