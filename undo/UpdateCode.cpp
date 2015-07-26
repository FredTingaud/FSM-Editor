#include <fsm-editor/undo/UpdateCode.h>

#include <fsm-editor/FSMScene.h>
#include <fsm-editor/fsm-elements/FSMElement.h>
#include <fsm-editor/fsm-elements/State.h>

UpdateCode::UpdateCode(FSMScene* scene, const QString& element, const QString& newCode)
  : QUndoCommand(QString("Change code for %1").arg(scene->getElement(element)->name()))
  , scene_(scene)
  , element_(element)
  , previousCode_(scene->getElement(element)->getCode())
  , nextCode_(newCode)
{}

void UpdateCode::undo()
{
  FSMElement* element = scene_->getElement(element_);
  element->setCode(previousCode_);
  scene_->selectElement(element_);
}

void UpdateCode::redo()
{
  FSMElement* element = scene_->getElement(element_);
  element->setCode(nextCode_);
  scene_->selectElement(element_);
}

int UpdateCode::id() const
{
  return FSMScene::UNDO_IDS::UNDO_MOVE;
}

bool UpdateCode::mergeWith(const QUndoCommand *other)
{
  if (other->id() != id()
      || static_cast<const UpdateCode*>(other)->scene_ != scene_
      || static_cast<const UpdateCode*>(other)->element_ != element_
      || static_cast<const UpdateCode*>(other)->previousCode_ != nextCode_
      )
      return false;
  nextCode_ = static_cast<const UpdateCode*>(other)->nextCode_;
  return true;
}
