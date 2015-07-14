#include <fsm-editor/fsm-elements/FSMElement.h>

QString FSMElement::getCode() const
{
  return code_;
}

void FSMElement::setCode(const QString& code)
{
  code_ = code;
}
