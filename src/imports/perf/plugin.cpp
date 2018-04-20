// Copyright © 2018 Loïc Molinari <loicm@loicm.fr>
//
// This file is part of Quicken.
//
// Quicken is free software: you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; version 3.
//
// Quicken is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Quicken. If not, see <http://www.gnu.org/licenses/>.

#include <QtQml/qqmlextensionplugin.h>

#include <Quicken/Quicken>  // FIXME

class QuickenPerfPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    QuickenPerfPlugin(QObject* parent = Q_NULLPTR) : QQmlExtensionPlugin(parent) { }
    ~QuickenPerfPlugin() { }

    void registerTypes(const char* uri) Q_DECL_OVERRIDE {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Quicken.Perf"));
        Q_UNUSED(uri);

        // FIXME Register types here.
    }
};

#include "plugin.moc"