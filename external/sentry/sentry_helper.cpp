// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//

#include <windows.h>
extern "C" {
#include <openssl/sha.h>
} // extern "C"
#include <string>
#include <vector>
#include <array>

namespace Sentry {
namespace {

constexpr auto kMaxPathLong = 32767;

// For sentry_helper.
std::string FileSha256(const wchar_t *path) {
	using uchar = unsigned char;
	constexpr auto kLimit = 10 * 1024 * 1024;
	auto buffer = std::vector<uchar>(kLimit);
	auto size = DWORD();

	const auto file = CreateFile(
		path,
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);
	if (file == INVALID_HANDLE_VALUE) {
		return {};
	}
	const auto read = ReadFile(file, buffer.data(), kLimit, &size, nullptr);
	CloseHandle(file);

	if (!read || !size || size >= kLimit) {
		return {};
	}
	auto binary = std::array<uchar, SHA256_DIGEST_LENGTH>{};
	SHA256(buffer.data(), size, binary.data());
	const auto hex = [](uchar value) {
		return (value >= 10) ? ('a' + (value - 10)) : ('0' + value);
	};
	auto result = std::string();
	result.reserve(binary.size() * 2);
	for (const auto byte : binary) {
		result.push_back(hex(byte / 16));
		result.push_back(hex(byte % 16));
	}
	return result;
}

} // namespace

std::wstring ResolveCrashpadHandler() {
	static const auto path = [] {
#ifdef DESKTOP_APP_CRASHPAD_HANDLER_HASH
		auto exePath = std::array<WCHAR, kMaxPathLong + 1>{ { 0 } };
		const auto exeLength = GetModuleFileName(
			nullptr,
			exePath.data(),
			kMaxPathLong + 1);
		if (!exeLength || exeLength >= kMaxPathLong + 1) {
			return std::wstring();
		}
		const auto exe = std::wstring(exePath.data());
		const auto last1 = exe.find_last_of('\\');
		const auto last2 = exe.find_last_of('/');
		const auto last = std::max(
			(last1 == std::wstring::npos) ? -1 : int(last1),
			(last2 == std::wstring::npos) ? -1 : int(last2));
		if (last < 0) {
			return std::wstring();
		}

#if defined _WIN64
		const auto arch = L"x64";
#elif defined _WIN32 // _WIN64
		const auto arch = L"x86";
#else // _WIN64 || _WIN32
#error "Invalid configuration."
#endif // _WIN64 || _WIN32

#define DESKTOP_APP_STRINGIFY2(x) #x
#define DESKTOP_APP_STRINGIFY(x) DESKTOP_APP_STRINGIFY2(x)
		const auto hash = DESKTOP_APP_STRINGIFY(DESKTOP_APP_CRASHPAD_HANDLER_HASH);
#undef DESKTOP_APP_STRINGIFY
#undef DESKTOP_APP_STRINGIFY2

		const auto handler = exe.substr(0, last + 1)
			+ L"modules\\" + arch + L"\\crashpad\\crashpad_handler.exe";
		const auto path = handler.c_str();
		return (FileSha256(path) == hash) ? path : std::wstring();
#elif defined SENTRY_HELPER_SPECIAL_TARGET // DESKTOP_APP_CRASHPAD_HANDLER_HASH
#error "Special target build should have crashpad_handler hash."
#else
		return std::wstring();
#endif // !DESKTOP_APP_CRASHPAD_HANDLER_HASH && SENTRY_HELPER_SPECIAL_TARGET
	}();
	return path;
}

} // namespace Sentry
