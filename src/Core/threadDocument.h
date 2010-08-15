/***************************************************************************
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef THREADDOCUMENT_H
#define THREADDOCUMENT_H

#include <QThread>
#include <QMutex>
#include "rocslib_export.h"
#include <QWaitCondition>
class DocumentManager;

class DataType;
class Datum;
class Pointer;

class DataTypeDocument;
class QtScriptBackend;

class ROCSLIB_EXPORT ThreadDocument : public QThread{
  Q_OBJECT

  public:
    ThreadDocument(QWaitCondition &docCondition,QMutex &mutex, QObject *parent = 0);
    virtual ~ThreadDocument();
    DataTypeDocument *document() const;// {return _graphDocument;}
    bool isRunning() const;
    QtScriptBackend* engine() const;

//    /** release actual document and set the doc */
//     void setGraphDocument(GraphDocument * doc);
    //void setDocumentName(const QString& name);

    /** Return a pointer to document manager. Delete it can lead to a crash.*/
    DocumentManager* documentManager();

  public slots:
    void run();
//     void createEmptyDocument();
//     void loadDocument(const QString& name = QString());

    void terminate();

  private:
    /** Release actual document */
//     void releaseDocument();

    QtScriptBackend* _engine;
    DataTypeDocument * _graphDocument;
    QWaitCondition &_docCondition;
    QMutex &_mutex;
    QString _documentName;

    DocumentManager * m_docManager;

};

#endif
