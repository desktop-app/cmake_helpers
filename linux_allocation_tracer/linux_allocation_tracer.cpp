// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "linux_allocation_tracer.h"

#include <atomic>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <malloc.h>
#include <dlfcn.h>
#include <stdio.h>

namespace {

std::atomic<void(*)(size_t, void *)> MallocLogger;
std::atomic<void(*)(size_t, void *)> VallocLogger;
std::atomic<void(*)(size_t, void *)> PVallocLogger;
std::atomic<void(*)(size_t, size_t, void *)> CallocLogger;
std::atomic<void(*)(void *, size_t, void *)> ReallocLogger;
std::atomic<void(*)(size_t, size_t, void *)> MemAlignLogger;
std::atomic<void(*)(size_t, size_t, void *)> AlignedAllocLogger;
std::atomic<void(*)(size_t, size_t, void *)> PosixMemAlignLogger;
std::atomic<void(*)(void *)> FreeLogger;

} // namespace

extern "C" {

//void *__real___malloc(size_t size);
void *__real___libc_malloc(size_t size);

void *__real_malloc(size_t size);
void *__real_valloc(size_t size);
void *__real_pvalloc(size_t size);
void *__real_calloc(size_t num, size_t size);
void *__real_realloc(void *ptr, size_t size);
void *__real_memalign(size_t alignment, size_t size);
void *__real_aligned_alloc(size_t alignment, size_t size);
int __real_posix_memalign(void **memptr, size_t alignment, size_t size);
void __real_free(void *ptr);

// void *__wrap___malloc(size_t size) {
//     const auto result = __real___malloc(size);
//     if (const auto logger = MallocLogger.load()) {
//         logger(size, result);
//     }
//     return result;
// }

void *__wrap___libc_malloc(size_t size) {
    const auto result = __real___libc_malloc(size);
    if (const auto logger = MallocLogger.load()) {
        logger(size, result);
    }
    return result;
}

void *__wrap_malloc(size_t size) {
    const auto result = __real_malloc(size);
    if (const auto logger = MallocLogger.load()) {
        logger(size, result);
    }
    return result;
}

void *__wrap_valloc(size_t size) {
    const auto result = __real_valloc(size);
    if (const auto logger = VallocLogger.load()) {
        logger(size, result);
    }
    return result;
}

void *__wrap_pvalloc(size_t size) {
    const auto result = __real_pvalloc(size);
    if (const auto logger = PVallocLogger.load()) {
        logger(size, result);
    }
    return result;
}

void *__wrap_calloc(size_t num, size_t size) {
    const auto result = __real_calloc(num, size);
    if (const auto logger = CallocLogger.load()) {
        logger(num, size, result);
    }
    return result;
}

void *__wrap_realloc(void *ptr, size_t size) {
    const auto result = __real_realloc(ptr, size);
    if (const auto logger = ReallocLogger.load()) {
        logger(ptr, size, result);
    }
    return result;
}

void *__wrap_memalign(size_t alignment, size_t size) {
    const auto result = __real_memalign(alignment, size);
    if (const auto logger = MemAlignLogger.load()) {
        logger(alignment, size, result);
    }
    return result;
}

void *__wrap_aligned_alloc(size_t alignment, size_t size) {
    const auto result = __real_aligned_alloc(alignment, size);
    if (const auto logger = AlignedAllocLogger.load()) {
        logger(alignment, size, result);
    }
    return result;
}

int __wrap_posix_memalign(void **memptr, size_t alignment, size_t size) {
    const auto result = __real_posix_memalign(memptr, alignment, size);
    if (!result) {
        if (const auto logger = PosixMemAlignLogger.load()) {
            logger(alignment, size, *memptr);
        }
    }
    return result;
}

void __wrap_free(void *ptr) {
    if (const auto logger = FreeLogger.load()) {
        logger(ptr);
    }
    __real_free(ptr);
}

} // extern "C"

void SetMallocLogger(void (*logger)(size_t, void *)) {
    MallocLogger = logger;
}

void SetVallocLogger(void (*logger)(size_t, void *)) {
    VallocLogger = logger;
}

void SetPVallocLogger(void (*logger)(size_t, void *)) {
    PVallocLogger = logger;
}

void SetCallocLogger(void (*logger)(size_t, size_t, void *)) {
    CallocLogger = logger;
}

void SetReallocLogger(void (*logger)(void *, size_t, void *)) {
    ReallocLogger = logger;
}

void SetMemAlignLogger(void (*logger)(size_t, size_t, void *)) {
    MemAlignLogger = logger;
}

void SetAlignedAllocLogger(void (*logger)(size_t, size_t, void *)) {
    AlignedAllocLogger = logger;
}

void SetPosixMemAlignLogger(void (*logger)(size_t, size_t, void *)) {
    PosixMemAlignLogger = logger;
}

void SetFreeLogger(void (*logger)(void *)) {
    FreeLogger = logger;
}
