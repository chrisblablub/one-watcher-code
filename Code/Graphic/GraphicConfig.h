/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GRAPHIC_CONFIG_H
#define GRAPHIC_CONFIG_H


#define MAKE_INCLUDE(x) <##x##>

#ifdef D3D9
#include <Graphic/D3D9/D3D9APITypeConversion.h>
#include <Graphic/D3D9/D3D9Font.h>
#include <Graphic/D3D9/D3D9IndexBuffer.h>
#include <Graphic/D3D9/D3D9RenderTarget.h>
#include <Graphic/D3D9/D3D9TextureManaged.h>
#include <Graphic/D3D9/D3D9VertexBuffer.h>
#include <Graphic/D3D9/D3D9VertexDeclaration.h>
#include <Graphic/D3D9/D3D9RenderDevice.h>
#include <Graphic/D3D9/D3D9GPUProfiler.h>
#endif

#ifdef ENV_GRAPHIC_API_D3D11
#define GRAPHIC_API_TYPECONVERSION      MAKE_INCLUDE(Graphic/D3D11/D3D11APITypeConversion.h)
#define GRAPHIC_API_FONT                MAKE_INCLUDE(Graphic/D3D11/D3D11BitmapFont.h)
#define GRAPHIC_API_INDEXBUFFER         MAKE_INCLUDE(Graphic/D3D11/D3D11IndexBuffer.h)
#define GRAPHIC_API_RENDERTARGET        MAKE_INCLUDE(Graphic/D3D11/D3D11RenderTarget.h)
#define GRAPHIC_API_TEXTURE             MAKE_INCLUDE(Graphic/D3D11/D3D11TextureManaged.h)
#define GRAPHIC_API_VERTEXBUFFER        MAKE_INCLUDE(Graphic/D3D11/D3D11VertexBuffer.h)
#define GRAPHIC_API_VERTEXDECLARATION   MAKE_INCLUDE(Graphic/D3D11/D3D11VertexDeclaration.h)
#define GRAPHIC_API_RENDERDEVICE        MAKE_INCLUDE(Graphic/D3D11/D3D11RenderDevice.h)
#define GRAPHIC_API_GPUPROFILER         MAKE_INCLUDE(Graphic/D3D11/D3D11GPUProfiler.h)
#define GRAPHIC_API_SHADER              MAKE_INCLUDE(Graphic/D3D11/D3D11Shader.h)
#endif // ENV_GRAPHIC_API_D3D11

#ifdef ENV_GRAPHIC_API_GNM
#define GRAPHIC_API_TYPECONVERSION      "Graphic/GNM/GNMAPITypeConversion.h"
#define GRAPHIC_API_FONT                "Graphic/GNM/GNMBitmapFont.h"
#define GRAPHIC_API_INDEXBUFFER         "Graphic/GNM/GNMIndexBuffer.h"
#define GRAPHIC_API_RENDERTARGET        "Graphic/GNM/GNMRenderTarget.h"
#define GRAPHIC_API_TEXTURE             "Graphic/GNM/GNMTextureManaged.h"
#define GRAPHIC_API_VERTEXBUFFER        "Graphic/GNM/GNMVertexBuffer.h"
#define GRAPHIC_API_VERTEXDECLARATION   "Graphic/GNM/GNMVertexDeclaration.h"
#define GRAPHIC_API_RENDERDEVICE        "Graphic/GNM/GNMRenderDevice.h"
#define GRAPHIC_API_GPUPROFILER         "Graphic/GNM/GNMGPUProfiler.h"
#define GRAPHIC_API_SHADER              "Graphic/GNM/GNMShader.h"
#endif // ENV_GRAPHIC_API_GNM


#define GRAPHIC_RENDERSYSTEM            "Graphic/RenderSystems/DeferredRenderSystem.h"

#endif // GRAPHIC_CONFIG_H