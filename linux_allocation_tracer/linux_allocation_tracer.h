/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
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
