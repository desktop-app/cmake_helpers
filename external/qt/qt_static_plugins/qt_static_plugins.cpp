/*
This file is part of Desktop App Toolkit,
a set of libraries for developing nice desktop applications.

For license and copyright information please follow this link:
https://github.com/desktop-app/legal/blob/master/LEGAL
*/
#include <QtCore/QtPlugin>

#if !defined QT_STATIC_PLUGINS_USE_PACKAGED && QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined Q_OS_WIN
Q_IMPORT_PLUGIN(QGenericEnginePlugin)
Q_IMPORT_PLUGIN(QWebpPlugin)
Q_IMPORT_PLUGIN(QJpegPlugin)
Q_IMPORT_PLUGIN(QGifPlugin)
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif // !QT_STATIC_PLUGINS_USE_PACKAGED && Qt < 6.0.0 && Q_OS_WIN
