/*
This file is part of Desktop App Toolkit,
a set of libraries for developing nice desktop applications.

For license and copyright information please follow this link:
https://github.com/desktop-app/legal/blob/master/LEGAL
*/
#include <QtCore/QtPlugin>

#ifndef DESKTOP_APP_USE_PACKAGED
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

#ifdef Q_OS_MAC
Q_IMPORT_PLUGIN(QSecureTransportBackend)
Q_IMPORT_PLUGIN(QSCNetworkReachabilityNetworkInformationBackendFactory)
#else // Q_OS_MAC
Q_IMPORT_PLUGIN(QTlsBackendOpenSSL)
#endif // Q_OS_MAC

#else // Qt >= 6.0.0
Q_IMPORT_PLUGIN(QGenericEnginePlugin)
#endif // Qt < 6.0.0

Q_IMPORT_PLUGIN(QWebpPlugin)
Q_IMPORT_PLUGIN(QJpegPlugin)
Q_IMPORT_PLUGIN(QGifPlugin)

#ifdef Q_OS_WIN
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif defined Q_OS_MAC // Q_OS_WIN
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#elif defined Q_OS_UNIX // Q_OS_WIN | Q_OS_MAC
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QXcbEglIntegrationPlugin)
Q_IMPORT_PLUGIN(QXcbGlxIntegrationPlugin)
Q_IMPORT_PLUGIN(QComposePlatformInputContextPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)
#ifdef QT_DBUS_LIB
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_IMPORT_PLUGIN(QConnmanEnginePlugin)
Q_IMPORT_PLUGIN(QNetworkManagerEnginePlugin)
#endif // Qt < 6.0.0
Q_IMPORT_PLUGIN(QIbusPlatformInputContextPlugin)
Q_IMPORT_PLUGIN(QXdgDesktopPortalThemePlugin)
#endif // QT_DBUS_LIB
#ifndef DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION
Q_IMPORT_PLUGIN(QWaylandIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandEglPlatformIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandEglClientBufferPlugin)
Q_IMPORT_PLUGIN(QWaylandXdgShellIntegrationPlugin)
Q_IMPORT_PLUGIN(QWaylandBradientDecorationPlugin)
#endif // !DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION
Q_IMPORT_PLUGIN(QGtk3ThemePlugin)
#endif // Q_OS_WIN | Q_OS_MAC | Q_OS_UNIX
#endif // !DESKTOP_APP_USE_PACKAGED

#if !defined DESKTOP_APP_USE_PACKAGED || defined DESKTOP_APP_USE_PACKAGED_LAZY
#if defined Q_OS_WIN || (defined Q_OS_UNIX && !defined Q_OS_MAC)
Q_IMPORT_PLUGIN(QAVIFPlugin)
Q_IMPORT_PLUGIN(HEIFPlugin)
Q_IMPORT_PLUGIN(QJpegXLPlugin)
#endif // Q_OS_WIN || (Q_OS_UNIX && !Q_OS_MAC)

#if defined Q_OS_UNIX && !defined Q_OS_MAC
Q_IMPORT_PLUGIN(NimfInputContextPlugin)
#ifdef QT_DBUS_LIB
Q_IMPORT_PLUGIN(QFcitxPlatformInputContextPlugin)
#ifndef DESKTOP_APP_DISABLE_X11_INTEGRATION
Q_IMPORT_PLUGIN(QFcitx5PlatformInputContextPlugin)
#endif // !DESKTOP_APP_DISABLE_X11_INTEGRATION
#endif // QT_DBUS_LIB
#ifndef DESKTOP_APP_DISABLE_X11_INTEGRATION
Q_IMPORT_PLUGIN(QHimePlatformInputContextPlugin)
#endif // !DESKTOP_APP_DISABLE_X11_INTEGRATION
#endif // Q_OS_UNIX && !Q_OS_MAC
#endif // !DESKTOP_APP_USE_PACKAGED || DESKTOP_APP_USE_PACKAGED_LAZY
