#ifdef D3D9

#include <Graphic/D3D9/D3D9TextureManaged.h>
#include <Graphic/D3D9/D3D9RenderDevice.h>


namespace env
{
    CTextureMgr* gTextureMgr = NULL;

    CD3D9TextureManaged::TextureAllocationPolicy::TextureAllocationPolicy(CD3D9RenderDevice* pDevice)
        : m_pDevice(pDevice) {}

    CD3D9TextureManaged* CD3D9TextureManaged::TextureAllocationPolicy::Create()
    {
        return m_pDevice->CreateTexture();
    }

    std::string CD3D9TextureManaged::GetNormalMapFilename(const std::string& strTexture)
    {
        std::string strNormalTexName = strTexture;
        size_t pos = strNormalTexName.find_last_of(".");
        if (pos != std::string::npos)
        {
            strNormalTexName.insert(pos, "_nmap");
        }
        else
            strNormalTexName.clear();

        return strNormalTexName;
    }

    //-----------------------------------------------------------------------------------
    std::string CD3D9TextureManaged::GetSpecularMapFilename(const std::string& strTexture)
    {
        std::string strNormalTexName = strTexture;
        size_t pos = strNormalTexName.find_last_of(".");
        if (pos != std::string::npos)
        {
            strNormalTexName.insert(pos, "_specmap");
        }
        else
            strNormalTexName.clear();

        return strNormalTexName;
    }

    //-----------------------------------------------------------------------------------
    std::string CD3D9TextureManaged::GetEmissiveMapFilename(const std::string& strTexture)
    {
        std::string strNormalTexName = strTexture;
        size_t pos = strNormalTexName.find_last_of(".");
        if (pos != std::string::npos)
        {
            strNormalTexName.insert(pos, "_emismap");
        }
        else
            strNormalTexName.clear();

        return strNormalTexName;
    }

    bool CD3D9TextureManaged::LoadResource()
    {
        Unload();

        HRESULT hr = NULL;
        D3DXIMAGE_INFO m_ImgInfo;
        hr = D3DXGetImageInfoFromFile(m_strFilename.c_str(), &m_ImgInfo);
        if (FAILED(hr))
        {
            ENV_LOG_ERROR("Texture not found: " + m_strFilename);
            return false;
        }

        if (m_ImgInfo.ResourceType == D3DRTYPE_TEXTURE)
        {
            IDirect3DTexture9* pTex = 0;
            hr = D3DXCreateTextureFromFileEx(m_pDevice, m_strFilename.c_str(), m_ImgInfo.Width, m_ImgInfo.Height,
                                             D3DX_DEFAULT, 0, m_ImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &pTex);

            m_pTexture = pTex;
            m_width = m_ImgInfo.Width;
            m_height = m_ImgInfo.Height;
        }
        else
        {
            IDirect3DCubeTexture9* pTex = 0;
            hr = D3DXCreateCubeTextureFromFileEx(m_pDevice, m_strFilename.c_str(), m_ImgInfo.Width,
                                                 D3DX_DEFAULT, 0, m_ImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &pTex);

            m_pTexture = pTex;
            m_width = m_ImgInfo.Width;
            m_height = m_ImgInfo.Height;
        }


        if (FAILED(hr))
        {
            ENV_LOG_INFO_DBG("Could not load texture " + m_strFilename);
            return false;
        }
        return true;
    }

    bool CD3D9TextureManaged::InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt)
    {
        m_strFilename = strName;
        m_strName = str;

        if (!LoadResource())
            return false;

        GetFileLastWriteTime(m_strFilename, pFt);

        return true;
    }

    bool CD3D9TextureManaged::Unload()
    {
        ENV_RELEASE(m_pTexture);
        return true;
    }

    const std::string& CD3D9TextureManaged::GetFilename()
    {
        return m_strFilename;
    }

    const std::string& CD3D9TextureManaged::GetName()
    {
        return m_strName;
    }

    void CD3D9TextureManaged::SetAlphaChannel(bool bSet)
    {
        m_bAlpha = bSet;
    }

    bool CD3D9TextureManaged::HasAlphaChannel()
    {
        return m_bAlpha;
    }
} // env

#endif // D3D9