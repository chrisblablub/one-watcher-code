#ifdef D3D9

#include <Graphic/D3D9/D3D9RenderDevice.h>
#include <Graphic/D3D9/D3D9APITypeConversion.h>

namespace env
{

    CD3D9RenderDevice::CD3D9RenderDevice()
        : m_pD3D(nullptr)
        , m_pDevice(nullptr)
        , m_pBackBuffer(nullptr)
        , m_pVertexDeclaration(nullptr)
        , m_pShader(nullptr)
    {
        SecureZeroMemory(&m_D3DPresentParams, sizeof(D3DPRESENT_PARAMETERS));

        m_D3DPresentParams.MultiSampleType = D3DMULTISAMPLE_NONE;
        m_D3DPresentParams.MultiSampleQuality = 0;
        m_D3DPresentParams.AutoDepthStencilFormat = ConvertToD3D9Format(TEXTUREFORMAT_TYPE_D24S8);
        m_D3DPresentParams.EnableAutoDepthStencil = true;
        m_D3DPresentParams.BackBufferFormat = ConvertToD3D9Format(TEXTUREFORMAT_TYPE_X8R8G8B8);
        m_D3DPresentParams.BackBufferCount = 1;
        m_D3DPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
        m_D3DPresentParams.Flags = 0;
        //if (bWindowed)
        m_D3DPresentParams.FullScreen_RefreshRateInHz = 0;

        m_boundSamplerStates.resize(8);
        for (vecBoundSamplerStates::iterator it = m_boundSamplerStates.begin(); it != m_boundSamplerStates.end(); ++it)
        {
            vecSamplerStates& samplerStates = *it;
            samplerStates.resize(SAMPLERSTATE_TYPE_END);

            for (vecSamplerStates::iterator jt = samplerStates.begin(); jt != samplerStates.end(); ++jt)
                jt->first = false;
        }

        m_boundRenderStates.resize(RENDERSTATE_TYPE_END);
        for (vecBoundRenderStates::iterator it = m_boundRenderStates.begin(); it != m_boundRenderStates.end(); ++it)
        {
            pairFlagRenderState& renderState = *it;
            
            renderState.first = false;
        }

        m_boundTextures.resize(8);
        for (vecBoundTextures::iterator it = m_boundTextures.begin(); it != m_boundTextures.end(); ++it)
            (*it) = 0;

        m_boundRenderTargets.resize(4);
        for (vecBoundRenderTargets::iterator it = m_boundRenderTargets.begin(); it != m_boundRenderTargets.end(); ++it)
            (*it) = 0;

        m_boundVertexBuffers.resize(4);
        for (vecBoundVertexBuffers::iterator it = m_boundVertexBuffers.begin(); it != m_boundVertexBuffers.end(); ++it)
            (*it) = 0;

        m_pBoundIndexBuffer = 0;
            
    }

    //-----------------------------------------------------------------------------------

    CD3D9RenderDevice::~CD3D9RenderDevice()
    {
        ENV_DELETE(m_pBackBuffer);
        ENV_RELEASE(m_pDevice);
        ENV_RELEASE(m_pD3D);
    }

    //-----------------------------------------------------------------------------------

    bool CD3D9RenderDevice::Initialize(const RenderSystemParams& renderSysParams, int screenWidth, int screenHeight, int adapater, bool bVSync, bool bWindowed)
    {
        m_adapter = adapater;

        m_D3DPresentParams.BackBufferWidth = screenWidth;
        m_D3DPresentParams.BackBufferHeight = screenHeight;
        m_D3DPresentParams.Windowed = bWindowed;
        m_D3DPresentParams.hDeviceWindow = renderSysParams.m_windowHandle;

        if (bVSync)
            m_D3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        else
            m_D3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

        ENV_DXCALL_RFALSE(m_pD3D->CreateDevice(m_adapter, D3DDEVTYPE_HAL, m_D3DPresentParams.hDeviceWindow,
            D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
            &m_D3DPresentParams, &m_pDevice));

        IDirect3DSurface9* pBackBuffer = 0;
        ENV_DXCALL_RFALSE(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer));

        m_pBackBuffer = new CRenderTarget(m_pDevice, pBackBuffer);

        return true;
    }

    //-----------------------------------------------------------------------------------

    bool CD3D9RenderDevice::EnumerateDisplayModes(vecDisplayModes& modes)
    {
        int NumAdapterModes = m_pD3D->GetAdapterModeCount(m_adapter, m_D3DPresentParams.BackBufferFormat);

        if (NumAdapterModes <= 0)
            return false;

        modes.clear();
        modes.reserve(NumAdapterModes);

        bool bValidDisplayMode = false;
        for (int i = 0; i < NumAdapterModes; ++i)
        {
            D3DDISPLAYMODE Mode;
            if (FAILED(m_pD3D->EnumAdapterModes(m_adapter, m_D3DPresentParams.BackBufferFormat, i, &Mode)))
            {
                continue;
            }

            modes.push_back(Mode);
        }

        return true;
    }

    //-----------------------------------------------------------------------------------

    void CD3D9RenderDevice::SetFVF(unsigned long fvf)
    {
        m_pDevice->SetFVF(fvf);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::DrawPrimitive(PRIMITIVE_TYPE primitiveType, unsigned int startVertex, unsigned int primitiveCount)
    {
        if (m_pShader)
        {
            m_pShader->UpdateUniforms();
            m_pShader->Begin();
            m_pShader->BeginPass(0);
        }

        m_pDevice->DrawPrimitive(ConvertToD3D9PrimitiveType(primitiveType), startVertex, primitiveCount);

        if (m_pShader)
        {
            m_pShader->EndPass();
            m_pShader->End();
        }
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::DrawIndexedPrimitive(PRIMITIVE_TYPE primitiveType, int baseVertexIndex,
        unsigned int minIndex, unsigned int numVertices,
        unsigned int startIndex, unsigned int primitiveCount)
    {
        if (m_pShader)
        {
            m_pShader->UpdateUniforms();
            m_pShader->Begin();
            m_pShader->BeginPass(0);
        }

        m_pDevice->DrawIndexedPrimitive(ConvertToD3D9PrimitiveType(primitiveType), baseVertexIndex, minIndex, numVertices, startIndex, primitiveCount);

        if (m_pShader)
        {
            m_pShader->EndPass();
            m_pShader->End();
        }

    }

    //-----------------------------------------------------------------------------------
    bool CD3D9RenderDevice::CheckDeviceSettings()
    {
        ENV_LOG_INFO("DEVICE SETTINGS ARE CURRENTLY NOT BEING CHECKED.");

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::Reset(int screenWidth, int screenHeight, bool bVSync, bool bWindowed)
    {
        m_D3DPresentParams.BackBufferWidth = screenWidth;
        m_D3DPresentParams.BackBufferHeight = screenHeight;
        m_D3DPresentParams.Windowed = bWindowed;

        if (bVSync)
            m_D3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        else
            m_D3DPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

        m_pDevice->Reset(&m_D3DPresentParams);

        IDirect3DSurface9* pBackBuffer = 0;
        m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);

        m_pBackBuffer = new CRenderTarget(m_pDevice, pBackBuffer);

        for (vecBoundSamplerStates::iterator it = m_boundSamplerStates.begin(); it != m_boundSamplerStates.end(); ++it)
            for (vecSamplerStates::iterator jt = it->begin(); jt != it->end(); ++jt)
                jt->first = false;

        for (vecBoundRenderStates::iterator it = m_boundRenderStates.begin(); it != m_boundRenderStates.end(); ++it)
            it->first = false;

        for (vecBoundTextures::iterator it = m_boundTextures.begin(); it != m_boundTextures.end(); ++it)
            (*it) = 0;

        for (vecBoundRenderTargets::iterator it = m_boundRenderTargets.begin(); it != m_boundRenderTargets.end(); ++it)
            (*it) = 0;

        for (vecBoundVertexBuffers::iterator it = m_boundVertexBuffers.begin(); it != m_boundVertexBuffers.end(); ++it)
            (*it) = 0;

        m_pBoundIndexBuffer = 0;
    }

    //-----------------------------------------------------------------------------------
    CVertexDeclaration* CD3D9RenderDevice::CreateVertexDeclaration(vecVertexDeclarationElements& vertexDeclarationElements, unsigned int stride, CShader* pShader)
    {
        return new CVertexDeclaration(m_pDevice, vertexDeclarationElements, stride);
    }

    //-----------------------------------------------------------------------------------
    CVertexBuffer* CD3D9RenderDevice::CreateVertexBuffer(unsigned int length, unsigned long usage, MEMORY_TYPE pool)
    {
        IDirect3DVertexBuffer9* pVB = 0;
        m_pDevice->CreateVertexBuffer(length, ConvertToD3D9Usage(usage), 0, ConvertToD3D9Pool(pool), &pVB, 0);

        return new CVertexBuffer(m_pDevice, pVB);
    }

    //-----------------------------------------------------------------------------------
    CIndexBuffer* CD3D9RenderDevice::CreateIndexBuffer(unsigned int length, unsigned long usage, TEXTUREFORMAT_TYPE format, MEMORY_TYPE pool)
    {
        IDirect3DIndexBuffer9* pIB = 0;
        m_pDevice->CreateIndexBuffer(length, ConvertToD3D9Usage(usage), ConvertToD3D9Format(format), ConvertToD3D9Pool(pool), &pIB, 0);

        return new CIndexBuffer(m_pDevice, pIB);
    }
    
    //-----------------------------------------------------------------------------------

    CTextureBase* CD3D9RenderDevice::CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned long usage,
        TEXTUREFORMAT_TYPE format, MEMORY_TYPE memory_pool)
    {
        IDirect3DTexture9* pTexture = 0;
        
        m_pDevice->CreateTexture(width, height, levels, ConvertToD3D9Usage(usage), ConvertToD3D9Format(format), ConvertToD3D9Pool(memory_pool), &pTexture, 0);
        
        return new CTextureBase(m_pDevice, pTexture, width, height);
    }

    //-----------------------------------------------------------------------------------
    CTextureManaged* CD3D9RenderDevice::CreateTexture()
    {
        return new CTextureManaged(m_pDevice);
    }

    //-----------------------------------------------------------------------------------
    CRenderTarget* CD3D9RenderDevice::CreateRenderTarget(unsigned int width, unsigned int height, TEXTUREFORMAT_TYPE format, bool bMipMaps)
    {
        return new CRenderTarget(m_pDevice, width, height, format, bMipMaps);
    }

    //-----------------------------------------------------------------------------------
    CFont* CD3D9RenderDevice::CreateD3DFont(const std::string& strFontFamily, bool bItalic, unsigned int weight, int size, int sizeMin, int sizeMax)
    {
        return new CFont(m_pDevice, strFontFamily, bItalic, weight, size, sizeMin, sizeMax);
    }

    //-----------------------------------------------------------------------------------
    CShader* CD3D9RenderDevice::CreateShader()
    {
        return new CShader(m_pDevice);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::OnLostDevice()
    {
        ENV_DELETE(m_pBackBuffer);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetShader(CShader* pShader)
    {
        m_pShader = pShader;
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::OnResetDevice(int screenWidth, int screenHeight)
    {
        for (vecBoundSamplerStates::iterator it = m_boundSamplerStates.begin(); it != m_boundSamplerStates.end(); ++it)
            for (vecSamplerStates::iterator jt = it->begin(); jt != it->end(); ++jt)
                jt->first = false;

        for (vecBoundRenderStates::iterator it = m_boundRenderStates.begin(); it != m_boundRenderStates.end(); ++it)
            it->first = false;

        for (vecBoundTextures::iterator it = m_boundTextures.begin(); it != m_boundTextures.end(); ++it)
            (*it) = 0;

        for (vecBoundRenderTargets::iterator it = m_boundRenderTargets.begin(); it != m_boundRenderTargets.end(); ++it)
            (*it) = 0;

        for (vecBoundVertexBuffers::iterator it = m_boundVertexBuffers.begin(); it != m_boundVertexBuffers.end(); ++it)
            (*it) = 0;

        m_pBoundIndexBuffer = 0;
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::BeginScene()
    {
        m_pDevice->BeginScene();

        for (vecBoundSamplerStates::iterator it = m_boundSamplerStates.begin(); it != m_boundSamplerStates.end(); ++it)
            for (vecSamplerStates::iterator jt = it->begin(); jt != it->end(); ++jt)
                jt->first = false;

        for (vecBoundTextures::iterator it = m_boundTextures.begin(); it != m_boundTextures.end(); ++it)
            (*it) = 0;

        for (vecBoundRenderTargets::iterator it = m_boundRenderTargets.begin(); it != m_boundRenderTargets.end(); ++it)
            (*it) = 0;

        for (vecBoundVertexBuffers::iterator it = m_boundVertexBuffers.begin(); it != m_boundVertexBuffers.end(); ++it)
            (*it) = 0;

        m_pBoundIndexBuffer = 0;
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::EndScene()
    {
        m_pDevice->EndScene();

        m_pDevice->Present(0, 0, 0, 0);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::Clear(const Vec4& ClearColor, unsigned long Buffers, float z, unsigned long stencil)
    {
        m_pDevice->Clear(0, 0, ConvertToD3D9ClearType(Buffers), 
            COLOR_ARGB32((int)(ClearColor.w * 255.0), (int)(ClearColor.x * 255.0), (int)(ClearColor.y * 255.0), (int)(ClearColor.z * 255.0)), z, stencil);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetTransform(TRANSFORM_TYPE transformType, const Matrix4x4& transformMatrix)
    {
        m_pDevice->SetTransform(ConvertToD3D9Transform(transformType), &convertToD3D9Matrix(transformMatrix));
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::GetTransform(TRANSFORM_TYPE transformType, Matrix4x4& transformMatrix)
    {
        D3DXMATRIX mtx;
        m_pDevice->GetTransform(ConvertToD3D9Transform(transformType), &mtx);

        transformMatrix = convertFromD3D9Matrix(mtx);
    }

    //-----------------------------------------------------------------------------------
    ERROR_TYPE CD3D9RenderDevice::TestCooperativeLevel()
    {
        HRESULT hr = m_pDevice->TestCooperativeLevel();
        return convertFromD3D9ErrorType(hr);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetRenderState(RENDERSTATE_TYPE rs, unsigned long value)
    {
        pairFlagRenderState& flagRenderState = m_boundRenderStates[rs];

        if (!flagRenderState.first || flagRenderState.second != value)
        {
            m_boundRenderStates[rs].second = value;
            m_boundRenderStates[rs].first = true;

            m_pDevice->SetRenderState(ConvertToD3D9RenderStateType(rs), ConvertToD3D9RenderStateValue(rs, value));
        }
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetSamplerState(unsigned long Sampler, SAMPLERSTATE_TYPE Type, unsigned long Value)
    {
        pairFlagSamplerState& flagSamplerState = m_boundSamplerStates[Sampler][Type];

        if (!flagSamplerState.first || /*flagSamplerState.second.first != Type ||*/ flagSamplerState.second != Value)
        {
            flagSamplerState.first = true;
            flagSamplerState.second = Value;
            //flagSamplerState.second = pairSamplerState(Type, Value);
            
            D3DSAMPLERSTATETYPE d3d9SamplerState;
            unsigned long d3d9Value;
            ConvertToD3D9SamplerStateType(Type, Value, d3d9SamplerState, d3d9Value);

            m_pDevice->SetSamplerState(Sampler, d3d9SamplerState, d3d9Value);
        }
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetTexture(unsigned long index, CTextureBase* pTexture)
    {
        if (pTexture)
        {
            CTextureBase* pBoundTexture = m_boundTextures[index];
            if (!pBoundTexture || pBoundTexture != pTexture)
            {
                pTexture->Bind(index);

                m_boundTextures[index] = pTexture;
            }
        }
        else
        {
            m_pDevice->SetTexture(index, 0);

            m_boundTextures[index] = 0;
        }

        
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetVertexBuffer(unsigned int streamNumber, unsigned int offset, CVertexBuffer* pVertexBuffer)
    {
        if (pVertexBuffer)
        {
            CVertexBuffer* pBoundVertexBuffer = m_boundVertexBuffers[streamNumber];

            if (!pBoundVertexBuffer || pBoundVertexBuffer != pVertexBuffer)
            {
                ENV_ASSERT_DBG(m_pVertexDeclaration);
                pVertexBuffer->Bind(streamNumber, offset, m_pVertexDeclaration->GetStride());

                m_boundVertexBuffers[streamNumber] = pVertexBuffer;
            }
        }
        else
        {
            m_boundVertexBuffers[streamNumber] = 0;
        }
    }
    void CD3D9RenderDevice::SetVertexDeclaration(CVertexDeclaration* pVertexDeclaration)
    {
        m_pVertexDeclaration = pVertexDeclaration;

        if (m_pVertexDeclaration)
            m_pVertexDeclaration->Bind();
        else
            m_pDevice->SetVertexDeclaration(nullptr);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetIndexBuffer(CIndexBuffer* pIndexBuffer)
    {
        if (pIndexBuffer)
        {

            if (!m_pBoundIndexBuffer || m_pBoundIndexBuffer != pIndexBuffer)
            {
                pIndexBuffer->Bind();

                m_pBoundIndexBuffer = pIndexBuffer;
            }
        }
        else
        {
            m_pBoundIndexBuffer = 0;
        }
    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetRenderTarget(unsigned long index, CRenderTarget* pRenderTarget)
    {
        if (pRenderTarget)
        {
            CRenderTarget* pBoundRenderTarget = m_boundRenderTargets[index];

            if (!pBoundRenderTarget || pBoundRenderTarget != pRenderTarget)
            {
                pRenderTarget->Bind(index);

                m_boundRenderTargets[index] = pRenderTarget;
            }
        }
        else
        {
            m_pDevice->SetRenderTarget(index, 0);

            m_boundRenderTargets[index] = 0;
        }
    }

    void CD3D9RenderDevice::SetDepthStencilTarget(CRenderTarget* pRenderTarget)
    {

    }

    //-----------------------------------------------------------------------------------
    void CD3D9RenderDevice::SetTextureSampler(unsigned long index, const CTextureSampler& sampler)
    {
        SetSamplerState(index, SAMPLERSTATE_TYPE_ADDRESSU,    sampler.GetAddressU());
        SetSamplerState(index, SAMPLERSTATE_TYPE_ADDRESSV,    sampler.GetAddressV());
        SetSamplerState(index, SAMPLERSTATE_TYPE_MINFILTER,   sampler.GetMinFilter());
        SetSamplerState(index, SAMPLERSTATE_TYPE_MAGFILTER,   sampler.GetMagFilter());
        SetSamplerState(index, SAMPLERSTATE_TYPE_MIPFILTER,   sampler.GetMipFilter());
        SetSamplerState(index, SAMPLERSTATE_TYPE_SRGBTEXTURE, sampler.GetSRGB());

        if (sampler.GetMinFilter() == TEXTUREFILTER_TYPE_ANISOTROPIC)
            SetSamplerState(index, SAMPLERSTATE_TYPE_MAXANISOTROPY, sampler.GetMaxAnisotropy());
    }
} // env

#endif // D3D9