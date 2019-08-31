
#ifdef D3D9

#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/D3D9/D3D9RenderTarget.h>
#include <Graphic/D3D9/D3D9APITypeConversion.h>
#include <Graphic/D3D9/D3D9TextureBase.h>

namespace env
{

	CD3D9RenderTarget::CD3D9RenderTarget(IDirect3DDevice9* pDevice, int width, int height, TEXTUREFORMAT_TYPE format, bool bMipMaps)
		: CBaseRenderTarget(width, height, format, bMipMaps),
		m_pDevice(pDevice),
		m_pTexture(0),
		m_pSurface(0),
		m_bWriteOnly(false),
		m_unit(-1)
	{
	}

	CD3D9RenderTarget::CD3D9RenderTarget(IDirect3DDevice9* pDevice, IDirect3DSurface9* pSurface)
		: CBaseRenderTarget(0, 0, TEXTUREFORMAT_TYPE_X8R8G8B8),
		m_pDevice(pDevice),
		m_pSurface(pSurface),
		m_pTexture(0),
		m_bWriteOnly(true),
		m_unit(-1)
	{
	}

	CD3D9RenderTarget::~CD3D9RenderTarget()
	{
		if (!m_bWriteOnly)
		{
			ENV_RELEASE(m_pSurface);
            ENV_DELETE(m_pTexture);
		}
		else
		{
			ENV_RELEASE(m_pSurface);
		}
	}


	void CD3D9RenderTarget::OnLostDevice()
	{
		if (!m_bWriteOnly)
		{
			ENV_RELEASE(m_pSurface);
			ENV_DELETE(m_pTexture);
		}
	}

	void CD3D9RenderTarget::OnResetDevice()
	{
		if (!m_bWriteOnly)
		{
			assert(!m_pTexture);

			long usage = USAGE_TYPE_RENDERTARGET;
			if (m_bMipMaps)
				usage |= USAGE_TYPE_AUTOGENMIPMAP;

			int mipLevels = 1;
			if (m_bMipMaps)
				mipLevels = 5;

            IDirect3DTexture9* pTex = 0;
            m_pDevice->CreateTexture(m_width, m_height, 1, ConvertToD3D9Usage(usage), ConvertToD3D9Format(m_format), ConvertToD3D9Pool(MEMORY_TYPE_DEFAULT), &pTex, 0);

            pTex->GetSurfaceLevel(0, &m_pSurface);

            m_pTexture = new CTextureBase(m_pDevice, pTex, m_width, m_height);
		}
	}

    void CD3D9RenderTarget::Bind(int index)
    {
        m_pDevice->SetRenderTarget(index, m_pSurface);
         
        m_unit = index;
    }
} // env

#endif // D3D9