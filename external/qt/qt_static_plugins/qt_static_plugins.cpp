/*
This file is part of Desktop App Toolkit,
a set of libraries for developing nice desktop applications.

For license and copyright information please follow this link:
https://github.com/desktop-app/legal/blob/master/LEGAL
*/
#include <QtCore/QtPlugin>

#ifndef DESKTOP_APP_USE_PACKAGED
Q_IMPORT_PLUGIN(QWebpPlugin)

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
Q_IMPORT_PLUGIN(QJpegPlugin)
Q_IMPORT_PLUGIN(QGifPlugin)
#endif // Qt 5.8.0

#ifdef Q_OS_WIN
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif defined Q_OS_MAC // Q_OS_WIN
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
Q_IMPORT_PLUGIN(QGenericEnginePlugin)
#elif defined Q_OS_UNIX // Q_OS_WIN | Q_OS_MAC
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QXcbEglIntegrationPlugin)
Q_IMPORT_PLUGIN(QXcbGlxIntegrationPlugin)
Q_IMPORT_PLUGIN(QGenericEnginePlugin)
Q_IMPORT_PLUGIN(QComposePlatformInputContextPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)
#ifndef DESKTOP_APP_DISABLE_DBUS_INTEGRATION
Q_IMPORT_PLUGIN(QConnmanEnginePlugin)
Q_IMPORT_PLUGIN(QNetworkManagerEnginePlugin)
Q_IMPORT_PLUGIN(QIbusPlatformInputContextPlugin)
#endif // !DESKTOP_APP_DISABLE_DBUS_INTEGRATION
#ifndef DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION
Q_IMPORT_PLUGIN(QWaylandIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandEglPlatformIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandEglClientBufferPlugin)
Q_IMPORT_PLUGIN(QWaylandWlShellIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandBradientDecorationPlugin)
#endif // !DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION
#ifndef DESKTOP_APP_DISABLE_GTK_INTEGRATION
Q_IMPORT_PLUGIN(QGtk3ThemePlugin)
#endif // !DESKTOP_APP_DISABLE_GTK_INTEGRATION
#endif // Q_OS_WIN | Q_OS_MAC | Q_OS_UNIX
#endif // !DESKTOP_APP_USE_PACKAGED

#if defined Q_OS_UNIX && !defined Q_OS_MAC
#if !defined DESKTOP_APP_USE_PACKAGED || defined DESKTOP_APP_USE_PACKAGED_LAZY
Q_IMPORT_PLUGIN(NimfInputContextPlugin)
#ifndef DESKTOP_APP_DISABLE_DBUS_INTEGRATION
Q_IMPORT_PLUGIN(QFcitxPlatformInputContextPlugin)
#ifndef DESKTOP_APP_DISABLE_X11_INTEGRATION
Q_IMPORT_PLUGIN(QFcitx5PlatformInputContextPlugin)
#endif // !DESKTOP_APP_DISABLE_X11_INTEGRATION
Q_IMPORT_PLUGIN(QHimePlatformInputContextPlugin)
#endif // !DESKTOP_APP_DISABLE_DBUS_INTEGRATION
#endif // !DESKTOP_APP_USE_PACKAGED || DESKTOP_APP_USE_PACKAGED_LAZY
#endif // Q_OS_UNIX && !Q_OS_MAC
