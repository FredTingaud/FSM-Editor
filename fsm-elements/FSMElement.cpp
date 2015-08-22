#include <fsm-editor/fsm-elements/FSMElement.h>

QString FSMElement::getCode() const
{
  return code_;
}

void FSMElement::setCode(const QString& code)
{
  code_ = code;
}

void FSMElement::setInError(const QString& error)
{
  error_ = error;
}

void FSMElement::clearError()
{
  error_ = "";
}

QString FSMElement::getErrorMessage() const
{
  return error_;
}

bool FSMElement::isInError() const
{
  return !error_.isEmpty();
}
