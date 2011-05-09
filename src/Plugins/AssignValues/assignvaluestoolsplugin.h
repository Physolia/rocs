/*
    Rocs-Tools-Plugin: Automatic assign of values to edges/nodes
    Copyright (C) 2011  Andreas Cord-Landwehr

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

#ifndef ASSIGNVALUESTOOLSPLUGIN_H
#define ASSIGNVALUESTOOLSPLUGIN_H

#include "../ToolsPluginInterface.h"

class QObject;

class AssignValuesToolPlugin : public ToolsPluginInterface
{
    Q_OBJECT  

public:
    AssignValuesToolPlugin(QObject* parent, const QList< QVariant >&);
    ~AssignValuesToolPlugin();
    QString run(QObject* parent = 0) const;
};

#endif // ASSIGNVALUESTOOLSPLUGIN_H
