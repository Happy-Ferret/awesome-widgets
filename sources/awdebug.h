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


#ifndef AWDEBUG_H
#define AWDEBUG_H

#include <QLoggingCategory>

#ifndef LOG_FORMAT
#define LOG_FORMAT "[%{time yyyy-MM-ddTHH:mm:ss.zzzZ}][%{if-debug}DD%{endif}%{if-info}II%{endif}%{if-warning}WW%{endif}%{if-critical}CC%{endif}%{if-fatal}FF%{endif}][%{category}][%{function}] %{message}"
#endif /* LOG_FORMAT */

// define info log level
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
#ifndef qCInfo
#define qCInfo qCDebug
#endif /* qCInfo */
#endif /* QT_VERSION */


Q_DECLARE_LOGGING_CATEGORY(LOG_AW)
Q_DECLARE_LOGGING_CATEGORY(LOG_DP)
Q_DECLARE_LOGGING_CATEGORY(LOG_ESM)
Q_DECLARE_LOGGING_CATEGORY(LOG_LIB)


#endif /* AWDEBUG_H */
