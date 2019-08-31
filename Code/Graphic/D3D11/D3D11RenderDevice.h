/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_RENDER_DEVICE_H
#define D3D11_RENDER_DEVICE_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/APITypes.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/D3D11/D3D11VertexDeclaration.h>
#include <Graphic/D3D11/D3D11VertexBuffer.h>
#include <Graphic/D3D11/D3D11IndexBuffer.h>
#include <Graphic/D3D11/D3D11TextureManaged.h>
#include <Graphic/D3D11/D3D11BitmapFont.h>
#include <Graphic/D3D11/D3D11Shader.h>
#include <Graphic/Core/RenderTarget.h>

namespace env
{
    struct DisplayMode
    {
        unsigned int       Width;
        unsigned int       Height;

        DXGI_MODE_DESC     Desc;
//         unsigned int       RefreshRate;
//         TEXTUREFORMAT_TYPE Format;
    };

    struct RenderSystemParams;

    typedef std::vector< DisplayMode > vecDisplayModes;

    class CRenderDevice
    {
    public:

        typedef std::vector< unsigned long >         vecSamplerStates;
        typedef std::pair< bool, vecSamplerStates >  pairFlagSamplerStates;
        typedef std::vector< pairFlagSamplerStates > vecBoundSamplerStates;
        typedef std::vector< CTextureBase* >         vecBoundTextures;
        typedef std::vector< CVertexBuffer* >        vecBoundVertexBuffers;
        typedef std::vector< CIndexBuffer* >         vecBoundIndexBuffers;
        typedef std::vector< CRenderTarget* >        vecBoundRenderTargets;
        typedef std::vector< unsigned long >         vecBoundRenderStates;

        CRenderDevice();
        ~CRenderDevice();

        void SetFullscreen(bool bFullscreen);
        bool GetFullscreen() const;
        void SetVSync(bool bSet) { m_bVSync = bSet; }
        void ResizeTarget(int w, int h);
        bool Initialize(const RenderSystemParams& renderSysParams, int screenWidth, int screenHeight, int adapater, bool bVSync, bool bWindowed);
        void Reset(int screenWidth, int screenHeight, bool bVSync, bool bWindowed);
        void OnLostDevice();
        void OnResetDevice(int screenWidth, int screenHeight);
        void BeginScene();
        void EndScene();
        bool CheckDeviceSettings();
        bool EnumerateDisplayModes(vecDisplayModes& modes);
        void FindClosestMatchingResolution(int& width, int& height, int requestedWidth, int requestedHeight);
        void ResetToDefaultRenderStates();

        void DrawPrimitive(PRIMITIVE_TYPE primitiveType, unsigned int startVertex, unsigned int primitiveCount);
        void DrawIndexedPrimitive(PRIMITIVE_TYPE primitiveType, int baseVertexIndex, unsigned int minIndex, unsigned int numVertices,
            unsigned int startIndex, unsigned int primitiveCount);

        std::unique_ptr<CVertexDeclaration> CreateVertexDeclaration(vecVertexDeclarationElements& vertexDeclarationElements, unsigned int stride, CShader* pShader);
        CVertexBuffer* CreateVertexBuffer(unsigned int length, unsigned long usage, void* pData = nullptr);
        CIndexBuffer* CreateIndexBuffer(unsigned int length, unsigned long usage, TEXTUREFORMAT_TYPE format, void* pData = nullptr);
        CTextureBase* CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned long usage,
            TEXTUREFORMAT_TYPE format, MEMORY_TYPE memory_pool);
        CTextureManaged* CreateTexture();
        std::unique_ptr<CRenderTarget> CreateRenderTarget(unsigned int width, unsigned int height, TEXTUREFORMAT_TYPE format, bool bCreateDepthStencil = false, bool bMipMaps = false);
        CFont* CreateD3DFont(const std::string& strFontFamily, bool bItalic, unsigned int weight, int size, int sizeMin, int sizeMax);
        CShader* CreateShader();

        void Clear(const Vec4& ClearColor, unsigned long Buffers, float z = 1.0f, unsigned long stencil = 0);
        CRenderTarget* GetBackBuffer() { return m_pBackBuffer; }
        void SetRenderState(RENDERSTATE_TYPE rs, unsigned long value);
        void SetSamplerState(unsigned long Sampler, SAMPLERSTATE_TYPE Type, unsigned long Value);
        void SetTexture(unsigned long index, CTextureBase* pTexture);
        void SetTextureSampler(unsigned long index, const CTextureSampler& sampler);
        void SetVertexBuffer(unsigned int streamNumber, unsigned int offset, CVertexBuffer* pVertexBuffer);
        void SetVertexDeclaration(CVertexDeclaration* pVertexDeclaration);
        void SetIndexBuffer(CIndexBuffer* pIndexBuffer);
        void SetShader(CShader* pShader);
        void SetRenderTarget(unsigned long index, CRenderTarget* pRenderTarget);
        void SetDepthStencilTarget(CRenderTarget* pRenderTarget);
        void SetViewport(const Vec2& pos, unsigned int width, unsigned int height);
       
        void BeginPerformanceMarker(const std::string& strName);
        void EndPerformanceMarker();

        inline ID3D11Device* GetDevice() const { return m_pDevice; }
        inline ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; }

        inline bool GetWindowed() const { return m_swapChainDesc.Windowed; }

        ERROR_TYPE TestCooperativeLevel();

    private:

        void ResetStates(bool bForceCommit = false);
        void ResetRenderStates(bool bForceCommit = false);
        void ResetTextureStates(bool bForceCommit = false);
        void ResetSamplerStates(bool bForceCommit = false);
        void ResetRenderTargets(bool bForceCommit = false);
        void ResetVertexBuffers(bool bForceCommit = false);
        void ResetIndexBuffer(bool bForceCommit = false);
        void ResetShader(bool bForceCommit = false);
        void ResetBoundTextureStates();

        void CommitStates(bool bForceCommit = false);
        void CommitRenderStates(bool bForceCommit = false);
        void CommitTextures(bool bForceCommit = false);
        void CommitSamplers(bool bForceCommit = false);
        void CommitRenderTargets(bool bForceCommit = false);
        void CommitVertexBuffers(bool bForceCommit = false);
        void CommitIndexBuffer(bool bForceCommit = false);
        void CommitShader(bool bForceCommit = false);
        
        void InitDefaultRenderStates(D3D11_DEPTH_STENCIL_DESC& depthStencilDesc);
        void InitDefaultRenderStates(D3D11_BLEND_DESC& blendDesc);
        void InitDefaultRenderStates(D3D11_RASTERIZER_DESC& rasterizerDesc);
        void InitDefaultSamplerStates(D3D11_SAMPLER_DESC& samplerDesc);
        void InitVertexIndexBufferDesc(D3D11_BUFFER_DESC& desc, unsigned int length, unsigned int bindFlags, unsigned long usage);

    private:

        ID3D11Device*           m_pDevice;
        ID3D11DeviceContext*    m_pDeviceContext;
        IDXGISwapChain*         m_pSwapChain;

        DXGI_SWAP_CHAIN_DESC    m_swapChainDesc;

        vecBoundRenderStates    m_renderStates;
        vecBoundRenderStates    m_boundRenderStates;
        vecBoundTextures        m_textures;
        vecBoundTextures        m_boundTextures;
        vecBoundSamplerStates   m_samplerStates;
        vecBoundSamplerStates   m_boundSamplerStates;
        vecBoundRenderTargets   m_renderTargets;
        vecBoundRenderTargets   m_boundRenderTargets;
        vecBoundVertexBuffers   m_vertexBuffers;
        vecBoundVertexBuffers   m_boundVertexBuffers;

        CIndexBuffer*           m_pBoundIndexBuffer;
        CIndexBuffer*           m_pIndexBuffer;

        CVertexDeclaration*     m_pBoundVertexDeclaration;
        CVertexDeclaration*     m_pVertexDeclaration;

        CShader*                m_pBoundShader;
        CShader*                m_pShader;

        std::set< ID3D11RasterizerState* >      m_createdRasterizerStates;
        std::set< ID3D11DepthStencilState* >    m_createdDepthStencilStates;
        std::set< ID3D11BlendState* >           m_createdBlendStates;
        std::set< ID3D11SamplerState* >         m_createdSamplerStates;

        CRenderTarget* m_pBackBuffer;
        CRenderTarget* m_pDepthStencilTarget;
        CRenderTarget* m_pBoundDepthStencilTarget;

        Microsoft::WRL::ComPtr< ID3DUserDefinedAnnotation > m_pPerformanceMarker;

        bool m_bVSync;
    };

    //typedef CD3D11RenderDevice CRenderDevice;

} // env
#endif // D3D11_RENDER_DEVICE_H 
#endif // ENV_GRAPHIC_API_D3D11
