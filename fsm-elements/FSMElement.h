#pragma once

#include <fsm-editor/model/GraphElement.h>

#include <QString>

class ExportVisitor;

/**
 * @brief Common parent to both States and Transitions.
 *
 * View equivalent to GraphElement.
 *
 * @see GraphElement.
 */
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

  /**
   * Pass an error message and display an error warning on the element.
   */
  virtual void setInError(const QString& error);

  /**
   * Remove the error warning and clear error message, if any.
   */
  virtual void clearError();

  /**
   * @return The current error message if any, an empty string otherwise.
   */
  virtual QString getErrorMessage() const;

  /**
   * Convenience message to check whether the error message is empty.
   */
  bool isInError() const;

private:
  QString code_;
  QString error_;
};
