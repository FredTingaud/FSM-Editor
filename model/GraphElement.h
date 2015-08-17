#pragma once

#include <QString>

/**
 * @brief Model representation shared between states and transitions.
 */
class GraphElement
{
public:
  /**
   * The names have to be unique in the graph, as they are the key used to store data.
   * @return The name of the element.
   */
  virtual QString name() const = 0;

  /**
   * Every element can have some code associated with it.
   * @return the code associated with this element.
   */
  virtual QString getCode() const = 0;
};
