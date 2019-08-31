/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_RENDER_DEVICE_H
#define D3D9_RENDER_DEVICE_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/APITypes.h>
#include <Graphic/D3D9/D3D9Config.h>
#include <Graphic/D3D9/D3D9VertexDeclaration.h>
#include <Graphic/D3D9/D3D9VertexBuffer.h>
#include <Graphic/D3D9/D3D9IndexBuffer.h>
#include <Graphic/D3D9/D3D9TextureManaged.h>
#include <Graphic/D3D9/D3D9Font.h>
#include <Graphic/D3D9/D3D9Shader.h>
#include <Graphic/Core/RenderTarget.h>

namespace env
{
    typedef std::vector< D3DDISPLAYMODE > vecDisplayModes;

    class CD3D9RenderDevice
    {
    public:
        typedef std::pair< bool, unsigned long >    pairFlagSamplerState;
        typedef std::pair< bool, unsigned long >    pairFlagRenderState;

        typedef std::vector< pairFlagSamplerState > vecSamplerStates;
        typedef std::vector< vecSamplerStates >     vecBoundSamplerStates;
        typedef std::vector< CTextureBase* >        vecBoundTextures;
        typedef std::vector< CVertexBuffer* >       vecBoundVertexBuffers;
        typedef std::vector< CIndexBuffer* >        vecBoundIndexBuffers;
        typedef std::vector< CRenderTarget* >       vecBoundRenderTargets;
        
        typedef std::vector< pairFlagRenderState >  vecBoundRenderStates;
        
        CD3D9RenderDevice();
        virtual ~CD3D9RenderDevice();

        virtual bool Initialize(const RenderSystemParams& renderSysParams, int screenWidth, int screenHeight, int adapater, bool bVSync, bool bWindowed);
        virtual void Reset(int screenWidth, int screenHeight, bool bVSync, bool bWindowed);
        virtual void OnLostDevice();
        virtual void OnResetDevice(int screenWidth, int screenHeight);
        virtual void BeginScene();
        virtual void EndScene();
        virtual bool CheckDeviceSettings();
        virtual bool EnumerateDisplayModes(vecDisplayModes& modes);
        virtual void ResetToDefaultRenderStates() {}
        virtual void SetFVF(unsigned long fvf);

        virtual void DrawPrimitive(PRIMITIVE_TYPE primitiveType, unsigned int startVertex, unsigned int primitiveCount);
        virtual void DrawIndexedPrimitive(PRIMITIVE_TYPE primitiveType, int baseVertexIndex, unsigned int minIndex, unsigned int numVertices,
            unsigned int startIndex, unsigned int primitiveCount);

        virtual CVertexDeclaration* CreateVertexDeclaration(vecVertexDeclarationElements& vertexDeclarationElements, unsigned int stride, CShader* pShader);
        virtual CVertexBuffer* CreateVertexBuffer(unsigned int length, unsigned long usage, MEMORY_TYPE pool);
        virtual CIndexBuffer* CreateIndexBuffer(unsigned int length, unsigned long usage, TEXTUREFORMAT_TYPE format, MEMORY_TYPE pool);
        CTextureBase* CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned long usage,
            TEXTUREFORMAT_TYPE format, MEMORY_TYPE memory_pool);
        CTextureManaged* CreateTexture();
        CRenderTarget* CreateRenderTarget(unsigned int width, unsigned int height, TEXTUREFORMAT_TYPE format, bool bMipMaps = false);
        CFont* CreateD3DFont(const std::string& strFontFamily, bool bItalic, unsigned int weight, int size, int sizeMin, int sizeMax);
        CShader* CreateShader();

        virtual void Clear(const Vec4& ClearColor, unsigned long Buffers, float z = 0.0f, unsigned long stencil = 0);
        virtual CRenderTarget* GetBackBuffer() { return m_pBackBuffer; }
        virtual void SetTransform(TRANSFORM_TYPE transformType, const Matrix4x4& transformMatrix);
        virtual void GetTransform(TRANSFORM_TYPE transformType, Matrix4x4& transformMatrix);
        virtual void SetRenderState(RENDERSTATE_TYPE rs, unsigned long value);
        virtual void SetSamplerState(unsigned long Sampler, SAMPLERSTATE_TYPE Type, unsigned long Value);
        virtual void SetTexture(unsigned long index, CTextureBase* pTexture);
        virtual void SetTextureSampler(unsigned long index, const CTextureSampler& sampler);
        virtual void SetVertexBuffer(unsigned int streamNumber, unsigned int offset, CVertexBuffer* pVertexBuffer);
        virtual void SetVertexDeclaration(CVertexDeclaration* pVertexDeclaration);
        virtual void SetShader(CShader* pShader);
        virtual void SetIndexBuffer(CIndexBuffer* pIndexBuffer);
        virtual void SetRenderTarget(unsigned long index, CRenderTarget* pRenderTarget);
        virtual void SetDepthStencilTarget(CRenderTarget* pRenderTarget);
        virtual void SetViewport(const Vec2& pos, unsigned int width, unsigned int height) {}

        virtual void BeginPerformanceMarker(const std::string& strName) {}
        virtual void EndPerformanceMarker() {}

        IDirect3DDevice9* GetDevice() const { return m_pDevice; }

        ERROR_TYPE TestCooperativeLevel();
        
    private:

        IDirect3DDevice9* m_pDevice;
        IDirect3D9* m_pD3D;

        int m_adapter;

        bool m_bInStateBlock;

        D3DPRESENT_PARAMETERS m_D3DPresentParams;

        vecBoundTextures m_boundTextures;
        vecBoundRenderStates m_boundRenderStates;
        vecBoundSamplerStates m_boundSamplerStates;
        vecBoundVertexBuffers m_boundVertexBuffers;
        vecBoundRenderTargets m_boundRenderTargets;

        CIndexBuffer* m_pBoundIndexBuffer;

        CVertexDeclaration* m_pVertexDeclaration;

        CRenderTarget* m_pBackBuffer;

        CShader* m_pShader;
    };

    typedef CD3D9RenderDevice CRenderDevice;

} // env
#endif // D3D9_RENDER_DEVICE_H 
#endif // D3D9