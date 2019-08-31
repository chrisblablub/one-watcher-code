/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_API_TYPE_CONVERSION_H
#define D3D11_API_TYPE_CONVERSION_H


#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/Core/APITypes.h>


namespace env
{

    unsigned long ConvertToD3D11RenderState(RENDERSTATE_TYPE renderStateType, unsigned long renderState);
    unsigned long ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE samplerStateType, unsigned long samplerState);
    unsigned long ConvertToD3D11SamplerStateFilter(unsigned long minFilter, unsigned long magFilter, unsigned long mipFilter);
    unsigned long ConvertFromD3D11(const D3D11_FILL_MODE& d3d11FillMode);
    unsigned long ConvertFromD3D11(const D3D11_CULL_MODE& d3d11CullMode);
    TEXTUREFORMAT_TYPE ConvertFromD3D11(DXGI_FORMAT mem);
    unsigned long ConvertToD3D11ClearType(unsigned long type);

    DXGI_FORMAT ConvertToD3D11Format(TEXTUREFORMAT_TYPE mem);

    D3D11_USAGE ConvertToD3D11Usage(unsigned long usage);

    D3D11_MAP ConvertToD3D11Map(unsigned long lockFlags);
} // env

#endif // D3D9_API_TYPE_CONVERSION_H
#endif // ENV_GRAPHIC_API_D3D11
