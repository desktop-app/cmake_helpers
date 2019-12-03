// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void *__wrap_aligned_alloc(size_t alignment, size_t size) {
	void *result = NULL;
	return (posix_memalign(&result, alignment, size) == 0)
		? result
		: NULL;
}

int enable_secure_inited = 0;
int enable_secure = 1;

char *__wrap_secure_getenv(const char *name) {
	if (enable_secure_inited == 0) {
		enable_secure_inited = 1;
		enable_secure = (geteuid() != getuid())
		|| (getegid() != getgid());
	}
	return enable_secure ? NULL : getenv(name);
}

