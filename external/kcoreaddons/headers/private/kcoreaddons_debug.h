#pragma once
#include <QLoggingCategory>
static inline const QLoggingCategory &KCOREADDONS_DEBUG() {
    static const QLoggingCategory category("kf.coreaddons", QtInfoMsg);
    return category;
}
