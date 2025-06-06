/*
This file is part of Desktop App Toolkit,
a set of libraries for developing nice desktop applications.

For license and copyright information please follow this link:
https://github.com/desktop-app/legal/blob/master/LEGAL
*/
#include <QtCore/QtPlugin>

#ifndef QT_STATIC_PLUGINS_DISABLE_AVIF
Q_IMPORT_PLUGIN(QAVIFPlugin)
#endif // !QT_STATIC_PLUGINS_DISABLE_AVIF
#ifndef QT_STATIC_PLUGINS_DISABLE_HEIF
Q_IMPORT_PLUGIN(HEIFPlugin)
#endif // !QT_STATIC_PLUGINS_DISABLE_HEIF
#ifndef QT_STATIC_PLUGINS_DISABLE_JXL
Q_IMPORT_PLUGIN(QJpegXLPlugin)
#endif // !QT_STATIC_PLUGINS_DISABLE_JXL
Q_IMPORT_PLUGIN(QOIPlugin)
