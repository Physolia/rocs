/* This file is part of KGraphViewer.
   Copyright (C) 2006-2007 Gael de Chalendar <kleag@free.fr>

   KGraphViewer is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA
*/


#include "DotGraphParsingHelper.h"
// #include "dotgraph.h"
#include "dotgrammar.h"
// #include "dotdefaults.h"
//#include "graphsubgraph.h"
// #include "graphdatum.h"
// #include "graphedge.h"

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_distinct.hpp>
#include <boost/spirit/include/classic_loops.hpp>
#include <boost/spirit/include/classic_confix.hpp>


#include <iostream>

#include <kdebug.h>

#include <QFile>
#include<QUuid>
#include "Core/DynamicPropertiesList.h"

using namespace std;

extern KGraphViewer::DotGraphParsingHelper* phelper;

namespace KGraphViewer
{
#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<int>::max()

DotGraphParsingHelper::DotGraphParsingHelper():
        attrid(),
        valid(),
        attributed(),
        subgraphid(),
        uniq(0),
        attributes(),
        graphAttributes(),
        datumAttributes(),
        pointersAttributes(),
        graphAttributesStack(),
        datumAttributesStack(),
        pointersAttributesStack(),
        edgebounds(),
        z(0),
        maxZ(0),
        graph(0),
//   gs(0),
        gn(0),
        ge(0)
{
}

void DotGraphParsingHelper::setgraphelementattributes(QObject* ge, const AttributesMap& attributes)
{
    AttributesMap::const_iterator it, it_end;
    it = attributes.begin();
    it_end = attributes.end();
    for (; it != it_end; it++)
    {
        kDebug() << "    " << QString::fromStdString((*it).first) << "\t=\t'" << QString::fromStdString((*it).second) <<"'";
        kDebug() << ge->metaObject()->className();
        if ((*it).first=="label" && strcmp(ge->metaObject()->className(), "Edge") == 0 )
        {

            QString label = QString::fromUtf8((*it).second.c_str());
            label.replace("\\n","\n");
            ge->setProperty("name", label);
        }
        else
        {
//       (*ge).attributes()[QString::fromStdString((*it).first)] =
            DynamicPropertiesList::New()->addProperty(ge, QString::fromStdString((*it).first));
            ge->setProperty((*it).first.c_str(), QString::fromStdString((*it).second));
        }
    }


}

void DotGraphParsingHelper::setgraphattributes()
{

    setgraphelementattributes(graph, graphAttributes);
}

void DotGraphParsingHelper::setsubgraphattributes()
{
//   kDebug() << "Attributes for subgraph are : ";
//   gs->setZ(z);
//   kDebug() << "z="<<gs->z();
//   setgraphelementattributes(gs, graphAttributes);
}

void DotGraphParsingHelper::setdatumattributes()
{

    if (gn == 0)
    {
//     kDebug() << "gn is null";
        return;
    }
//   kDebug() << "Attributes for datum " << gn->id() << " are : ";
//   gn->setZ(z+1);
//   kDebug() << "z="<<gn->z();
    setgraphelementattributes(gn, datumAttributes);
}

void DotGraphParsingHelper::setedgeattributes()
{
//   kDebug() << "setedgeattributeswith z = " << z;

//   kDebug() << "Attributes for edge " << ge->fromDatum()->id() << "->" << ge->toDatum()->id() << " are : ";
//   ge->setZ(z+1);
//   kDebug() << "z="<<ge->z();
    setgraphelementattributes(ge, pointersAttributes);

//   if (pointersAttributes.find("_tdraw_") != pointersAttributes.end())
//   {
//     parse_renderop(pointersAttributes["_tdraw_"], ge->renderOperations());
// //     kDebug() << "edge renderOperations size is now " << ge->renderOperations().size();
//     DotRenderOpVec::const_iterator it, it_end;
//     it = ge->renderOperations().constBegin(); it_end = ge->renderOperations().constEnd();
//     for (; it != it_end; it++)
//       ge->arrowheads().push_back(*it);
//   }
//   if (pointersAttributes.find("_hdraw_") != pointersAttributes.end())
//   {
//     parse_renderop(pointersAttributes["_hdraw_"], ge->renderOperations());
// //     kDebug() << "edge renderOperations size is now " << ge->renderOperations().size();
//     DotRenderOpVec::const_iterator it, it_end;
//     it = ge->renderOperations().constBegin(); it_end = ge->renderOperations().constEnd();
//     for (; it != it_end; it++)
//       ge->arrowheads().push_back(*it);
//   }
}

void DotGraphParsingHelper::setattributedlist()
{
// //   kDebug() << "Setting attributes list for " << QString::fromStdString(attributed);
    if (attributed == "graph")
    {
        if (attributes.find("bb") != attributes.end())
        {
            std::vector< int > v;
            parse_integers(attributes["bb"].c_str(), v);
            if (v.size()>=4)
            {
                kDebug() << "setting width and height to " << v[2] << v[3];
//         graph->width(v[2]);
//         graph->height(v[3]);
            }
        }
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++)
        {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            graphAttributes[(*it).first] = (*it).second;
        }
    }
    else if (attributed == "datum")
    {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++)
        {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            datumAttributes[(*it).first] = (*it).second;
        }
    }
    else if (attributed == "edge")
    {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++)
        {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            pointersAttributes[(*it).first] = (*it).second;
        }
    }
    attributes.clear();
}

void DotGraphParsingHelper::createdatum(const std::string& datumid)
{
    QString id = QString::fromStdString(datumid);
    kDebug() << id;
    gn = dynamic_cast<Datum*>(graph->datum(id));
    if (gn==0 )//&& graph->data().size() < KGV_MAX_ITEMS_TO_LOAD)
    {
        kDebug() << "Creating a new datum" << subgraphid;
        gn = graph->addDatum(id);

        if (!subgraphid.isEmpty()) {
            gn->addDynamicProperty("SubGraph", subgraphid.last());
//       kDebug () << gn->dynamicPropertyNames();
        }
//     gn->setId(id);
//     gn->label(QString::fromStdString(datumid));
//     if (z>0 && gs != 0)
//     {
// //       kDebug() << "Adding datum" << id << "in subgraph" << gs->id();
//       gs->content().push_back(gn);
//     }
//     else
//     {
// //       kDebug() << "Adding datum" << id;
//       graph->data()[id] = gn;
//     }
    }
    edgebounds.clear();
}

void DotGraphParsingHelper::createsubgraph()
{
//   kDebug() ;
    if (phelper)
    {
        ;

//     std::string str = phelper->subgraphid;
//     if (str.empty())
//     {
//       std::ostringstream oss;
//       oss << "kgv_id_" << phelper->uniq++;
//       str = oss.str();
//     }
// //     kDebug() << QString::fromStdString(str);
//     if (graph->subgraphs().find(QString::fromStdString(str)) == graph->subgraphs().end())
//     {
// //       kDebug() << "Creating a new subgraph";
//       gs = new GraphSubgraph();
//       gs->setId(QString::fromStdString(str));
// //       gs->label(QString::fromStdString(str));
//       graph->subgraphs().insert(QString::fromStdString(str), gs);
// //       kDebug() << "there is now"<<graph->subgraphs().size()<<"subgraphs in" << graph;
//     }
//     else
//     {
// //       kDebug() << "Found existing subgraph";
//       gs = *(graph->subgraphs().find(QString::fromStdString(str)));
//     }
//     phelper->subgraphid = "";
    }
}

void DotGraphParsingHelper::createedges()
{
//   kDebug();
    std::string datum1Name, datum2Name;
    datum1Name = edgebounds.front();
    edgebounds.pop_front();
    while (!edgebounds.empty())
    {
        datum2Name = edgebounds.front();
        edgebounds.pop_front();

        Datum* gn1 = graph->datum(QString::fromStdString(datum1Name));
        if (gn1 == 0)
        {
//       kDebug() << "new datum 1";
            gn1 = graph->addDatum(QString::fromStdString(datum1Name));

            if (!subgraphid.isEmpty()) {
                gn1->addDynamicProperty("SubGraph", subgraphid.last());
            }
//             gn1 = graph->datum(QString::fromStdString(datum1Name));
        }
        Datum* gn2 = graph->datum(QString::fromStdString(datum2Name));
        if (gn2 == 0)
        {
//       kDebug() << "new datum 1";
            gn2 = graph->addDatum(QString::fromStdString(datum2Name));

            if (!subgraphid.isEmpty()) {
                gn2->addDynamicProperty("SubGraph", subgraphid.last());
            }
        }
//     if (graph->data().size() >= KGV_MAX_ITEMS_TO_LOAD || graph->edges().size() >= KGV_MAX_ITEMS_TO_LOAD)
//     {
//       return;
//     }
//     kDebug() << QString::fromStdString(datum1Name) << ", " << QString::fromStdString(datum2Name);
//     rocsGraph;

//     kDebug() << "Found gn1="<<gn1<<" and gn2=" << gn2;
        if (gn1 == 0 || gn2 == 0)
        {
            kError() << "Unable to find or create edge bound(s) gn1=" << gn1 << "; gn2=" << gn2;
        }

        ge = graph->addPointer(gn1, gn2);
        if (!subgraphid.isEmpty()) {
            ge->addDynamicProperty("SubGraph", subgraphid.last());
        }
//     ge->setFromDatum(gn1);
//     ge->setToDatum(gn2);
//     kDebug() << ge->fromDatum()->id() << " -> " << ge->toDatum()->id();
        setedgeattributes();
//     kDebug() << ge->id();
//     if (ge->name() == "")
//     {
//       ge->setName(QString::fromStdString(datum1Name)+QString::fromStdString(datum2Name)+QUuid::createUuid().toString().remove("{").remove("}").remove("-"));
//     }
//     kDebug() << ge->id();
//     kDebug() << "num before=" << graph->edges().size();
//     graph->edges().insert(ge->id(), ge);
//     kDebug() << "num after=" << graph->edges().size();


        datum1Name = datum2Name;
    }
    edgebounds.clear();
}

void DotGraphParsingHelper::finalactions()
{
//   GraphEdgeMap::iterator it, it_end;
//   it = graph->edges().begin(); it_end = graph->edges().end();
//   for (; it != it_end; it++)
//   {
//     (*it)->setZ(maxZ+1);
//   }
}

}
