/*
    This file is part of Rocs.
    Copyright 2004-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TestDataStructure.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "KrossBackend.h"
#include "QtScriptBackend.h"
#include <qtest_kde.h>

#include <kross/core/action.h>
#include <kross/core/manager.h>
#include <KDebug>
#include <Document.h>
#include <DataStructurePluginManager.h>
#include <DocumentManager.h>

TestDataStructure::TestDataStructure()
{
    DocumentManager::self()->addDocument(new Document("test"));;
}

void TestDataStructure::cleanupTestCase()
{
}

void TestDataStructure::dataAddDeleteTest()
{
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);
    DataList dataList;

    // create 10 data elements
    for (int i = 0; i < 10; i++) {
        dataList.append(ds->addData(QString(i)));
    }
    QVERIFY2(ds->dataList().size() == 10, "ERROR: Number of data elements is not 10");

    // remove all data elements
    foreach(DataPtr data, dataList) {
        data->remove();
    }

    QVERIFY2(ds->dataList().size() == 0, "ERROR: Not all data elements were deleted");

    ds->remove();
}

void TestDataStructure::pointerAddDeleteTest()
{
    // test for undirected pointers
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);
    DataList dataList;

    // create 10 data elements
    // x x x x x x x x x x
    for (int i = 0; i < 10; i++) {
        dataList.append(ds->addData(QString(i)));
    }
    QVERIFY2(ds->dataList().size() == 10, "ERROR: Number of data elements is not 10");

    // connect data elements to a line
    // x-x-x-x-x-x-x-x-x-x
    for (int i = 0; i < dataList.size() - 1; i++) {
        ds->addPointer(dataList[i], dataList[i + 1]);
    }
    QVERIFY2(ds->pointers().size() == 9, "ERROR: Number of data elements is not 9");
    QVERIFY2(dataList[0]->pointerList().size() == 1, "ERROR: data gives wrong number of pointers");
    QVERIFY2(dataList[1]->pointerList().size() == 2, "ERROR: data gives wrong number of pointers");

    // remove first pointer from list
    // x x-x-x-x-x-x-x-x-x
    ds->pointers().first()->remove();
    QVERIFY2(ds->pointers().size() == 8, "ERROR: pointer was not removed");
    QVERIFY2(dataList[0]->pointerList().size() == 0, "ERROR: data gives wrong number of pointers");
    QVERIFY2(dataList[1]->pointerList().size() == 1, "ERROR: data gives wrong number of pointers");

    // remove second node, should trigger deletion of second pointer
    // x o x-x-x-x-x-x-x-x
    dataList[1]->remove();
    QVERIFY2(ds->pointers().size() == 7, "ERROR: data deletion did not remove its pointers");

    // remove fourth node, should trigger deletion of its two adjacend pointers
    // x o x o x-x-x-x-x-x
    dataList[3]->remove();
    QVERIFY2(ds->pointers().size() == 5, "ERROR: data deletion did not remove its both pointers");

    ds->remove();
}


void TestDataStructure::createSimpleGraph()
{
    QMap<QString, DataPtr> dataList;
    /* Creates a simple Graph with 5 datums and connects them with pointers. */
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);

    ds->setProperty("name", "Graph1");
    dataList.insert("a", ds->addData("a"));
    dataList.insert("b", ds->addData("b"));
    dataList.insert("c", ds->addData("c"));
    dataList.insert("d", ds->addData("d"));
    dataList.insert("e", ds->addData("e"));

    ds->addPointer(dataList["a"], dataList["b"]);
    ds->addPointer(dataList["b"], dataList["c"]);
    ds->addPointer(dataList["c"], dataList["d"]);
    ds->addPointer(dataList["d"], dataList["e"]);
    ds->addPointer(dataList["e"], dataList["a"]);

    QVERIFY2(ds->dataList().size() == 5, "ERROR: Number of data is not 5 ");
    QVERIFY2(ds->pointers().size() == 5, "ERROR: Number of pointers is not 5 ");

    foreach(DataPtr n, ds->dataList()) {
        QVERIFY2(n->outPointerList().size() == 1, "ERROR: Number of out pointers is not 1");
        QVERIFY2(n->inPointerList().size() == 1, "ERROR: Number of in pointers is not 1");
        QVERIFY2(n->adjacentDataList().size() == 2, "ERROR: Number of Adjacent Nodes is not 2");
        QVERIFY2(n->pointerList().size() == 2, "ERROR: Number of adjacent pointers is not 2");
    }
    dataList.clear();
    ds->remove();
}


void TestDataStructure::dataTypesTest()
{
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);

    DataList dataListDefault, dataList1, dataList2;
    QVERIFY2(ds->document()->dataTypeList().size() == 1, "ERROR: no default data type created");

    // register two further data types
    int type1 = ds->document()->registerDataType("type1");
    int type2 = ds->document()->registerDataType("type2");
    QVERIFY2(ds->document()->dataTypeList().size() == 3, "ERROR: data types were not created");

    // create data elements
    for (int i = 0; i < 3; i++) {
        dataListDefault.append(ds->addData(QString(i)));
        dataList1.append(ds->addData(QString(i), type1));
        dataList2.append(ds->addData(QString(i), type2));
    }
    QVERIFY2(ds->dataList().size() == 3,
             "list contains " + ds->dataList().size()
            );
    QVERIFY2(ds->dataList(type1).size() == 3,
             "list contains " + ds->dataList().size()
            );
    QVERIFY2(ds->dataList(type2).size() == 3,
             "list contains " + ds->dataList().size()
            );
    QVERIFY2(dataListDefault.at(0)->dataType() == 0, "ERROR: not correct autoset of type");
    QVERIFY2(dataList1.at(0)->dataType() == type1, "ERROR: not correct autoset of type");
    QVERIFY2(dataList2.at(0)->dataType() == type2, "ERROR: not correct autoset of type");

    // add pointers
    ds->addPointer(dataListDefault[0], dataList1[0]);
    ds->addPointer(dataList1[0], dataList2[0]);
    ds->addPointer(dataList2[0], dataListDefault[0]);
    QVERIFY2(ds->pointers(0).size() == 3, "ERROR: pointers were not correctly created");

    // remove data type
    ds->document()->removeDataType(type2);
    QVERIFY2(!ds->document()->dataTypeList().contains(type2),"ERROR: data type was not unregistered");
    QVERIFY2(ds->pointers(0).size() == 1, "ERROR: pointers were not correctly deleted");

    ds->remove();
}


void TestDataStructure::pointerTypesTest()
{
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);

    DataList dataList;
    QVERIFY2(ds->document()->pointerTypeList().size() == 1, "ERROR: no default pointer type created");

    // create data elements
    for (int i = 0; i < 10; i++) {
        dataList.append(ds->addData(QString(i)));
    }

    // register two further data types
    int type1 = ds->document()->registerPointerType("type1");
    QVERIFY2(ds->document()->pointerTypeList().size() == 2, "ERROR: pointer types were not created");

    // connect data elements to a line
    for (int i = 0; i < 4; i++) {
        ds->addPointer(dataList[i], dataList[i + 1]);
    }
    for (int i = 0; i < 9; i++) {
        ds->addPointer(dataList[i], dataList[i + 1], type1);
    }
    QVERIFY2(ds->pointers().size() == 4, "ERROR: wrong number of pointers");
    QVERIFY2(ds->pointers(type1).size() == 9, "ERROR: wrong number of pointers");
    QVERIFY(dataList[0]->adjacentDataList().size() == 1);
    QVERIFY(dataList[1]->adjacentDataList().size() == 2);
    QVERIFY(dataList[5]->adjacentDataList().size() == 2);

    // remove first node
    dataList[0]->remove();
    QVERIFY2(ds->pointers().size() == 3, "ERROR: wrong number of pointers");
    QVERIFY2(ds->pointers(type1).size() == 8, "ERROR: wrong number of pointers");

    ds->document()->removePointerType(type1);
    QVERIFY(dataList[2]->adjacentDataList().size() == 2);
    QVERIFY(dataList[6]->adjacentDataList().size() == 0);

    ds->remove();
}

void TestDataStructure::pointerDirectionChange()
{
    QMap<int, DataPtr> dataList;
    DataStructurePtr ds = DataStructure::create(DocumentManager::self()->activeDocument());
    DocumentManager::self()->activeDocument()->setActiveDataStructure(ds);

    // add two nodes 0 and 1, set type of default pointer type to unidirectional
    ds->document()->pointerType(0)->setDirection(PointerType::Unidirectional);
    dataList.insert(0, ds->addData("0"));
    dataList.insert(1, ds->addData("1"));

    // add pointer from 0 to 1 and test pointer list properties
    dataList[0]->addPointer(dataList[1]);
    QVERIFY(dataList[0]->outPointerList().length() == 1);
    QVERIFY(dataList[0]->inPointerList().length() == 0);
    QVERIFY(dataList[0]->adjacentDataList().length() == 1);
    QVERIFY(dataList[1]->outPointerList().length() == 0);
    QVERIFY(dataList[1]->inPointerList().length() == 1);
    QVERIFY(dataList[1]->adjacentDataList().length() == 1);

    ds->document()->pointerType(0)->setDirection(PointerType::Bidirectional);
    QVERIFY(dataList[0]->outPointerList().length() == 1);
    QVERIFY(dataList[0]->inPointerList().length() == 1);
    QVERIFY(dataList[0]->adjacentDataList().length() == 1);
    QVERIFY(dataList[1]->outPointerList().length() == 1);
    QVERIFY(dataList[1]->inPointerList().length() == 1);
    QVERIFY(dataList[1]->adjacentDataList().length() == 1);

    ds->document()->pointerType(0)->setDirection(PointerType::Unidirectional);
    QVERIFY(dataList[0]->outPointerList().length() == 1);
    QVERIFY(dataList[0]->inPointerList().length() == 0);
    QVERIFY(dataList[0]->adjacentDataList().length() == 1);
    QVERIFY(dataList[1]->outPointerList().length() == 0);
    QVERIFY(dataList[1]->inPointerList().length() == 1);
    QVERIFY(dataList[1]->adjacentDataList().length() == 1);

    dataList.clear();
    ds->remove();
}


QTEST_KDEMAIN_CORE(TestDataStructure)