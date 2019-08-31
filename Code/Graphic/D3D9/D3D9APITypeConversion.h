/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_API_TYPE_CONVERSION_H
#define D3D9_API_TYPE_CONVERSION_H


#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>
#include <Graphic/Core/APITypes.h>


namespace env
{
	D3DPRIMITIVETYPE ConvertToD3D9PrimitiveType(PRIMITIVE_TYPE type);

	void ConvertToD3D9SamplerStateType(SAMPLERSTATE_TYPE type, unsigned long value, D3DSAMPLERSTATETYPE& d3d9Sampler, unsigned long& d3d9Value);

    D3DRENDERSTATETYPE ConvertToD3D9RenderStateType(RENDERSTATE_TYPE rs);
    unsigned long ConvertToD3D9RenderStateValue(RENDERSTATE_TYPE rs, unsigned long value);

	D3DPOOL ConvertToD3D9Pool(MEMORY_TYPE mem);

	D3DFORMAT ConvertToD3D9Format(TEXTUREFORMAT_TYPE mem);

	unsigned long ConvertToD3D9Usage(unsigned long usage);

	unsigned long ConvertToD3D9Lock(unsigned long lock);

	unsigned long ConvertToD3D9ClearType(unsigned long type);

	D3DTRANSFORMSTATETYPE ConvertToD3D9Transform(TRANSFORM_TYPE type);

	ERROR_TYPE convertFromD3D9ErrorType(unsigned long d3d9Error);

	D3DXMATRIX convertToD3D9Matrix(const Matrix4x4&);

	Matrix4x4 convertFromD3D9Matrix(const D3DXMATRIX&);
	
} // env

#endif // D3D9_API_TYPE_CONVERSION_H
#endif // D3D9