// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <cstdlib>

void SetMallocLogger(void (*logger)(size_t, void *));
void SetVallocLogger(void (*logger)(size_t, void *));
void SetPVallocLogger(void (*logger)(size_t, void *));
void SetCallocLogger(void (*logger)(size_t, size_t, void *));
void SetReallocLogger(void (*logger)(void *, size_t, void *));
void SetMemAlignLogger(void (*logger)(size_t, size_t, void *));
void SetAlignedAllocLogger(void (*logger)(size_t, size_t, void *));
void SetPosixMemAlignLogger(void (*logger)(size_t, size_t, void *));
void SetFreeLogger(void (*logger)(void *));
