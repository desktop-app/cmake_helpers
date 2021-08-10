// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//

#include <d3d9.h>
#include <d3d11.h>
#include <d3dcompiler.h>
extern "C" {
#include <openssl/sha.h>
} // extern "C"
#include <string>
#include <vector>
#include <array>

#define LOAD_SYMBOL(handle, func) LoadSymbol(handle, #func, func)

namespace DirectX {
namespace {

constexpr auto kMaxPathLong = 32767;

using Handle = HINSTANCE;

// d3dcompiler_47.dll

HRESULT (__stdcall *D3DCompile)(
	LPCVOID pSrcData,
	SIZE_T SrcDataSize,
	LPCSTR pFileName,
	CONST D3D_SHADER_MACRO* pDefines,
	ID3DInclude* pInclude,
	LPCSTR pEntrypoint,
	LPCSTR pTarget,
	UINT Flags1,
	UINT Flags2,
	ID3DBlob** ppCode,
	ID3DBlob** ppErrorMsgs);

HRESULT (__stdcall *D3DDisassemble)(
	_In_reads_bytes_(SrcDataSize) LPCVOID pSrcData,
	_In_ SIZE_T SrcDataSize,
	_In_ UINT Flags,
	_In_opt_ LPCSTR szComments,
	_Out_ ID3DBlob** ppDisassembly);

// d3d9.dll

IDirect3D9 * (__stdcall *Direct3DCreate9)(UINT SDKVersion);
int (__stdcall *D3DPERF_BeginEvent)(D3DCOLOR col, LPCWSTR wszName);
int (__stdcall *D3DPERF_EndEvent)(void);
void (__stdcall *D3DPERF_SetMarker)(D3DCOLOR col, LPCWSTR wszName);
DWORD (__stdcall *D3DPERF_GetStatus)(void);

// d3d11.dll

HRESULT (__stdcall *D3D11CreateDevice)(
	_In_opt_ IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	_In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	_COM_Outptr_opt_ ID3D11Device** ppDevice,
	_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
	_COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext);

// dxgi.dll

HRESULT (__stdcall *CreateDXGIFactory)(
	REFIID riid,
	_COM_Outptr_ void **ppFactory);

HRESULT (__stdcall *CreateDXGIFactory1)(
	REFIID riid,
	_COM_Outptr_ void **ppFactory);

template <typename Function>
inline bool LoadSymbol(Handle handle, const char *name, Function &func) {
	func = handle
		? reinterpret_cast<Function>(GetProcAddress(handle, name))
		: nullptr;
	return (func != nullptr);
}

// For win_directx_helper.
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
	auto index = 0;
	for (const auto byte : binary) {
		result.push_back(hex(byte / 16));
		result.push_back(hex(byte % 16));
	}
	return result;
}

bool ResolveD3DCompiler(const wchar_t *path) {
	const auto d3dcompiler = LoadLibrary(path);
	return true
		&& LOAD_SYMBOL(d3dcompiler, D3DCompile)
		&& LOAD_SYMBOL(d3dcompiler, D3DDisassemble);
}

bool ResolveD3DCompiler() {
	static const auto loaded = [] {
#ifdef DESKTOP_APP_D3DCOMPILER_HASH
		auto exePath = std::array<WCHAR, kMaxPathLong + 1>{ 0 };
		const auto exeLength = GetModuleFileName(
			nullptr,
			exePath.data(),
			kMaxPathLong + 1);
		if (!exeLength || exeLength >= kMaxPathLong + 1) {
			return false;
		}
		const auto exe = std::wstring(exePath.data());
		const auto last1 = exe.find_last_of('\\');
		const auto last2 = exe.find_last_of('/');
		const auto last = std::max(
			(last1 == std::wstring::npos) ? -1 : int(last1),
			(last2 == std::wstring::npos) ? -1 : int(last2));
		if (last < 0) {
			return false;
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
		const auto hash = DESKTOP_APP_STRINGIFY(DESKTOP_APP_D3DCOMPILER_HASH);
#undef DESKTOP_APP_STRINGIFY
#undef DESKTOP_APP_STRINGIFY2

		const auto compiler = exe.substr(0, last + 1)
			+ L"modules\\" + arch + L"\\d3d\\d3dcompiler_47.dll";
		const auto path = compiler.c_str();
		if (FileSha256(path) == hash && ResolveD3DCompiler(path)) {
			return true;
		}
#elif defined DESKTOP_APP_SPECIAL_TARGET // DESKTOP_APP_D3DCOMPILER_HASH
#error "Special target build should have d3dcompiler hash."
#endif // !DESKTOP_APP_D3DCOMPILER_HASH && DESKTOP_APP_SPECIAL_TARGET

		return ResolveD3DCompiler(L"d3dcompiler_47.dll");
	}();
	return loaded;
}

bool ResolveD3D9() {
	static const auto loaded = [] {
		const auto d3d9 = LoadLibrary(L"d3d9.dll");
		LOAD_SYMBOL(d3d9, D3DPERF_BeginEvent);
		LOAD_SYMBOL(d3d9, D3DPERF_EndEvent);
		LOAD_SYMBOL(d3d9, D3DPERF_SetMarker);
		LOAD_SYMBOL(d3d9, D3DPERF_GetStatus);
		return ResolveD3DCompiler()
			&& LOAD_SYMBOL(d3d9, Direct3DCreate9);
	}();
	return loaded;
}

bool ResolveD3D11() {
	static const auto loaded = [] {
		const auto d3d11 = LoadLibrary(L"d3d11.dll");
		return ResolveD3DCompiler()
			&& LOAD_SYMBOL(d3d11, D3D11CreateDevice);
	}();
	return loaded;
}

bool ResolveDXGI() {
	static const auto loaded = [&] {
		const auto dxgi = LoadLibrary(L"dxgi.dll");
		LOAD_SYMBOL(dxgi, CreateDXGIFactory1);
		return true
			&& LOAD_SYMBOL(dxgi, CreateDXGIFactory);
	}();
	return loaded;
}

} // namespace
} // namespace DirectX

bool DirectXResolveCompiler() {
	return DirectX::ResolveD3DCompiler();
}

namespace D = DirectX;

extern "C" {

IDirect3D9 * WINAPI Direct3DCreate9(UINT SDKVersion) {
	return D::ResolveD3D9()
		? D::Direct3DCreate9(SDKVersion)
		: nullptr;
}

int WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName) {
	return (D::ResolveD3D9() && D::D3DPERF_BeginEvent)
		? D::D3DPERF_BeginEvent(col, wszName)
		: -1;
}

int WINAPI D3DPERF_EndEvent(void) {
	return (D::ResolveD3D9() && D::D3DPERF_EndEvent)
		? D::D3DPERF_EndEvent()
		: -1;
}

void WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName) {
	if (D::ResolveD3D9() && D::D3DPERF_SetMarker) {
		D::D3DPERF_SetMarker(col, wszName);
	}
}

DWORD WINAPI D3DPERF_GetStatus(void) {
	return (D::ResolveD3D9() && D::D3DPERF_GetStatus)
		? D::D3DPERF_GetStatus()
		: 0;
}

HRESULT WINAPI D3D11CreateDevice(
		_In_opt_ IDXGIAdapter* pAdapter,
		D3D_DRIVER_TYPE DriverType,
		HMODULE Software,
		UINT Flags,
		_In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
		UINT FeatureLevels,
		UINT SDKVersion,
		_COM_Outptr_opt_ ID3D11Device** ppDevice,
		_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
		_COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext) {
	return D::ResolveD3D11()
		? D::D3D11CreateDevice(
			pAdapter,
			DriverType,
			Software,
			Flags,
			pFeatureLevels,
			FeatureLevels,
			SDKVersion,
			ppDevice,
			pFeatureLevel,
			ppImmediateContext)
		: CO_E_DLLNOTFOUND;
}

HRESULT WINAPI CreateDXGIFactory(
		REFIID riid,
		_COM_Outptr_ void **ppFactory) {
	return D::ResolveDXGI()
		? D::CreateDXGIFactory(riid, ppFactory)
		: CO_E_DLLNOTFOUND;
}

HRESULT WINAPI CreateDXGIFactory1(
		REFIID riid,
		_COM_Outptr_ void **ppFactory) {
	return (D::ResolveDXGI() && D::CreateDXGIFactory1)
		? D::CreateDXGIFactory1(riid, ppFactory)
		: CO_E_DLLNOTFOUND;
}

} // extern "C"
