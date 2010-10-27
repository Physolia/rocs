/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ListStructure.h"
#include "KDebug"
#include "ListNode.h"
#include "Pointer.h"
#include <QDebug>

Rocs::ListStructure::ListStructure ( DataTypeDocument* parent ) : DataType ( parent ) {
  qDebug() << "Creating  a list structure";
  initialize();

//   arrangeNodes();
}

Rocs::ListStructure::ListStructure(DataType& other): DataType(other)
{
  initialize();
//   setDirected(true);
//   qDebug() << "Criando pelo construtor de cópia";
//   _animationGroup = new QParallelAnimationGroup(this);
//   setBegin(addDatum("P"));
//   begin()->hideName(true);
//   begin()->hideValue(false);

//   arrangeNodes();

}

void Rocs::ListStructure::initialize(){
  _front = new ListNode(this);
  _front->setName("P");
  _front->hideName(false);
  _front->hideValue(true);
  setDirected(true);

  _animationGroup = new QParallelAnimationGroup(this);
  arrangeNodes();
//   setBegin(0);
//   Datum * p = addDatum("P");
//   p->hideName(false);
//   p->hideValue(true);
//   setBegin(p);
}


Rocs::ListStructure::~ListStructure() {

}

Pointer* Rocs::ListStructure::addPointer(Datum* from, Datum* to){

    foreach(Pointer *e, from->adjacent_pointers()){
      e->self_remove();
    }

    Pointer * e =  DataType::addPointer ( from, to );
    arrangeNodes();
    return e;
}


void Rocs::ListStructure::setEngine(QScriptEngine* engine)
{
    DataType::setEngine(engine);
    _front->setEngine(engine);
}


Datum* Rocs::ListStructure::addDatum ( QString name ) {

    Datum *n = DataType::addDatum(new ListNode(this));
    n->setName(name);
    arrangeNodes();
    return n;
}

QScriptValue Rocs::ListStructure::front() {
  return _front->scriptValue();
}

QScriptValue Rocs::ListStructure::createNode(const QString & name){
    Datum * n = addDatum(name);
    n->setEngine( engine() );
    return n->scriptValue();
}

void Rocs::ListStructure::arrangeNodes(){
  qreal x;
  qreal y;
  if (_animationGroup->state() != QAnimationGroup::Stopped){
      _animationGroup->stop();
  }
  if (data().isEmpty()){
    _front->setX(40);
    _front->setY(120);
    return;
  }
  QScopedArrayPointer<bool>visited (new bool[data().count()]);
  for (int i = 0; i < data().count(); ++i){
      visited[i] = false;
  }

  ListNode * n = _front;

  if (n == 0 && !data().isEmpty()){
      n = qobject_cast<ListNode*>(data().at(0));
      visited[data().indexOf(n)] = true;
  }

  if (n == 0){
    return;
  }

  QPropertyAnimation * anim = new QPropertyAnimation(n, "x");;
  anim->setDuration(500);
  anim->setStartValue(n->x());
  anim->setEndValue(40);
  _animationGroup->addAnimation(anim);
  anim = new QPropertyAnimation(n, "y");;
  anim->setDuration(500);
  anim->setStartValue(n->y());
  anim->setEndValue(120);
  _animationGroup->addAnimation(anim);





  x = n->width() * 40;
  y = 250;
  while ((n = n->next())){
    if (visited[data().indexOf(n)] || n == _front){
      break;
    }
    visited[data().indexOf(n)] = true;
    x = x + 70 + n->width()*40;
    anim = new QPropertyAnimation(n, "x");;
    anim->setDuration(500);
    anim->setStartValue(n->x());
    anim->setEndValue(x);
    _animationGroup->addAnimation(anim);
    anim = new QPropertyAnimation(n, "y");;
    anim->setDuration(500);
    anim->setStartValue(n->y());
    anim->setEndValue(y);
    _animationGroup->addAnimation(anim);
  }
  x = y = 30;
  foreach (Datum * n, data()){
    if (!visited[data().indexOf(n)]){
      anim = new QPropertyAnimation(n, "x");;
      anim->setDuration(500);
      anim->setStartValue(n->x());
      anim->setEndValue(x);
      _animationGroup->addAnimation(anim);
      anim = new QPropertyAnimation(n, "y");;
      anim->setDuration(500);
      anim->setStartValue(n->y());
      anim->setEndValue(y);
      _animationGroup->addAnimation(anim);
//       n->setX(x);
//       n->setY(y);
      x += 60;
    }
  }
  _animationGroup->start();
}
