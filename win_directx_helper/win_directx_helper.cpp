// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//

#include <d3d9.h>
#include <d3d11.h>
#include <string>

#define LOAD_SYMBOL(handle, func) LoadSymbol(handle, #func, func)

namespace DirectX {
namespace {

using Handle = HINSTANCE;

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

bool ResolveD3D9() {
	static const auto loaded = [] {
		const auto d3d9 = LoadLibrary(L"d3d9.dll");
		LOAD_SYMBOL(d3d9, D3DPERF_BeginEvent);
		LOAD_SYMBOL(d3d9, D3DPERF_EndEvent);
		LOAD_SYMBOL(d3d9, D3DPERF_SetMarker);
		LOAD_SYMBOL(d3d9, D3DPERF_GetStatus);
		return true
			&& LOAD_SYMBOL(d3d9, Direct3DCreate9);
	}();
	return loaded;
}

bool ResolveD3D11() {
	static const auto loaded = [] {
		const auto d3d11 = LoadLibrary(L"d3d11.dll");
		return true
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
