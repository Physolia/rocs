/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "editorpluginmanager.h"
#include "editorplugininterface.h"
#include "logging_p.h"

#include <KPluginMetaData>
#include <KPluginLoader>
#include <KPluginFactory>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonObject>

using namespace GraphTheory;

class GraphTheory::EditorPluginManagerPrivate
{
public:
    EditorPluginManagerPrivate()
    {

    }

    ~EditorPluginManagerPrivate()
    { }

    QList<EditorPluginInterface*> m_plugins;
};

EditorPluginManager::EditorPluginManager()
    : d(new EditorPluginManagerPrivate)
{
    loadPlugins();
}

EditorPluginManager::~EditorPluginManager()
{

}

QList<EditorPluginInterface*> EditorPluginManager::plugins() const
{
    return d->m_plugins;
}

void EditorPluginManager::loadPlugins()
{
    // remove all present plugins
    foreach(EditorPluginInterface *f, d->m_plugins) {
        delete f;
    }
    d->m_plugins.clear();

    // dirs to check for plugins
    QStringList dirsToCheck;
    foreach (const QString &directory, QCoreApplication::libraryPaths()) {
        dirsToCheck << directory + QDir::separator() + "rocs/editorplugins";
    }

    // load plugins
    foreach (const QString &dir, dirsToCheck) {
        QVector<KPluginMetaData> metadataList = KPluginLoader::findPlugins(dir,[=](const KPluginMetaData &data){
            return data.serviceTypes().contains("rocs/editorplugins");
        });
        for (const auto &metadata : metadataList) {
            KPluginFactory *factory = KPluginLoader(metadata.fileName()).factory();
            EditorPluginInterface *plugin = factory->create<EditorPluginInterface>(this);
            plugin->setDisplayName(metadata.name());
            d->m_plugins.append(plugin);
            qCDebug(GRAPHTHEORY_GENERAL) << "Loaded plugin:" << metadata.name();
        }
    }
}
