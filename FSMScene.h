#pragma once

#include <QGraphicsScene>

class QUndoCommand;
class State;
class Transition;
class FSMElement;
class ExportVisitor;
class FSMScene : public QGraphicsScene
{
  Q_OBJECT;
public:
  FSMScene();

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

  void pushCommand(QUndoCommand* command);

  Q_SIGNAL void command(QUndoCommand* command);
  Q_SIGNAL void codeChanged(const QString& code);
  Q_SIGNAL void codeHidden();

  Q_SLOT void checkSelection();

  State* addState(const QString& name, const QPointF& pos);
  void removeState(const QString& name);
  State* getState(const QString& name) const;
  FSMElement* getElement(const QString& name) const;

  Transition* getTransition(const QString& name) const;

  void setCode(FSMElement* element, const QString& code);
  void updateCode(const QString& code);
  QString generateExport(ExportVisitor& visitor);

  void selectElement(const QString& element);

public:
  enum UNDO_IDS
  {
    UNDO_MOVE = 1,
    UNDO_CODE = 2
  };

private:
  static int index;
  static const QColor BACKGROUND_COLOR;

private:
  std::map<QString, State*> states_;
  FSMElement* editingElement_;
};
