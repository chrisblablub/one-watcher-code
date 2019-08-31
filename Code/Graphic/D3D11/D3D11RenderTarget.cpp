#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/D3D11/D3D11RenderTarget.h>
#include <Graphic/D3D11/D3D11APITypeConversion.h>
#include <Graphic/D3D11/D3D11TextureBase.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    void CRenderTarget::CreateTextureBuffer(int width, int height, TEXTUREFORMAT_TYPE format)
    {
        bool bBindShader = format != TEXTUREFORMAT_TYPE_D24S8 &&
            format != TEXTUREFORMAT_TYPE_INDEX16 &&
            format != TEXTUREFORMAT_TYPE_INDEX32;

        D3D11_TEXTURE2D_DESC backBufferTextureDesc;
        backBufferTextureDesc.Width = width;
        backBufferTextureDesc.Height = height;
        backBufferTextureDesc.MipLevels = 1;
        backBufferTextureDesc.ArraySize = 1;
        backBufferTextureDesc.Format = ConvertToD3D11Format(format);
        backBufferTextureDesc.SampleDesc.Count = 1;
        backBufferTextureDesc.SampleDesc.Quality = 0;
        backBufferTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        backBufferTextureDesc.BindFlags = 0;
        backBufferTextureDesc.CPUAccessFlags = 0;
        backBufferTextureDesc.MiscFlags = 0;

        if (bBindShader)
            backBufferTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        ENV_DXCALL(m_pDevice->CreateTexture2D(&backBufferTextureDesc, 0, &m_pTextureBuffer));
    }

    //-----------------------------------------------------------------------------------
    void CRenderTarget::CreateDepthStencilBuffer(unsigned int width, unsigned int height)
    {
        ENV_ASSERT_DBG(m_pDepthStencilBuffer == nullptr);
        ENV_ASSERT_DBG(m_pDepthStencilView == nullptr);

        D3D11_TEXTURE2D_DESC backBufferTextureDesc;
        backBufferTextureDesc.Width = width;
        backBufferTextureDesc.Height = height;
        backBufferTextureDesc.MipLevels = 1;
        backBufferTextureDesc.ArraySize = 1;
        backBufferTextureDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        backBufferTextureDesc.SampleDesc.Count = 1;
        backBufferTextureDesc.SampleDesc.Quality = 0;
        backBufferTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        backBufferTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        backBufferTextureDesc.CPUAccessFlags = 0;
        backBufferTextureDesc.MiscFlags = 0;

        ENV_DXCALL(m_pDevice->CreateTexture2D(&backBufferTextureDesc, 0, &m_pDepthStencilBuffer));
        ENV_DXCALL(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));
    }

    //-----------------------------------------------------------------------------------
    CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, int width, int height, TEXTUREFORMAT_TYPE format, bool bCreateDepthStencil, bool bMipMaps)
        : CBaseRenderTarget(width, height, format, bMipMaps)
        , m_pDevice(pDevice)
        , m_pDeviceContext(pDeviceContext)
        , m_pDepthStencilBuffer(nullptr)
        , m_pDepthStencilView(nullptr)
        , m_pTexture(nullptr)
        , m_pTextureRenderTargetView(nullptr)
        , m_pTextureBuffer(nullptr)
        , m_bTextureCreated(true)
        , m_bDepthStencilCreate(bCreateDepthStencil)
        , m_unit(-1)
    {
//         if (m_bTextureCreated)
//         {
//             ENV_ASSERT_DBG(!m_pTextureBuffer);
//             ENV_ASSERT_DBG(!m_pTexture);
// 
//             CreateTextureBuffer(width, height, format);
// 
//             D3D11_SHADER_RESOURCE_VIEW_DESC textureSRVDesc;
//             textureSRVDesc.Format = ConvertToD3D11Format(m_format);
//             textureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//             textureSRVDesc.Texture2D.MipLevels = 1;
//             textureSRVDesc.Texture2D.MostDetailedMip = 0;
// 
//             ID3D11ShaderResourceView* pSRV = nullptr;
//             ENV_DXCALL(m_pDevice->CreateShaderResourceView(m_pTextureBuffer, &textureSRVDesc, &pSRV));
// 
//             m_pTexture = new CTextureBase(m_pDevice, m_pDeviceContext, m_pTextureBuffer, pSRV, m_width, m_height);
//         }
// 
//         ENV_ASSERT_DBG(m_pTextureBuffer);

//         m_pDevice->CreateRenderTargetView(m_pTextureBuffer, 0, &m_pTextureRenderTargetView);
// 
//         if (bCreateDepthStencil)
//         {
//             D3D11_TEXTURE2D_DESC surfaceDesc;
//             m_pTextureBuffer->GetDesc(&surfaceDesc);
// 
//             CreateDepthStencilBuffer(surfaceDesc.Width, surfaceDesc.Height);
//         }
    }

    //-----------------------------------------------------------------------------------
    CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Texture2D* pSurface, bool bCreateDepthStencil)
        : CBaseRenderTarget(0, 0, TEXTUREFORMAT_TYPE_A8R8G8B8)
        , m_pDevice(pDevice)
        , m_pDeviceContext(pDeviceContext)
        , m_pTextureBuffer(pSurface)
        , m_pDepthStencilBuffer(nullptr)
        , m_pDepthStencilView(nullptr)
        , m_pTexture(nullptr)
        , m_pTextureRenderTargetView(nullptr)
        , m_bTextureCreated(false)
        , m_bDepthStencilCreate(bCreateDepthStencil)
        , m_unit(-1)
    {
        D3D11_TEXTURE2D_DESC tex2DDesc;
        pSurface->GetDesc(&tex2DDesc);

        SetWidth(tex2DDesc.Width);
        SetHeight(tex2DDesc.Height);

//         ENV_DXCALL(m_pDevice->CreateRenderTargetView(m_pTextureBuffer, 0, &m_pTextureRenderTargetView));
// 
//         if (bCreateDepthStencil)
//         {
//             D3D11_TEXTURE2D_DESC surfaceDesc;
//             m_pTextureBuffer->GetDesc(&surfaceDesc);
// 
//             CreateDepthStencilBuffer(surfaceDesc.Width, surfaceDesc.Height);
//         }
    }

    //-----------------------------------------------------------------------------------
    CRenderTarget::~CRenderTarget()
    {
        ENV_RELEASE(m_pDepthStencilView);
        ENV_RELEASE(m_pDepthStencilBuffer);
        ENV_RELEASE(m_pTextureRenderTargetView);

        bool bTextureObject = m_pTexture != nullptr;

        ENV_DELETE(m_pTexture);

        if (!m_bTextureCreated || !bTextureObject)
            ENV_RELEASE(m_pTextureBuffer);
    }

    //-----------------------------------------------------------------------------------
    void CRenderTarget::OnLostDevice()
    {
        ENV_RELEASE(m_pDepthStencilView);
        ENV_RELEASE(m_pDepthStencilBuffer);
        ENV_RELEASE(m_pTextureRenderTargetView);

        bool bTextureObject = m_pTexture != nullptr;

        if (m_bTextureCreated)
        {
            ENV_DELETE(m_pTexture);
        }

        if (!bTextureObject)
        {
            ENV_RELEASE(m_pTextureBuffer);
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderTarget::OnResetDevice()
    {
        ENV_RELEASE(m_pTextureRenderTargetView);

        if (m_bTextureCreated)
        {
            CreateTextureBuffer(m_width, m_height, m_format);

            if (m_format != TEXTUREFORMAT_TYPE_D24S8)
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC textureSRVDesc;
                textureSRVDesc.Format = ConvertToD3D11Format(m_format);
                textureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                textureSRVDesc.Texture2D.MipLevels = 1;
                textureSRVDesc.Texture2D.MostDetailedMip = 0;

                ID3D11ShaderResourceView* pSRV = nullptr;
                ENV_DXCALL(m_pDevice->CreateShaderResourceView(m_pTextureBuffer, &textureSRVDesc, &pSRV));

                m_pTexture = new CTextureBase(m_pDevice, m_pDeviceContext, m_pTextureBuffer, pSRV, m_width, m_height);
            }
            
        }

        if (m_format != TEXTUREFORMAT_TYPE_D24S8)
        {
            ENV_ASSERT_DBG(m_pTextureBuffer);
            ENV_DXCALL(m_pDevice->CreateRenderTargetView(m_pTextureBuffer, 0, &m_pTextureRenderTargetView));
            ENV_ASSERT_DBG(m_pTextureRenderTargetView);

        }
        
        if (m_bDepthStencilCreate)
        {
            D3D11_TEXTURE2D_DESC surfaceDesc;
            m_pTextureBuffer->GetDesc(&surfaceDesc);

            CreateDepthStencilBuffer(surfaceDesc.Width, surfaceDesc.Height);
        }
    }
} // env

#endif // ENV_GRAPHIC_API_D3D11
