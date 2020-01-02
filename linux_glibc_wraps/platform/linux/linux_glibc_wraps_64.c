// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include <time.h>

#if defined(_M_X64) || defined(__x86_64__)
#define GETTIME_GLIBC_VERSION "2.2.5"
#elif defined(__aarch64__)
#define GETTIME_GLIBC_VERSION "2.17"
#else
#error Please add glibc wraps for your architecture
#endif

typedef unsigned int UTItype __attribute__ ((mode (TI)));
typedef int TItype __attribute__ ((mode (TI)));

int __clock_gettime_glibc_old(clockid_t clk_id, struct timespec *tp);
__asm__(".symver __clock_gettime_glibc_old,clock_gettime@GLIBC_" GETTIME_GLIBC_VERSION);


int __wrap_clock_gettime(clockid_t clk_id, struct timespec *tp) {
	return __clock_gettime_glibc_old(clk_id, tp);
}

UTItype __udivmodti4(UTItype num, UTItype den, UTItype *rem_p);

TItype __wrap___divmodti4(TItype num, TItype den, TItype *rem_p) {
	int minus = 0;
	TItype v;

	if (num < 0) {
		num = -num;
		minus = 1;
	}
	if (den < 0) {
		den = -den;
		minus ^= 1;
	}

	v = __udivmodti4(num, den, (UTItype *)rem_p);
	if (minus) {
		v = -v;
		if (rem_p)
			*rem_p = -(*rem_p);
	}

	return v;
}

