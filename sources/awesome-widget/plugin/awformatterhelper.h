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


#ifndef AWFORMATTERHELPER_H
#define AWFORMATTERHELPER_H

#include "abstractextitemaggregator.h"

#include "awabstractformatter.h"


class AWFormatterHelper : public AbstractExtItemAggregator
{
    Q_OBJECT

public:
    explicit AWFormatterHelper(QWidget *_parent = nullptr);
    virtual ~AWFormatterHelper();
    QString convert(const QVariant &_value, const QString &_name) const;
    QStringList definedFormatters() const;
    QHash<QString, QString> getFormatters() const;
    void initItems();
    QList<AbstractExtItem *> items() const;
    QStringList knownFormatters() const;
    bool removeUnusedFormatters(const QStringList &_keys) const;
    bool writeFormatters(const QHash<QString, QString> &_configuration) const;

public slots:
    void editItems();

private:
    // methods
    AWAbstractFormatter::FormatterClass
    defineFormatterClass(const QString &_stringType) const;
    void initFormatters();
    void initKeys();
    void installDirectories();
    QPair<QString, AWAbstractFormatter::FormatterClass>
    readMetadata(const QString &_filePath) const;
    // parent methods
    void doCreateItem();
    // properties
    QStringList m_directories;
    QString m_filePath;
    QHash<QString, AWAbstractFormatter *> m_formatters;
    QHash<QString, AWAbstractFormatter *> m_formattersClasses;
};


#endif /* AWFORMATTERHELPER_H */
