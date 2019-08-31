#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11RenderDevice.h>
#include <Graphic/D3D11/D3D11APITypeConversion.h>
#include <locale>
#include <codecvt>

namespace env
{
    //-----------------------------------------------------------------------------------
    CRenderDevice::CRenderDevice()
        : m_pSwapChain(nullptr)
        , m_pDevice(nullptr)
        , m_pDeviceContext(nullptr)
        , m_pBackBuffer(nullptr)
        , m_pIndexBuffer(nullptr)
        , m_pBoundIndexBuffer(nullptr)
        , m_pShader(nullptr)
        , m_pBoundShader(nullptr)
        , m_pVertexDeclaration(nullptr)
        , m_pBoundVertexDeclaration(nullptr)
        , m_pDepthStencilTarget(nullptr)
        , m_pBoundDepthStencilTarget(nullptr)
        , m_pPerformanceMarker(nullptr)
        , m_bVSync(true) {}

    //-----------------------------------------------------------------------------------
    CRenderDevice::~CRenderDevice()
    {
        for (std::set< ID3D11SamplerState* >::iterator it = m_createdSamplerStates.begin(); it != m_createdSamplerStates.end(); ++it)
        {
            ID3D11SamplerState* pState = *it;
            ENV_RELEASE(pState);
        }

        for (std::set< ID3D11BlendState* >::iterator it = m_createdBlendStates.begin(); it != m_createdBlendStates.end(); ++it)
        {
            ID3D11BlendState* pState = *it;
            ENV_RELEASE(pState);
        }

        for (std::set< ID3D11RasterizerState* >::iterator it = m_createdRasterizerStates.begin(); it != m_createdRasterizerStates.end(); ++it)
        {
            ID3D11RasterizerState* pState = *it;
            ENV_RELEASE(pState);
        }

        for (std::set< ID3D11DepthStencilState* >::iterator it = m_createdDepthStencilStates.begin(); it != m_createdDepthStencilStates.end(); ++it)
        {
            ID3D11DepthStencilState* pState = *it;
            ENV_RELEASE(pState);
        }

        ENV_DELETE(m_pBackBuffer);

        m_pPerformanceMarker = Microsoft::WRL::ComPtr< ID3DUserDefinedAnnotation >();

        ENV_RELEASE(m_pSwapChain);
        ENV_RELEASE(m_pDeviceContext);
        
        if (m_pDevice && false)
        {
            ID3D11Debug* pDebug = nullptr;
            ENV_DXCALL_DBG(m_pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&pDebug));

            ENV_RELEASE(m_pDevice);

            ENV_DXCALL_DBG(pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL));
            ENV_RELEASE(pDebug);
        }
        else
        {
            ENV_RELEASE(m_pDevice);
        }
        
    }

    //-----------------------------------------------------------------------------------
    bool CRenderDevice::Initialize(const RenderSystemParams& renderSysParams, int screenWidth, int screenHeight, int adapater, bool bVSync, bool bWindowed)
    {
        m_swapChainDesc.BufferDesc.Width = screenWidth;
        m_swapChainDesc.BufferDesc.Height = screenHeight;
        m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        m_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        m_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        m_swapChainDesc.BufferCount = 2;
        m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        m_swapChainDesc.SampleDesc.Count = 1;
        m_swapChainDesc.SampleDesc.Quality = 0;
        m_swapChainDesc.Windowed = bWindowed;
        m_swapChainDesc.OutputWindow = renderSysParams.m_windowHandle;
        m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        m_swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        unsigned int deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#ifdef DEBUG
        deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

        D3D_FEATURE_LEVEL featureLevel;
        ENV_DXCALL_RFALSE(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, NULL, 0, D3D11_SDK_VERSION,
                                                        &m_swapChainDesc, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pDeviceContext));

        ENV_DXCALL(m_pDeviceContext->QueryInterface(IID_PPV_ARGS(&m_pPerformanceMarker)));

        ResetStates(true);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CRenderDevice::EnumerateDisplayModes(vecDisplayModes& modes)
    {
        IDXGIDevice * pDXGIDevice = nullptr;
        if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice)))
            return false;

        IDXGIAdapter * pDXGIAdapter = nullptr;
        if (FAILED(pDXGIDevice->GetAdapter(&pDXGIAdapter)))
            return false;

        IDXGIOutput* pOutput;
        if (FAILED(pDXGIAdapter->EnumOutputs(0, &pOutput)))
            return false;

        UINT numModes;
        if (FAILED(pOutput->GetDisplayModeList(m_swapChainDesc.BufferDesc.Format, m_swapChainDesc.Flags, &numModes, 0)))
            return false;

        std::vector<DXGI_MODE_DESC> descs;
        descs.resize(numModes);

        if (FAILED(pOutput->GetDisplayModeList(m_swapChainDesc.BufferDesc.Format, m_swapChainDesc.Flags, &numModes, &descs[0])))
            return false;
        

        modes.resize(descs.size());

        for (size_t i = 0; i < descs.size(); ++i)
        {
            modes[i].Width = descs[i].Width;
            modes[i].Height = descs[i].Height;
            modes[i].Desc = descs[i];
        }

        ENV_RELEASE(pOutput);
        ENV_RELEASE(pDXGIAdapter);
        ENV_RELEASE(pDXGIDevice);

        return true;
    }


    void CRenderDevice::FindClosestMatchingResolution(int& width, int& height, int requestedWidth, int requestedHeight)
    {
        IDXGIDevice * pDXGIDevice = nullptr;
        if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice)))
            return;

        IDXGIAdapter * pDXGIAdapter = nullptr;
        if (FAILED(pDXGIDevice->GetAdapter(&pDXGIAdapter)))
            return;

        IDXGIOutput* pOutput;
        if (FAILED(pDXGIAdapter->EnumOutputs(0, &pOutput)))
            return;

        UINT numModes;
        if (FAILED(pOutput->GetDisplayModeList(m_swapChainDesc.BufferDesc.Format, m_swapChainDesc.Flags, &numModes, 0)))
            return;

        DXGI_MODE_DESC requestedMode;
        requestedMode.Format = m_swapChainDesc.BufferDesc.Format;
        requestedMode.RefreshRate.Denominator = 0;
        requestedMode.RefreshRate.Numerator = 0;
        requestedMode.Scaling = m_swapChainDesc.BufferDesc.Scaling;
        requestedMode.ScanlineOrdering = m_swapChainDesc.BufferDesc.ScanlineOrdering;
        requestedMode.Width = requestedWidth;
        requestedMode.Height = requestedHeight;

        DXGI_MODE_DESC mode = requestedMode;

        if (SUCCEEDED(pOutput->FindClosestMatchingMode(&requestedMode, &mode, m_pDevice)))
        {
            width = mode.Width;
            height = mode.Height;
        }
        else
        {
            width = m_swapChainDesc.BufferDesc.Width;
            height = m_swapChainDesc.BufferDesc.Height;
        }
        

        ENV_RELEASE(pOutput);
        ENV_RELEASE(pDXGIAdapter);
        ENV_RELEASE(pDXGIDevice);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::DrawPrimitive(PRIMITIVE_TYPE primitiveType, unsigned int startVertex, unsigned int primitiveCount)
    {

        if (m_pShader)
            m_pShader->Begin();

        // Commit renderstates to the graphics card
        CommitStates();

        unsigned int numVertices = 0;

        if (primitiveType == PRIMITIVE_TYPE_TRIANGLELIST)
        {
            numVertices = primitiveCount * 3;

            m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        }
        else if (primitiveType == PRIMITIVE_TYPE_POINTLIST)
        {
            numVertices = primitiveCount;

            m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        }
        else if (primitiveType == PRIMITIVE_TYPE_LINELIST)
        {
            numVertices = primitiveCount * 2;

            m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }
        else
        {
            ENV_ASSERT_DBG(false);
        }

        m_pDeviceContext->Draw(numVertices, startVertex);

        if (m_pShader)
            m_pShader->End();

        ResetBoundTextureStates();
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::DrawIndexedPrimitive(PRIMITIVE_TYPE primitiveType, int baseVertexIndex,
                                             unsigned int minIndex, unsigned int numVertices,
                                             unsigned int startIndex, unsigned int primitiveCount)
    {
        if (m_pShader)
            m_pShader->Begin();

        // Commit renderstates to the graphics card
        CommitStates();

        unsigned int numIndices = 0;

        if (primitiveType == PRIMITIVE_TYPE_TRIANGLELIST)
        {
            numIndices = primitiveCount * 3;

            m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        }
        else if (primitiveType == PRIMITIVE_TYPE_POINTLIST)
        {
            numIndices = primitiveCount;

            m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
        }
        else if (primitiveType == PRIMITIVE_TYPE_LINELIST)
        {
            numIndices = primitiveCount * 2;

            m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }
        else
        {
            ENV_ASSERT_DBG(false);
        }

        m_pDeviceContext->DrawIndexed(numIndices, startIndex, baseVertexIndex);

        if (m_pShader)
            m_pShader->End();

        ResetBoundTextureStates();
    }

    //-----------------------------------------------------------------------------------
    bool CRenderDevice::CheckDeviceSettings()
    {
        ENV_LOG_INFO("DEVICE SETTINGS ARE CURRENTLY NOT BEING CHECKED.");

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::Reset(int screenWidth, int screenHeight, bool bVSync, bool bWindowed)
    {
        m_swapChainDesc.BufferDesc.Width = screenWidth;
        m_swapChainDesc.BufferDesc.Height = screenHeight;
        m_swapChainDesc.Windowed = bWindowed;
        
        m_bVSync = bVSync;

        ResetStates();
    }

    //-----------------------------------------------------------------------------------
    std::unique_ptr<CVertexDeclaration> CRenderDevice::CreateVertexDeclaration(vecVertexDeclarationElements& vertexDeclarationElements, unsigned int stride, CShader* pShader)
    {
        return std::move(std::make_unique<CVertexDeclaration>(m_pDevice, m_pDeviceContext, pShader->GetCode(), vertexDeclarationElements, stride));
    }

    //-----------------------------------------------------------------------------------
    CVertexBuffer* CRenderDevice::CreateVertexBuffer(unsigned int length, unsigned long usage, void* pData)
    {
        D3D11_BUFFER_DESC vertexBufferDesc;
        InitVertexIndexBufferDesc(vertexBufferDesc, length, D3D11_BIND_VERTEX_BUFFER, usage);


        ID3D11Buffer* pVertexBuffer = nullptr;

        if (pData)
        {
            D3D11_SUBRESOURCE_DATA data;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            data.pSysMem = pData;

            ENV_DXCALL(m_pDevice->CreateBuffer(&vertexBufferDesc, &data, &pVertexBuffer));
        }
        else
        {
            ENV_DXCALL(m_pDevice->CreateBuffer(&vertexBufferDesc, nullptr, &pVertexBuffer));
        }


        return new CVertexBuffer(m_pDeviceContext, pVertexBuffer);
    }

    //-----------------------------------------------------------------------------------
    CIndexBuffer* CRenderDevice::CreateIndexBuffer(unsigned int length, unsigned long usage, TEXTUREFORMAT_TYPE format, void* pData)
    {
        D3D11_BUFFER_DESC indexBufferDesc;
        InitVertexIndexBufferDesc(indexBufferDesc, length, D3D11_BIND_INDEX_BUFFER, usage);

        ID3D11Buffer* pIndexBuffer = nullptr;

        if (pData)
        {
            D3D11_SUBRESOURCE_DATA data;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            data.pSysMem = pData;

            ENV_DXCALL(m_pDevice->CreateBuffer(&indexBufferDesc, &data, &pIndexBuffer));
        }
        else
        {
            ENV_DXCALL(m_pDevice->CreateBuffer(&indexBufferDesc, nullptr, &pIndexBuffer));
        }

        return new CIndexBuffer(m_pDeviceContext, pIndexBuffer, format);
    }

    //-----------------------------------------------------------------------------------
    CTextureBase* CRenderDevice::CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned long usage,
                                               TEXTUREFORMAT_TYPE format, MEMORY_TYPE memory_pool)
    {
        ID3D11Texture2D* pTexture = nullptr;
        ID3D11ShaderResourceView* pTextureSRV = nullptr;

        D3D11_TEXTURE2D_DESC textureDesc;
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = levels;
        textureDesc.ArraySize = 1;
        textureDesc.Format = ConvertToD3D11Format(format);
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = ConvertToD3D11Usage(usage);
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SHADER_RESOURCE_VIEW_DESC textureSRVDesc;
        textureSRVDesc.Format = ConvertToD3D11Format(format);
        textureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureSRVDesc.Texture2D.MipLevels = levels;
        textureSRVDesc.Texture2D.MostDetailedMip = -1;

        ENV_DXCALL(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture));
        ENV_DXCALL(m_pDevice->CreateShaderResourceView(pTexture, &textureSRVDesc, &pTextureSRV));

        return new CTextureBase(m_pDevice, m_pDeviceContext, pTexture, pTextureSRV, width, height);
    }

    //-----------------------------------------------------------------------------------
    CTextureManaged* CRenderDevice::CreateTexture()
    {
        return new CTextureManaged(m_pDevice, m_pDeviceContext);
    }

    //-----------------------------------------------------------------------------------
    std::unique_ptr<CRenderTarget> CRenderDevice::CreateRenderTarget(unsigned int width, unsigned int height, TEXTUREFORMAT_TYPE format, bool bCreateDepthStencil, bool bMipMaps)
    {
        return std::move(std::make_unique<CRenderTarget>(m_pDevice, m_pDeviceContext, width, height, format, bCreateDepthStencil, bMipMaps));
    }

    //-----------------------------------------------------------------------------------
    CFont* CRenderDevice::CreateD3DFont(const std::string& strFontFamily, bool bItalic, unsigned int weight, int size, int sizeMin, int sizeMax)
    {
        return new CFont(m_pDevice, m_pDeviceContext);
    }

    //-----------------------------------------------------------------------------------
    CShader* CRenderDevice::CreateShader()
    {
        return new CShader(m_pDevice, m_pDeviceContext);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::OnLostDevice()
    {
        m_pDeviceContext->ClearState();

        ENV_DELETE(m_pBackBuffer);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetFullscreen(bool bFullscreen)
    {
        m_swapChainDesc.Windowed = !bFullscreen;
        ENV_DXCALL(m_pSwapChain->SetFullscreenState(!m_swapChainDesc.Windowed, nullptr));
    }

    //-----------------------------------------------------------------------------------
    bool CRenderDevice::GetFullscreen() const
    {
        BOOL bFullscreen;

        ENV_DXCALL(m_pSwapChain->GetFullscreenState(&bFullscreen, NULL));

        return bFullscreen;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResizeTarget(int w, int h)
    {
        m_swapChainDesc.BufferDesc.Width = w;
        m_swapChainDesc.BufferDesc.Height = h;

        vecDisplayModes modes;
        EnumerateDisplayModes(modes);

        unsigned int RefreshRate = 0;
        int index = -1;

        for (unsigned int i = 0; i < modes.size(); ++i)
        {
            if (modes[i].Width != w ||
                modes[i].Height != h)
            {
                continue;
            }

            if (modes[i].Desc.RefreshRate.Numerator > RefreshRate)
            {
                RefreshRate = modes[i].Desc.RefreshRate.Numerator;
                index = i;
            }
        }

        DXGI_MODE_DESC bufferDesc;
        SecureZeroMemory(&bufferDesc, sizeof(bufferDesc));

        if (index != -1)
        {
            bufferDesc.Width = modes[index].Desc.Width;
            bufferDesc.Height = modes[index].Desc.Height;
        }
        else
        {
            bufferDesc.Width = w;
            bufferDesc.Height = h;
        }
        m_pSwapChain->ResizeTarget(&m_swapChainDesc.BufferDesc);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::OnResetDevice(int screenWidth, int screenHeight)
    {
        ResetTextureStates();

        for (vecBoundRenderTargets::iterator it = m_boundRenderTargets.begin(); it != m_boundRenderTargets.end(); ++it)
            (*it) = 0;

        for (vecBoundVertexBuffers::iterator it = m_boundVertexBuffers.begin(); it != m_boundVertexBuffers.end(); ++it)
            (*it) = 0;

        m_pBoundIndexBuffer = 0;

        m_swapChainDesc.BufferDesc.Width = screenWidth;
        m_swapChainDesc.BufferDesc.Height = screenHeight;

        vecDisplayModes modes;
        EnumerateDisplayModes(modes);

        unsigned int RefreshRate = 0;
        int index = -1;

        for (unsigned int i = 0; i < modes.size(); ++i)
        {
            if (modes[i].Width != screenWidth ||
                modes[i].Height != screenHeight)
            {
                continue;
            }

            if (modes[i].Desc.RefreshRate.Numerator > RefreshRate)
            {
                RefreshRate = modes[i].Desc.RefreshRate.Numerator;
                index = i;
            }
        }

         if (index != -1)
             m_swapChainDesc.BufferDesc = modes[index].Desc;

        ID3D11Texture2D* pBackBuffer = 0;


        ENV_DXCALL(m_pSwapChain->ResizeBuffers(0, m_swapChainDesc.BufferDesc.Width, m_swapChainDesc.BufferDesc.Height, m_swapChainDesc.BufferDesc.Format, m_swapChainDesc.Flags));
        ENV_DXCALL(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));

        m_pBackBuffer = new CRenderTarget(m_pDevice, m_pDeviceContext, pBackBuffer, true);
        m_pBackBuffer->OnResetDevice();

        // ENV_DXCALL(m_pSwapChain->SetFullscreenState(!m_swapChainDesc.Windowed, nullptr));
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::BeginScene()
    {
        ResetTextureStates();
        ResetSamplerStates();
        ResetShader();
        ResetRenderStates();

        m_pIndexBuffer = nullptr;
        m_pBoundIndexBuffer = nullptr;

        for (vecBoundVertexBuffers::iterator it = m_boundVertexBuffers.begin(); it != m_boundVertexBuffers.end(); ++it)
        {
            *it = nullptr;
        }

        for (vecBoundVertexBuffers::iterator it = m_vertexBuffers.begin(); it != m_vertexBuffers.end(); ++it)
        {
            *it = nullptr;
        }

        for (unsigned int i = 0; i < m_renderTargets.size(); ++i)
        {
            m_renderTargets[i] = nullptr;
            m_boundRenderTargets[i] = nullptr;
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::EndScene()
    {
        m_pSwapChain->Present(m_bVSync ? 1 : 0, 0);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::Clear(const Vec4& ClearColor, unsigned long Buffers, float z, unsigned long stencil)
    {
        CommitStates();

        if (Buffers & CLEAR_TYPE_TARGET)
        {
            for (unsigned int i = 0; i < m_boundRenderTargets.size(); ++i)
            {
                if (m_boundRenderTargets[i] &&
                    m_boundRenderTargets[i]->GetRenderTargetView())
                {
                    m_pDeviceContext->ClearRenderTargetView(m_boundRenderTargets[i]->GetRenderTargetView(), &ClearColor[0]);
                }
            }
        }

        unsigned long clearFlags = ConvertToD3D11ClearType(Buffers);

        if (m_pBoundDepthStencilTarget && clearFlags > 0)
            m_pDeviceContext->ClearDepthStencilView(m_pBoundDepthStencilTarget->GetDepthStencilView(), clearFlags, z, stencil);
    }

    //-----------------------------------------------------------------------------------
    ERROR_TYPE CRenderDevice::TestCooperativeLevel()
    {
        return ERROR_TYPE_OK;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetStates(bool bForceCommit)
    {
        bForceCommit = true;

        ResetRenderStates(bForceCommit);
        ResetTextureStates(bForceCommit);
        ResetSamplerStates(bForceCommit);
        ResetRenderTargets(bForceCommit);
        ResetVertexBuffers(bForceCommit);
        ResetIndexBuffer(bForceCommit);
        ResetShader(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetToDefaultRenderStates()
    {
        // Init bound renderstates to D3D11 defaults
        // Rasterizer
        m_boundRenderStates.resize(RENDERSTATE_TYPE_END);
        m_renderStates.resize(RENDERSTATE_TYPE_END);
        m_renderStates[RENDERSTATE_TYPE_ZENABLE] = true;
        m_renderStates[RENDERSTATE_TYPE_ZWRITEENABLE] = true;
        m_renderStates[RENDERSTATE_TYPE_ZFUNC] = COMPARISON_TYPE_LESS;
        m_renderStates[RENDERSTATE_TYPE_STENCILWRITEMASK] = 0xFFFFFFFF;
        m_renderStates[RENDERSTATE_TYPE_STENCILREADMASK] = 0xFFFFFFFF;
        m_renderStates[RENDERSTATE_TYPE_STENCILENABLE] = false;
        m_renderStates[RENDERSTATE_TYPE_STENCILFUNC] = COMPARISON_TYPE_ALWAYS;
        m_renderStates[RENDERSTATE_TYPE_STENCILZFAIL] = STENCILOP_TYPE_KEEP;
        m_renderStates[RENDERSTATE_TYPE_STENCILPASS] = STENCILOP_TYPE_KEEP;
        m_renderStates[RENDERSTATE_TYPE_STENCILFAIL] = STENCILOP_TYPE_KEEP;
        m_renderStates[RENDERSTATE_TYPE_STENCILREF] = 0;

        // Blend
        m_renderStates[RENDERSTATE_TYPE_ALPHABLENDENABLE] = false;
        m_renderStates[RENDERSTATE_TYPE_SRCBLEND] = BLEND_TYPE_ONE;
        m_renderStates[RENDERSTATE_TYPE_DESTBLEND] = BLEND_TYPE_ZERO;
        m_renderStates[RENDERSTATE_TYPE_BLENDOP] = BLENDOP_TYPE_ADD;
        m_renderStates[RENDERSTATE_TYPE_SRCBLENDALPHA] = BLEND_TYPE_ONE;
        m_renderStates[RENDERSTATE_TYPE_DESTBLENDALPHA] = BLEND_TYPE_ZERO;
        m_renderStates[RENDERSTATE_TYPE_BLENDOPALPHA] = BLENDOP_TYPE_ADD;
        m_renderStates[RENDERSTATE_TYPE_COLORWRITEENABLE] = true;

        // DepthStencil
        m_renderStates[RENDERSTATE_TYPE_FILLMODE] = FILLMODE_TYPE_SOLID;
        m_renderStates[RENDERSTATE_TYPE_CULLMODE] = CULLMODE_TYPE_BACK;
        m_renderStates[RENDERSTATE_TYPE_SCISSORTESTENABLE] = false;

        // Not used by D3D11
        m_renderStates[RENDERSTATE_TYPE_ALPHATESTENABLE] = false;
        m_renderStates[RENDERSTATE_TYPE_ALPHAFUNC] = COMPARISON_TYPE_GREATEREQUAL;
        m_renderStates[RENDERSTATE_TYPE_ALPHAREF] = 0;
        m_renderStates[RENDERSTATE_TYPE_SRGBWRITEENABLE] = false;
        m_renderStates[RENDERSTATE_TYPE_SEPARATEALPHABLENDENABLE] = false;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetRenderStates(bool bForceCommit)
    {
        ResetToDefaultRenderStates();

        CommitRenderStates(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetTextureStates(bool bForceCommit)
    {
        m_boundTextures.resize(8);
        m_textures.resize(m_boundTextures.size());
        for (unsigned int ts = 0; ts < m_textures.size(); ++ts)
            m_textures[ts] = nullptr;

        CommitTextures(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetBoundTextureStates()
    {
        vecBoundTextures tmpCopy = m_textures;

        ResetTextureStates(true);

        m_textures = tmpCopy;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetSamplerStates(bool bForceCommit)
    {
        m_boundSamplerStates.resize(m_boundTextures.size());
        m_samplerStates.resize(m_boundSamplerStates.size());
        for (unsigned int samplerIndex = 0; samplerIndex < m_samplerStates.size(); ++samplerIndex)
        {
            vecSamplerStates& boundSamplerStates = m_boundSamplerStates[samplerIndex].second;
            vecSamplerStates& samplerStates = m_samplerStates[samplerIndex].second;

            m_boundSamplerStates[samplerIndex].first = false;
            m_samplerStates[samplerIndex].first = false;

            boundSamplerStates.resize(SAMPLERSTATE_TYPE_END);
            samplerStates.resize(SAMPLERSTATE_TYPE_END);

            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_ADDRESSU, TEXTUREADDRESS_TYPE_CLAMP);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_ADDRESSV, TEXTUREADDRESS_TYPE_CLAMP);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_ADDRESSW, TEXTUREADDRESS_TYPE_CLAMP);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_MAGFILTER, TEXTUREFILTER_TYPE_LINEAR);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_MINFILTER, TEXTUREFILTER_TYPE_LINEAR);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_MIPFILTER, TEXTUREFILTER_TYPE_NONE);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_MAXMIPLEVEL, 0);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_MAXANISOTROPY, 16);
            SetSamplerState(samplerIndex, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        }

        CommitSamplers(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetRenderTargets(bool bForceCommit)
    {
        m_boundRenderTargets.resize(8);
        m_renderTargets.resize(m_boundRenderTargets.size());
        for (vecBoundRenderTargets::iterator it = m_renderTargets.begin(); it != m_renderTargets.end(); ++it)
        {
            *it = nullptr;
        }

        m_pDepthStencilTarget = nullptr;

        CommitRenderTargets(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetVertexBuffers(bool bForceCommit)
    {
        m_boundVertexBuffers.resize(8);
        m_vertexBuffers.resize(8);
        for (vecBoundVertexBuffers::iterator it = m_vertexBuffers.begin(); it != m_vertexBuffers.end(); ++it)
        {
            *it = nullptr;
        }

        m_pVertexDeclaration = nullptr;

        CommitVertexBuffers(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetIndexBuffer(bool bForceCommit)
    {
        m_pIndexBuffer = 0;

        CommitIndexBuffer(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::ResetShader(bool bForceCommit)
    {
        m_pShader = 0;

        CommitShader(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::InitDefaultRenderStates(D3D11_DEPTH_STENCIL_DESC& depthStencilDesc)
    {
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        depthStencilDesc.StencilEnable = FALSE;
        depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::InitDefaultRenderStates(D3D11_BLEND_DESC& blendDesc)
    {
        blendDesc.AlphaToCoverageEnable = FALSE;
        blendDesc.IndependentBlendEnable = FALSE;
        blendDesc.RenderTarget[0].BlendEnable = FALSE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::InitDefaultRenderStates(D3D11_RASTERIZER_DESC& rasterizerDesc)
    {
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;
        rasterizerDesc.FrontCounterClockwise = FALSE;
        rasterizerDesc.DepthClipEnable = FALSE;
        rasterizerDesc.MultisampleEnable = FALSE;
        rasterizerDesc.AntialiasedLineEnable = FALSE;
        rasterizerDesc.ScissorEnable = FALSE;
        rasterizerDesc.DepthBias = 0;
        rasterizerDesc.SlopeScaledDepthBias = 0.0f;
        rasterizerDesc.DepthBiasClamp = 0.0f;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::InitDefaultSamplerStates(D3D11_SAMPLER_DESC& samplerDesc)
    {
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.MinLOD = -FLT_MAX;
        samplerDesc.MaxLOD = FLT_MAX;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 16;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.BorderColor[0] = 0.0f;
        samplerDesc.BorderColor[1] = 0.0f;
        samplerDesc.BorderColor[2] = 0.0f;
        samplerDesc.BorderColor[3] = 0.0f;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::InitVertexIndexBufferDesc(D3D11_BUFFER_DESC& bufferDesc, unsigned int length, unsigned int bindFlags, unsigned long usage)
    {
        //         ENV_ASSERT_DBG(!(usage & USAGE_TYPE_RENDERTARGET));
        //         ENV_ASSERT_DBG(!(usage & USAGE_TYPE_AUTOGENMIPMAP));

        //         if (usage & USAGE_TYPE_WRITEONLY)
        //             usage |= USAGE_TYPE_DYNAMIC;

        bufferDesc.ByteWidth = length;
        bufferDesc.BindFlags = bindFlags;
        bufferDesc.Usage = ConvertToD3D11Usage(usage);
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        if ((usage & USAGE_TYPE_CPUREADACCESS) &&
            (usage & USAGE_TYPE_CPUWRITEACCESS))
        {
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
        }
        else if (usage & USAGE_TYPE_CPUWRITEACCESS)
        {
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        }
        else
        {
            bufferDesc.CPUAccessFlags = 0;
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitStates(bool bForceCommit)
    {
        CommitRenderStates(bForceCommit);
        CommitRenderTargets(bForceCommit);
        CommitTextures(bForceCommit);
        CommitSamplers(bForceCommit);
        CommitVertexBuffers(bForceCommit);
        CommitIndexBuffer(bForceCommit);
        CommitShader(bForceCommit);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitRenderStates(bool bForceCommit)
    {
        bool bUpdateDepthStencilRS = bForceCommit;
        bool bUpdateRasterizerRS = bForceCommit;
        bool bUpdateBlendRS = bForceCommit;

        for (unsigned int rs = 0; rs < m_renderStates.size(); ++rs)
        {
            if (bForceCommit || m_renderStates[rs] != m_boundRenderStates[rs])
            {
                // Update render state
                RENDERSTATE_TYPE renderState = (RENDERSTATE_TYPE)rs;

                if (rs == RENDERSTATE_TYPE_ALPHABLENDENABLE ||
                    rs == RENDERSTATE_TYPE_BLENDOP ||
                    rs == RENDERSTATE_TYPE_BLENDOPALPHA ||
                    rs == RENDERSTATE_TYPE_SRCBLEND ||
                    rs == RENDERSTATE_TYPE_SRCBLENDALPHA ||
                    rs == RENDERSTATE_TYPE_DESTBLEND ||
                    rs == RENDERSTATE_TYPE_DESTBLENDALPHA)
                {
                    bUpdateBlendRS = true;
                }
                else if (rs == RENDERSTATE_TYPE_CULLMODE ||
                         rs == RENDERSTATE_TYPE_FILLMODE ||
                         rs == RENDERSTATE_TYPE_SCISSORTESTENABLE)
                {
                    bUpdateRasterizerRS = true;
                }
                else if (rs == RENDERSTATE_TYPE_ZENABLE ||
                         rs == RENDERSTATE_TYPE_ZWRITEENABLE ||
                         rs == RENDERSTATE_TYPE_ZFUNC ||
                         rs == RENDERSTATE_TYPE_STENCILENABLE ||
                         rs == RENDERSTATE_TYPE_STENCILZFAIL ||
                         rs == RENDERSTATE_TYPE_STENCILFAIL ||
                         rs == RENDERSTATE_TYPE_STENCILPASS ||
                         rs == RENDERSTATE_TYPE_STENCILFUNC ||
                         rs == RENDERSTATE_TYPE_STENCILREF)
                {
                    bUpdateDepthStencilRS = true;
                }
            }

            // Set new renderstate as currently bound renderstate
            m_boundRenderStates[rs] = m_renderStates[rs];
        }

        if (bUpdateBlendRS || bForceCommit)
        {
            D3D11_BLEND_DESC blendDesc;
            InitDefaultRenderStates(blendDesc);

            blendDesc.RenderTarget[0].BlendEnable = m_renderStates[RENDERSTATE_TYPE_ALPHABLENDENABLE];
            blendDesc.RenderTarget[0].BlendOp = static_cast<D3D11_BLEND_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_BLENDOP, m_renderStates[RENDERSTATE_TYPE_BLENDOP]));
            blendDesc.RenderTarget[0].BlendOpAlpha = static_cast<D3D11_BLEND_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_BLENDOPALPHA, m_renderStates[RENDERSTATE_TYPE_BLENDOPALPHA]));
            blendDesc.RenderTarget[0].DestBlend = static_cast<D3D11_BLEND>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_DESTBLEND, m_renderStates[RENDERSTATE_TYPE_DESTBLEND]));
            blendDesc.RenderTarget[0].SrcBlend = static_cast<D3D11_BLEND>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_SRCBLEND, m_renderStates[RENDERSTATE_TYPE_SRCBLEND]));
            blendDesc.RenderTarget[0].SrcBlendAlpha = static_cast<D3D11_BLEND>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_SRCBLENDALPHA, m_renderStates[RENDERSTATE_TYPE_SRCBLENDALPHA]));
            blendDesc.RenderTarget[0].DestBlendAlpha = static_cast<D3D11_BLEND>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_DESTBLENDALPHA, m_renderStates[RENDERSTATE_TYPE_DESTBLENDALPHA]));
            blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

            ID3D11BlendState* pState = nullptr;
            ENV_DXCALL(m_pDevice->CreateBlendState(&blendDesc, &pState));

            FLOAT blendFactors[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            UINT sampleMask = 0xFFFFFFFF;

            m_pDeviceContext->OMSetBlendState(pState, blendFactors, sampleMask);

            unsigned int numStates = m_createdBlendStates.size();
            m_createdBlendStates.insert(pState);
            if (m_createdBlendStates.size() == numStates)
                ENV_RELEASE(pState);
        }

        if (bUpdateRasterizerRS || bForceCommit)
        {
            //             RasterizerState rasterizerState;
            //             InitDefaultRenderStates(rasterizerState);
            // 
            //             std::set< RasterizerState* >::iterator it = m_createdRasterizerStates.find(&rasterizerState);
            // 
            //             if (it != m_createdRasterizerStates.end())
            //             {
            //                 ID3D11RasterizerState* pState = nullptr;
            //                 ENV_DXCALL(m_pDevice->CreateRasterizerState(&rasterizerState, &pState));
            // 
            //                 m_pDeviceContext->RSSetState(pState);
            // 
            //                 m_createdRasterizerStates.insert(new rasterizerState);
            // 
            //             }
            D3D11_RASTERIZER_DESC rasterizerDesc;
            InitDefaultRenderStates(rasterizerDesc);

            rasterizerDesc.CullMode = static_cast<D3D11_CULL_MODE>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_CULLMODE, m_renderStates[RENDERSTATE_TYPE_CULLMODE]));
            rasterizerDesc.FillMode = static_cast<D3D11_FILL_MODE>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_FILLMODE, m_renderStates[RENDERSTATE_TYPE_FILLMODE]));
            rasterizerDesc.ScissorEnable = static_cast<BOOL>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_SCISSORTESTENABLE, m_renderStates[RENDERSTATE_TYPE_SCISSORTESTENABLE]));

            ID3D11RasterizerState* pState = nullptr;
            ENV_DXCALL(m_pDevice->CreateRasterizerState(&rasterizerDesc, &pState));

            m_pDeviceContext->RSSetState(pState);

            unsigned int numStates = m_createdRasterizerStates.size();
            m_createdRasterizerStates.insert(pState);
            if (m_createdRasterizerStates.size() == numStates)
                ENV_RELEASE(pState);
        }

        if (bUpdateDepthStencilRS || bForceCommit)
        {
            D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
            InitDefaultRenderStates(depthStencilDesc);

            depthStencilDesc.DepthEnable = static_cast<BOOL>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_ZENABLE, m_renderStates[RENDERSTATE_TYPE_ZENABLE]));
            depthStencilDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_ZWRITEENABLE, m_renderStates[RENDERSTATE_TYPE_ZWRITEENABLE]));
            depthStencilDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_ZFUNC, m_renderStates[RENDERSTATE_TYPE_ZFUNC]));
            depthStencilDesc.StencilEnable = static_cast<BOOL>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILENABLE, m_renderStates[RENDERSTATE_TYPE_STENCILENABLE]));
            depthStencilDesc.StencilReadMask = static_cast<UINT8>(m_renderStates[RENDERSTATE_TYPE_STENCILREADMASK]);
            depthStencilDesc.StencilWriteMask = static_cast<UINT8>(m_renderStates[RENDERSTATE_TYPE_STENCILWRITEMASK]);
            depthStencilDesc.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILZFAIL, m_renderStates[RENDERSTATE_TYPE_STENCILZFAIL]));
            depthStencilDesc.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILFAIL, m_renderStates[RENDERSTATE_TYPE_STENCILFAIL]));
            depthStencilDesc.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILPASS, m_renderStates[RENDERSTATE_TYPE_STENCILPASS]));
            depthStencilDesc.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILFUNC, m_renderStates[RENDERSTATE_TYPE_STENCILFUNC]));
            depthStencilDesc.BackFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILZFAIL, m_renderStates[RENDERSTATE_TYPE_STENCILZFAIL]));
            depthStencilDesc.BackFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILFAIL, m_renderStates[RENDERSTATE_TYPE_STENCILFAIL]));
            depthStencilDesc.BackFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILPASS, m_renderStates[RENDERSTATE_TYPE_STENCILPASS]));
            depthStencilDesc.BackFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(ConvertToD3D11RenderState(RENDERSTATE_TYPE_STENCILFUNC, m_renderStates[RENDERSTATE_TYPE_STENCILFUNC]));

            ID3D11DepthStencilState* pState = nullptr;
            ENV_DXCALL(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &pState));

            m_pDeviceContext->OMSetDepthStencilState(pState, m_renderStates[RENDERSTATE_TYPE_STENCILREF]);

            unsigned int numStates = m_createdDepthStencilStates.size();
            m_createdDepthStencilStates.insert(pState);
            if (m_createdDepthStencilStates.size() == numStates)
                ENV_RELEASE(pState);
        }

    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitTextures(bool bForceCommit)
    {
        ID3D11ShaderResourceView* srv[8] = { nullptr };

        for (unsigned int textureIndex = 0; textureIndex < m_textures.size(); ++textureIndex)
        {
            if (bForceCommit || m_textures[textureIndex] != m_boundTextures[textureIndex])
            {
                if (m_textures[textureIndex] == nullptr)
                {
                    // Reset texture slot to zero
                    srv[textureIndex] = nullptr;
                }
                else
                {
                    srv[textureIndex] = m_textures[textureIndex]->GetSRV();
                }
            }
            else if (m_boundTextures[textureIndex])
            {
                srv[textureIndex] = m_boundTextures[textureIndex]->GetSRV();
            }

            m_boundTextures[textureIndex] = m_textures[textureIndex];
        }

        m_pDeviceContext->PSSetShaderResources(0, 8, srv);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitRenderTargets(bool bForceCommit)
    {
        ID3D11RenderTargetView* renderTargetView[8] = { nullptr };
        ID3D11DepthStencilView* depthStencilView = nullptr;

        bool bCommit = false;

        // Check for changed rendertargets
        for (unsigned int renderTargetIndex = 0; renderTargetIndex < m_renderTargets.size(); ++renderTargetIndex)
        {
            if (bForceCommit || m_renderTargets[renderTargetIndex] != m_boundRenderTargets[renderTargetIndex])
                bCommit = true;

            m_boundRenderTargets[renderTargetIndex] = m_renderTargets[renderTargetIndex];

            if (m_boundRenderTargets[renderTargetIndex])
                renderTargetView[renderTargetIndex] = m_boundRenderTargets[renderTargetIndex]->GetRenderTargetView();
            else
                renderTargetView[renderTargetIndex] = nullptr;
        }

        // Check for changed depthstenciltarget
        if (bForceCommit || m_pDepthStencilTarget != m_pBoundDepthStencilTarget)
        {
            m_pBoundDepthStencilTarget = m_pDepthStencilTarget;

            if (m_pBoundDepthStencilTarget)
                depthStencilView = m_pBoundDepthStencilTarget->GetDepthStencilView();

            bCommit = true;
        }
        else if (m_pBoundDepthStencilTarget)
        {
            depthStencilView = m_pBoundDepthStencilTarget->GetDepthStencilView();
        }

        if (bCommit)
        {
            D3D11_VIEWPORT viewports[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

            for (unsigned int renderTargetIndex = 0; renderTargetIndex < m_renderTargets.size(); ++renderTargetIndex)
            {
                if (m_renderTargets[renderTargetIndex])
                {
                    viewports[renderTargetIndex].Width = static_cast<FLOAT>(m_renderTargets[renderTargetIndex]->GetWidth());
                    viewports[renderTargetIndex].Height = static_cast<FLOAT>(m_renderTargets[renderTargetIndex]->GetHeight());
                }
            }

            m_pDeviceContext->RSSetViewports(8, viewports);
            m_pDeviceContext->OMSetRenderTargets(8, renderTargetView, depthStencilView);
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitVertexBuffers(bool bForceCommit)
    {
        for (unsigned int vertexBufferIndex = 0; vertexBufferIndex < m_boundVertexBuffers.size(); ++vertexBufferIndex)
        {
            if (bForceCommit || m_vertexBuffers[vertexBufferIndex] != m_boundVertexBuffers[vertexBufferIndex])
            {
                if (m_vertexBuffers[vertexBufferIndex])
                {
                    ENV_ASSERT_DBG(m_pVertexDeclaration);
                    m_vertexBuffers[vertexBufferIndex]->Bind(vertexBufferIndex, 0, m_pVertexDeclaration->GetStride());
                }
                else
                {
                    ID3D11Buffer* nullVB = { nullptr };
                    UINT stride = 0;
                    UINT offset = 0;

                    m_pDeviceContext->IASetVertexBuffers(vertexBufferIndex, 1, &nullVB, &stride, &offset);
                }
            }

            m_boundVertexBuffers[vertexBufferIndex] = m_vertexBuffers[vertexBufferIndex];
        }

        if (bForceCommit || m_pBoundVertexDeclaration != m_pVertexDeclaration)
        {
            if (m_pVertexDeclaration)
            {
                m_pVertexDeclaration->Bind();
                m_pBoundVertexDeclaration = m_pVertexDeclaration;
            }
            else
            {
                m_pDeviceContext->IASetInputLayout(nullptr);
                m_pBoundVertexDeclaration = nullptr;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitIndexBuffer(bool bForceCommit)
    {
        if (bForceCommit || m_pIndexBuffer != m_pBoundIndexBuffer)
        {
            if (m_pIndexBuffer != nullptr)
                m_pIndexBuffer->Bind();
            else
                m_pDeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0);

            m_pBoundIndexBuffer = m_pIndexBuffer;
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitShader(bool bForceCommit)
    {
        if (m_pShader)
            m_pShader->CompileConstantBuffers();

        if (bForceCommit || m_pShader != m_pBoundShader)
        {
            if (m_pShader != nullptr)
            {
                std::vector<CShader::vecConstantBuffersGPU>& constantBuffers = m_pShader->GetConstantBuffers();

                CShader::vecConstantBuffersGPU& vertexUniforms = constantBuffers[CShader::SHADER_TYPE_VERTEX];
                CShader::vecConstantBuffersGPU& pixelUniforms = constantBuffers[CShader::SHADER_TYPE_PIXEL];
                CShader::vecConstantBuffersGPU& geometryUniforms = constantBuffers[CShader::SHADER_TYPE_GEOMETRY];
                CShader::vecConstantBuffersGPU& computeUniforms = constantBuffers[CShader::SHADER_TYPE_COMPUTE];

                if (!vertexUniforms.empty())
                    m_pDeviceContext->VSSetConstantBuffers(0, vertexUniforms.size(), &vertexUniforms[0]);

                if (!pixelUniforms.empty())
                    m_pDeviceContext->PSSetConstantBuffers(0, pixelUniforms.size(), &pixelUniforms[0]);

                if (!geometryUniforms.empty())
                    m_pDeviceContext->GSSetConstantBuffers(0, geometryUniforms.size(), &geometryUniforms[0]);

                if (!computeUniforms.empty())
                    m_pDeviceContext->CSSetConstantBuffers(0, computeUniforms.size(), &computeUniforms[0]);
            }
            else
            {
                m_pDeviceContext->VSSetShader(nullptr, 0, 0);
                m_pDeviceContext->PSSetShader(nullptr, 0, 0);
                m_pDeviceContext->HSSetShader(nullptr, 0, 0);
                m_pDeviceContext->DSSetShader(nullptr, 0, 0);
                m_pDeviceContext->GSSetShader(nullptr, 0, 0);
                m_pDeviceContext->CSSetShader(nullptr, 0, 0);
            }

            m_pBoundShader = m_pShader;
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::CommitSamplers(bool bForceCommit)
    {
        ENV_ASSERT_DBG(m_samplerStates.size() == m_boundSamplerStates.size());
        ENV_ASSERT_DBG(m_samplerStates.size() == 8);

        ID3D11SamplerState* currentSamplerStates[8];
        //         ID3D11SamplerState* newSamplerStates[8];
        m_pDeviceContext->PSGetSamplers(0, 8, currentSamplerStates);
        // 
        //         D3D11_SAMPLER_DESC currentSamplerDescs[8];
        //         for (unsigned int sd = 0; sd < 8; ++sd)
        //         {
        //             if (currentSamplerStates[sd])
        //                 currentSamplerStates[sd]->GetDesc(&currentSamplerDescs[sd]);
        //             else
        //             {
        //                 InitDefaultSamplerStates(currentSamplerDescs[sd]);
        //                 bForceCommit = true;
        //             }
        //         }

        bool bUpdateSamplers = false;

        ID3D11SamplerState* newSamplerStates[8];

        for (unsigned int s = 0; s < m_samplerStates.size(); ++s)
        {
            bool bUpdateSamplerState = false;

            if ((m_samplerStates[s].first && !m_boundSamplerStates[s].first) || bForceCommit)
            {
                bUpdateSamplerState = true;
            }
            else
            {
                for (unsigned int k = 0; k < m_samplerStates[s].second.size(); ++k)
                {
                    if (m_boundSamplerStates[s].second[k] != m_samplerStates[s].second[k])
                    {
                        bUpdateSamplerState = true;
                        break;
                    }
                }
            }

            if (bUpdateSamplerState || bForceCommit)
            {
                D3D11_SAMPLER_DESC currentSamplerDescs;
                InitDefaultSamplerStates(currentSamplerDescs);

                currentSamplerDescs.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_ADDRESSU, m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSU]));
                currentSamplerDescs.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_ADDRESSV, m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSV]));
                currentSamplerDescs.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_ADDRESSW, m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSW]));
                currentSamplerDescs.MaxLOD = static_cast<FLOAT>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_MAXMIPLEVEL, m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAXMIPLEVEL]));
                currentSamplerDescs.MaxAnisotropy = static_cast<UINT>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_MAXANISOTROPY, m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAXANISOTROPY]));
                currentSamplerDescs.Filter = static_cast<D3D11_FILTER>(ConvertToD3D11SamplerStateFilter(m_samplerStates[s].second[SAMPLERSTATE_TYPE_MINFILTER],
                                                                                                        m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAGFILTER],
                                                                                                        m_samplerStates[s].second[SAMPLERSTATE_TYPE_MIPFILTER]));

                m_pDevice->CreateSamplerState(&currentSamplerDescs, &newSamplerStates[s]);

                // Updates currently bound samplerstates
                for (unsigned int samplerStateType = 0; samplerStateType < SAMPLERSTATE_TYPE_END; ++samplerStateType)
                {
                    m_boundSamplerStates[s].second[samplerStateType] = m_samplerStates[s].second[samplerStateType];
                }

                m_boundSamplerStates[s].first = true;

                unsigned int numStates = m_createdSamplerStates.size();
                m_createdSamplerStates.insert(newSamplerStates[s]);
                if (m_createdSamplerStates.size() == numStates)
                {
                    newSamplerStates[s]->Release();
                }

                bUpdateSamplers = true;
            }
            else
            {
                newSamplerStates[s] = currentSamplerStates[s];
            }
        }

        if (bUpdateSamplers || bForceCommit)
        {
            for (unsigned int i = 0; i < 8; ++i)
            {
                D3D11_SAMPLER_DESC samplerDesc;
                newSamplerStates[i]->GetDesc(&samplerDesc);

                D3D11_TEXTURE_ADDRESS_MODE m = samplerDesc.AddressU;
            }

            m_pDeviceContext->PSSetSamplers(0, 8, newSamplerStates);
        }

        for (unsigned int sd = 0; sd < 8; ++sd)
        {
            ENV_RELEASE(currentSamplerStates[sd]);
        }


        // 
        //                 if (m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSU] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSU])
        //                 {
        //                     currentSamplerDescs[s].AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_ADDRESSU, m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSU]));
        //                     bUpdateSamplerState = true;
        //                 }
        //                 if (m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSV] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSV] || bForceCommit)
        //                 {
        //                     currentSamplerDescs[s].AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_ADDRESSV, m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSV]));
        //                     bUpdateSamplerState = true;
        //                 }
        //                 if (m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSW] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSW] || bForceCommit)
        //                 {
        //                     currentSamplerDescs[s].AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_ADDRESSW, m_samplerStates[s].second[SAMPLERSTATE_TYPE_ADDRESSW]));
        //                     bUpdateSamplerState = true;
        //                 }
        //                 if (m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAXMIPLEVEL] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_MAXMIPLEVEL] || bForceCommit)
        //                 {
        //                     currentSamplerDescs[s].MaxLOD = static_cast<FLOAT>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_MAXMIPLEVEL, m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAXMIPLEVEL]));
        //                     bUpdateSamplerState = true;
        //                 }
        //                 if (m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAXANISOTROPY] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_MAXANISOTROPY] || bForceCommit)
        //                 {
        //                     currentSamplerDescs[s].MaxAnisotropy = static_cast<UINT>(ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE_MAXANISOTROPY, m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAXANISOTROPY]));
        //                     bUpdateSamplerState = true;
        //                 }
        //                 if (m_samplerStates[s].second[SAMPLERSTATE_TYPE_MINFILTER] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_MINFILTER] ||
        //                     m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAGFILTER] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_MAGFILTER] ||
        //                     m_samplerStates[s].second[SAMPLERSTATE_TYPE_MIPFILTER] != m_boundSamplerStates[s].second[SAMPLERSTATE_TYPE_MIPFILTER] || bForceCommit)
        //                 {
        //                     currentSamplerDescs[s].Filter = static_cast<D3D11_FILTER>(ConvertToD3D11SamplerStateFilter(
        //                         m_samplerStates[s].second[SAMPLERSTATE_TYPE_MINFILTER],
        //                         m_samplerStates[s].second[SAMPLERSTATE_TYPE_MAGFILTER],
        //                         m_samplerStates[s].second[SAMPLERSTATE_TYPE_MIPFILTER]));
        //                     bUpdateSamplerState = true;
        //                 }

        //             if (bUpdateSamplerState || bForceCommit)
        //             {
        //                 m_pDevice->CreateSamplerState(&currentSamplerDescs, &newSamplerStates[s]);
        // 
        //                 // Updates currently bound samplerstates
        //                 for (unsigned int samplerStateType = 0; samplerStateType < SAMPLERSTATE_TYPE_END; ++samplerStateType)
        //                 {
        //                     m_boundSamplerStates[s].second[samplerStateType] = m_samplerStates[s].second[samplerStateType];
        //                 }
        // 
        //                 m_boundSamplerStates[s].first = true;
        // 
        //                 unsigned int numStates = m_createdSamplerStates.size();
        //                 m_createdSamplerStates.insert(newSamplerStates[s]);
        //                 if (m_createdSamplerStates.size() == numStates)
        //                 {
        //                     newSamplerStates[s]->Release();
        //                 }
        // 
        //                 bUpdateSamplers = true;
        //             }
        //             else
        //             {
        //                 newSamplerStates[s] = currentSamplerStates[s];
        //             }
        //         }
        // 
        //         if (bUpdateSamplers || bForceCommit)
        //         {
        //             m_pDeviceContext->PSSetSamplers(0, 8, newSamplerStates);
        //         }
        // 
        //         for (unsigned int sd = 0; sd < 8; ++sd)
        //         {
        //             ENV_RELEASE(currentSamplerStates[sd]);
        //         }
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetRenderState(RENDERSTATE_TYPE rs, unsigned long value)
    {
        m_renderStates[rs] = value;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetSamplerState(unsigned long Sampler, SAMPLERSTATE_TYPE Type, unsigned long Value)
    {
        m_samplerStates[Sampler].first = true;
        m_samplerStates[Sampler].second[Type] = Value;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetTexture(unsigned long index, CTextureBase* pTexture)
    {
        m_textures[index] = pTexture;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetVertexBuffer(unsigned int streamNumber, unsigned int offset, CVertexBuffer* pVertexBuffer)
    {
        m_vertexBuffers[streamNumber] = pVertexBuffer;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetVertexDeclaration(CVertexDeclaration* pVertexDeclaration)
    {
        m_pVertexDeclaration = pVertexDeclaration;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetIndexBuffer(CIndexBuffer* pIndexBuffer)
    {
        m_pIndexBuffer = pIndexBuffer;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetShader(CShader* pShader)
    {
        m_pShader = pShader;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetRenderTarget(unsigned long index, CRenderTarget* pRenderTarget)
    {
        m_renderTargets[index] = pRenderTarget;
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetDepthStencilTarget(CRenderTarget* pRenderTarget)
    {
        //         ENV_ASSERT_DBG(pRenderTarget);
        //         ENV_ASSERT_DBG(pRenderTarget->GetDepthStencilView());

        m_pDepthStencilTarget = pRenderTarget;
    }

    void CRenderDevice::SetViewport(const Vec2& pos, unsigned int width, unsigned int height)
    {
        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = pos[0];
        viewport.TopLeftY = pos[1];
        viewport.Width = static_cast<FLOAT>(width);
        viewport.Height = static_cast<FLOAT>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_pDeviceContext->RSSetViewports(1, &viewport);
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::SetTextureSampler(unsigned long index, const CTextureSampler& sampler)
    {
        SetSamplerState(index, SAMPLERSTATE_TYPE_ADDRESSU, sampler.GetAddressU());
        SetSamplerState(index, SAMPLERSTATE_TYPE_ADDRESSV, sampler.GetAddressV());
        SetSamplerState(index, SAMPLERSTATE_TYPE_ADDRESSW, sampler.GetAddressW());
        SetSamplerState(index, SAMPLERSTATE_TYPE_MINFILTER, sampler.GetMinFilter());
        SetSamplerState(index, SAMPLERSTATE_TYPE_MAGFILTER, sampler.GetMagFilter());
        SetSamplerState(index, SAMPLERSTATE_TYPE_MIPFILTER, sampler.GetMipFilter());

        if (sampler.GetMinFilter() == TEXTUREFILTER_TYPE_ANISOTROPIC)
            SetSamplerState(index, SAMPLERSTATE_TYPE_MAXANISOTROPY, sampler.GetMaxAnisotropy());
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::BeginPerformanceMarker(const std::string& strName)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        const std::wstring strNameUnicode = converter.from_bytes(strName);

        m_pPerformanceMarker->BeginEvent(strNameUnicode.c_str());
    }

    //-----------------------------------------------------------------------------------
    void CRenderDevice::EndPerformanceMarker()
    {
        m_pPerformanceMarker->EndEvent();
    }

} // env
#endif // ENV_GRAPHIC_API_D3D11
