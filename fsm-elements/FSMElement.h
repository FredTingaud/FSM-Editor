#pragma once

/**
 * Common parent to both States and Transitions.
 * View equivalent to GraphElement.
 *
 * @see GraphElement.
 */
#include <fsm-editor/model/GraphElement.h>

#include <QString>

class ExportVisitor;

class FSMElement : public GraphElement
{
public:
  /**
   * @return the code associated to the element.
   */
  virtual QString getCode() const override;

  /**
   * Sets the code associated to the element.
   */
  virtual void setCode(const QString& code);
private:
  QString code_;
};
