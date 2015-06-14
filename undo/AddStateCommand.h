#pragma once

#include <QUndoCommand>
#include <QPointF>

class QGraphicsScene;
class AddStateCommand : public QUndoCommand
{
public:
  AddStateCommand(QGraphicsScene* scene, const QString& name, const QPointF& position);

  virtual void undo() override;

  virtual void redo() override;

private:
  QGraphicsScene* scene_;
  QString name_;
  QPointF pos_;
};