/*
    This file is part of Rocs.
    Copyright 2013       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef APIDOCWIDGET_H
#define APIDOCWIDGET_H

#include "ui_ApiDocWidget.h"
#include <QWidget>

class QUrl;
class ApiDocManager;
class ApiDocModel;

/**
 * \class ApiDocWidget
 * This Widget loads API information about all script interfaces and displays them.
 */
class ApiDocWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Default constructor.
     */
    explicit ApiDocWidget(QWidget* parent);
    void showHtmlOutline(bool logHistory);

public slots:
    void showTreeOutline();
    void showHtmlOutline();
    void showDetails(const QModelIndex &index);
    void showObjectApi(const QString &id, bool logHistory);
    void showObjectApi(const QUrl &aliasPage);
    void historyGoBack();
    void historyGoForward();

private:
    ApiDocManager *_manager;
    ApiDocModel *_model;
    QUrl _baseUrl;
    QList<QString> _history;
    int _historyPointer;
    Ui::ApiDocWidget *ui;
};

#endif // APIDOCWIDGET_H
