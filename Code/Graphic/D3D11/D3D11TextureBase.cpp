
#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11TextureBase.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CTextureBase::CTextureBase(ID3D11Device* pDevice,
                               ID3D11DeviceContext* pDeviceContext)
                               : m_pDevice(pDevice)
                               , m_pDeviceContext(pDeviceContext)
                               , m_pTexture(nullptr)
                               , m_pTextureSRV(nullptr)
                               , m_sampler(CDefaultTextureSampler::GetLinearWrapSampler()) {}

    //-----------------------------------------------------------------------------------
    CTextureBase::CTextureBase(ID3D11Device* pDevice,
                               ID3D11DeviceContext* pDeviceContext,
                               ID3D11Texture2D* pTex,
                               ID3D11ShaderResourceView* pSRV,
                               int width, int height)
                               : m_pDevice(pDevice)
                               , m_pDeviceContext(pDeviceContext)
                               , m_pTexture(pTex)
                               , m_pTextureSRV(pSRV)
                               , m_width(width)
                               , m_height(height)
                               , m_sampler(CDefaultTextureSampler::GetLinearWrapSampler()) {}

    //-----------------------------------------------------------------------------------
    CTextureBase::~CTextureBase()
    {
        ENV_RELEASE(m_pTexture);
        ENV_RELEASE(m_pTextureSRV);
    }
} // env
#endif // ENV_GRAPHIC_API_D3D11
