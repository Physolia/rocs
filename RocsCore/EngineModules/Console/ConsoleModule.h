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

#ifndef CONSOLEMODULE_H
#define CONSOLEMODULE_H

#include <QObject>
#include <QList>
#include <QPair>

#include "RocsCoreExport.h"

/**
 * \class ConsoleModule
 * This class provides an interface object for the scripting engine to write informatoin to
 * the application console. The application widget that displays these information shall listen
 * to this object.
 */
class ROCSLIB_EXPORT ConsoleModule : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleModule(QObject *parent = 0);
    ~ConsoleModule();

    enum MessageType {
        Log,
        Debug,
        Error
    };

    /**
     * Clear the backlog.
     */
    void clear();

    /**
     * Get all backlog since last clear.
     * \return the backlog
     */
    QList< QPair<MessageType, QString> > backlog() const;

public slots:
    /**
     * Print a log message \p message.
     */
    Q_INVOKABLE void log(const QString &message);

    /**
     * Print a debug message \p message.
     */
    Q_INVOKABLE void debug(const QString &message);

    /**
     * Print an error message \p message.
     */
    Q_INVOKABLE void error(const QString &message);

signals:
    void backlogChanged(ConsoleModule::MessageType,QString);

private:
    Q_DISABLE_COPY(ConsoleModule)
    QList< QPair<MessageType, QString> > _backlog;
};

#endif // CONSOLEMODULE_H