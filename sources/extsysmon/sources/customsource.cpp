/***************************************************************************
 *   This file is part of awesome-widgets                                  *
 *                                                                         *
 *   awesome-widgets is free software: you can redistribute it and/or      *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   awesome-widgets is distributed in the hope that it will be useful,    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with awesome-widgets. If not, see http://www.gnu.org/licenses/  *
 ***************************************************************************/


#include "customsource.h"

#include "awdebug.h"
#include "extscript.h"


CustomSource::CustomSource(QObject *parent, const QStringList args)
    : AbstractExtSysMonSource(parent, args)
{
    Q_ASSERT(args.count() == 0);
    qCDebug(LOG_ESM);

    extScripts = new ExtItemAggregator<ExtScript>(nullptr, QString("scripts"));
    m_sources = getSources();
}


CustomSource::~CustomSource()
{
    qCDebug(LOG_ESM);

    delete extScripts;
}


QVariant CustomSource::data(QString source)
{
    qCDebug(LOG_ESM);
    qCDebug(LOG_ESM) << "Source" << source;

    // there are only one value
    return extScripts->itemByTagNumber(index(source))->run().values().first();
}


QVariantMap CustomSource::initialData(QString source) const
{
    qCDebug(LOG_ESM);
    qCDebug(LOG_ESM) << "Source" << source;

    QVariantMap data;
    data[QString("min")] = QString("");
    data[QString("max")] = QString("");
    data[QString("name")]
        = QString("Custom command '%1' output")
              .arg(extScripts->itemByTagNumber(index(source))->uniq());
    data[QString("type")] = QString("QString");
    data[QString("units")] = QString("");

    return data;
}


QStringList CustomSource::sources() const
{
    qCDebug(LOG_ESM);

    return m_sources;
}


QStringList CustomSource::getSources()
{
    qCDebug(LOG_ESM);

    QStringList sources;
    foreach (ExtScript *item, extScripts->activeItems())
        sources.append(QString("custom/%1").arg(item->tag(QString("custom"))));

    return sources;
}
