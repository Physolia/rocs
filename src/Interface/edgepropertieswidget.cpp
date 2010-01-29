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

#include "edgepropertieswidget.h"
#include "edge.h"
#include "MainWindow.h"
#include "model_GraphProperties.h"
#include <graph.h>

EdgePropertiesWidget::EdgePropertiesWidget(MainWindow *parent): QWidget(parent) {
    setupUi(this);
        
}

void EdgePropertiesWidget::setEdge(Edge *e, QPointF pos) {
    _edge = e;
    move(pos.x()+ 10,  pos.y() + 10);
    
    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_edge);
    
    _propertiesTable->setModel(model);
    
    
    show();
    activateWindow();
    raise();
    disconnect();
    
    connect(_edge, SIGNAL(updateNeeded()), this, SLOT(reflectAttributes()));
    connect(_edge, SIGNAL(posChanged()), this, SLOT(reflectAttributes()));
    reflectAttributes();
}

void EdgePropertiesWidget::reflectAttributes(){
   _name->setText(_edge->name());
   _value->setText(_edge->value());
   _color->setColor(_edge->color());
   _width->setValue(_edge->width());
   _propertyName->setText("");
   _propertyValue->setText("");
   _isPropertyGlobal->setCheckState(Qt::Unchecked);
}
void EdgePropertiesWidget::on__name_textChanged(const QString& s) {
    _edge->setName(s);
}

void EdgePropertiesWidget::on__value_textChanged(const QString& s) {
    _edge->setValue(s);
}

void EdgePropertiesWidget::on__color_activated(const QColor& c) {
    _edge->setColor(c.name());
}

void EdgePropertiesWidget::on__width_valueChanged(double v) {
    _edge->setWidth(v);
}

void EdgePropertiesWidget::on__style_activated(int index) {
    switch(index){
      case 0 : _edge->setStyle("solid"); break;
      case 1 : _edge->setStyle("dash"); break;
      case 2 : _edge->setStyle("dot"); break;
      case 3 : _edge->setStyle("dash dot"); break;
    }
}

void EdgePropertiesWidget::on__showName_toggled(bool b){
  _edge->hideName(!b);
}

void EdgePropertiesWidget::on__showValue_toggled(bool b){
  _edge->hideValue(!b);
}

void EdgePropertiesWidget::on__addProperty_clicked(){
    if (_isPropertyGlobal->checkState() == Qt::Checked ){
      _edge->graph()->addEdgesDinamicProperty(_propertyName->text(),
					      QVariant(_propertyValue->text()));
    }else{
      _edge->addDinamicProperty(_propertyName->text(),
				QVariant(_propertyValue->text()));
    }
}