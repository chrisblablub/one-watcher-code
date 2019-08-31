/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#ifdef _MSC_VER
#pragma once
#endif

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP) && (_WIN32_WINNT <= _WIN32_WINNT_WIN8)
#error WIC is not supported on Windows Phone 8.0
#endif

#include <d3d11_1.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

#if defined(_MSC_VER) && (_MSC_VER<1610) && !defined(_In_reads_)
#define _In_reads_(exp)
#define _Out_writes_(exp)
#define _In_reads_bytes_(exp)
#endif

#ifndef _Use_decl_annotations_
#define _Use_decl_annotations_
#endif

namespace DirectX
{
    // Standard version
    HRESULT CreateWICTextureFromMemory(_In_ ID3D11Device* d3dDevice,
                                       _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                       _In_ size_t wicDataSize,
                                       _Out_opt_ ID3D11Resource** texture,
                                       _Out_opt_ ID3D11ShaderResourceView** textureView,
                                       _In_ size_t maxsize = 0
                                       );

    HRESULT CreateWICTextureFromFile(_In_ ID3D11Device* d3dDevice,
                                     _In_z_ const wchar_t* szFileName,
                                     _Out_opt_ ID3D11Resource** texture,
                                     _Out_opt_ ID3D11ShaderResourceView** textureView,
                                     _In_ size_t maxsize = 0
                                     );

    // Standard version with optional auto-gen mipmap support
    HRESULT CreateWICTextureFromMemory(_In_ ID3D11Device* d3dDevice,
                                       _In_opt_ ID3D11DeviceContext* d3dContext,
                                       _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                       _In_ size_t wicDataSize,
                                       _Out_opt_ ID3D11Resource** texture,
                                       _Out_opt_ ID3D11ShaderResourceView** textureView,
                                       _In_ size_t maxsize = 0
                                       );

    HRESULT CreateWICTextureFromFile(_In_ ID3D11Device* d3dDevice,
                                     _In_opt_ ID3D11DeviceContext* d3dContext,
                                     _In_z_ const wchar_t* szFileName,
                                     _Out_opt_ ID3D11Resource** texture,
                                     _Out_opt_ ID3D11ShaderResourceView** textureView,
                                     _In_ size_t maxsize = 0
                                     );

    // Extended version
    HRESULT CreateWICTextureFromMemoryEx(_In_ ID3D11Device* d3dDevice,
                                         _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                         _In_ size_t wicDataSize,
                                         _In_ size_t maxsize,
                                         _In_ D3D11_USAGE usage,
                                         _In_ unsigned int bindFlags,
                                         _In_ unsigned int cpuAccessFlags,
                                         _In_ unsigned int miscFlags,
                                         _In_ bool forceSRGB,
                                         _Out_opt_ ID3D11Resource** texture,
                                         _Out_opt_ ID3D11ShaderResourceView** textureView
                                         );

    HRESULT CreateWICTextureFromFileEx(_In_ ID3D11Device* d3dDevice,
                                       _In_z_ const wchar_t* szFileName,
                                       _In_ size_t maxsize,
                                       _In_ D3D11_USAGE usage,
                                       _In_ unsigned int bindFlags,
                                       _In_ unsigned int cpuAccessFlags,
                                       _In_ unsigned int miscFlags,
                                       _In_ bool forceSRGB,
                                       _Out_opt_ ID3D11Resource** texture,
                                       _Out_opt_ ID3D11ShaderResourceView** textureView
                                       );

    // Extended version with optional auto-gen mipmap support
    HRESULT CreateWICTextureFromMemoryEx(_In_ ID3D11Device* d3dDevice,
                                         _In_opt_ ID3D11DeviceContext* d3dContext,
                                         _In_reads_bytes_(wicDataSize) const uint8_t* wicData,
                                         _In_ size_t wicDataSize,
                                         _In_ size_t maxsize,
                                         _In_ D3D11_USAGE usage,
                                         _In_ unsigned int bindFlags,
                                         _In_ unsigned int cpuAccessFlags,
                                         _In_ unsigned int miscFlags,
                                         _In_ bool forceSRGB,
                                         _Out_opt_ ID3D11Resource** texture,
                                         _Out_opt_ ID3D11ShaderResourceView** textureView
                                         );

    HRESULT CreateWICTextureFromFileEx(_In_ ID3D11Device* d3dDevice,
                                       _In_opt_ ID3D11DeviceContext* d3dContext,
                                       _In_z_ const wchar_t* szFileName,
                                       _In_ size_t maxsize,
                                       _In_ D3D11_USAGE usage,
                                       _In_ unsigned int bindFlags,
                                       _In_ unsigned int cpuAccessFlags,
                                       _In_ unsigned int miscFlags,
                                       _In_ bool forceSRGB,
                                       _Out_opt_ ID3D11Resource** texture,
                                       _Out_opt_ ID3D11ShaderResourceView** textureView
                                       );
}

#endif // ENV_GRAPHIC_API_D3D11
