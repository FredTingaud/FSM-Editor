#include <fsm-editor/io/FSMReader.h>

#include <fsm-editor/settings/dummy/DummyTransition.h>
#include <fsm-editor/settings/dummy/DummyState.h>

#include <QTextStream>

Graph FSMReader::read(QTextStream& inStream)
{
  Graph result;
  QRegExp stateRegexp("!! State ([A-Za-z0-9_\\-]+) \\{");
  QRegExp transitionRegexp("!! Transition from ([A-Za-z0-9_\\-]+) to ([A-Za-z0-9_\\-]+) \\{");
  QList<GraphState*> states;
  QList<GraphTransition*> transitions;
  while (!inStream.atEnd())
  {
    QString line = inStream.readLine();
    if (line.contains(stateRegexp))
    {
      states.append(readState(stateRegexp.cap(1), inStream));
    }
    else if (line.contains(transitionRegexp))
    {
      transitions.append(readTransition(transitionRegexp.cap(1), transitionRegexp.cap(2), inStream));
    }
  }
  result.setData(std::move(states), std::move(transitions));
  return result;
}

GraphState* FSMReader::readState(const QString& name, QTextStream& inStream)
{
  DummyState* state = new DummyState(name);
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
      code += line;
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
  return state;
}

GraphTransition* FSMReader::readTransition(const QString& origin, const QString& destination, QTextStream& inStream)
{
  DummyTransition* transition = new DummyTransition(origin, destination);
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
  return transition;
}
