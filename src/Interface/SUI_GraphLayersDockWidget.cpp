
/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "SUI_GraphLayersDockWidget.h"
#include "SUI_MainWindow.h"

#include "graph.h"
#include "graphDocument.h"
#include "model_GraphLayers.h"

#include <KDebug>

GraphLayersDockWidget::GraphLayersDockWidget(QWidget* parent, Qt::WindowFlags flags)
: QDockWidget(i18n("Graph Layers"),parent, flags) 
{
  _document = 0;
  _layerModel = 0;

  setupUi(this);
  _btnNewGraph ->setEnabled(false);
  connect(_graphLayersListView, SIGNAL(clicked(const QModelIndex&)),
   this, SLOT(setActiveGraph(const QModelIndex&)));
}

void GraphLayersDockWidget::on__btnNewGraph_clicked()
{
  _graphLayersListView->model()->insertRows(0, GraphDocument::Simple);
}

void GraphLayersDockWidget::on__btnNewMultiGraph_clicked(){
  _graphLayersListView->model()->insertRows(0, GraphDocument::Multi);
}

void GraphLayersDockWidget::on__btnNewOrientedGraph_clicked(){
  _graphLayersListView->model()->insertRows(0, GraphDocument::Oriented);
}

void GraphLayersDockWidget::setGraphDocument( GraphDocument *document)
{
  kDebug() << "Got The Graph";
  if ( _layerModel != 0) delete _layerModel;
  _document = document;
  _layerModel = new GraphLayersModel( document );
  _graphLayersListView->setModel( _layerModel );
  _btnNewGraph -> setEnabled(true);
}

void GraphLayersDockWidget::setActiveGraph(const QModelIndex& modelindex){
   Graph *g = _layerModel -> at(modelindex);
  if (g == 0) kDebug() << "Returning a NULL pointer";
  else{
    emit activeGraphChanged( g );
    kDebug() << "Send the Graph at position " << modelindex.row();
  }
}
