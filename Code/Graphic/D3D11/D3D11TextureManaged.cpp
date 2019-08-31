#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11TextureManaged.h>
#include <Graphic/D3D11/D3D11RenderDevice.h>
#include <Graphic/DirectXTK/WICTextureLoader.h>
#include <Graphic/DirectXTK/DDSTextureLoader.h>
#include <Common/Core/FilePackageManager.h>

#include <locale>
#include <codecvt>

namespace env
{
    //-----------------------------------------------------------------------------------
    CTextureManaged::TextureAllocationPolicy::TextureAllocationPolicy()
        : m_pDevice(nullptr)
        , m_bSRGB(true) {}

    //-----------------------------------------------------------------------------------
    CTextureManaged::TextureAllocationPolicy::TextureAllocationPolicy(CRenderDevice* pDevice, bool bSRGB)
        : m_pDevice(pDevice)
        , m_bSRGB(bSRGB) {}

    CTextureManaged::TextureAllocationPolicy::TextureAllocationPolicy(const TextureAllocationPolicy& other, bool bSRGB)
    {
        TextureAllocationPolicy temp;
        temp.m_pDevice = other.m_pDevice;
        temp.m_bSRGB = bSRGB;

        std::swap(*this, temp);
    }

    //-----------------------------------------------------------------------------------
    CTextureManaged* CTextureManaged::TextureAllocationPolicy::Create()
    {
        CTextureManaged* pTex = m_pDevice->CreateTexture();

        pTex->SetSRGB(m_bSRGB);

        return pTex;
    }

    //-----------------------------------------------------------------------------------
    std::string CTextureManaged::GetNormalMapFilename(const std::string& strTexture)
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
    std::string CTextureManaged::GetSpecularMapFilename(const std::string& strTexture)
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
    std::string CTextureManaged::GetEmissiveMapFilename(const std::string& strTexture)
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

    //-----------------------------------------------------------------------------------
    CTextureManaged::CTextureManaged(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
        : CTextureBase(pDevice, pDeviceContext)
        , m_bSRGB(true)
        , m_bAlpha(true){}

    //-----------------------------------------------------------------------------------
    CTextureManaged::~CTextureManaged()
    {
        ENV_RELEASE(m_pTextureSRV);
        ENV_RELEASE(m_pTexture);
    }

    //-----------------------------------------------------------------------------------
    bool CTextureManaged::LoadResource()
    {
        Unload();

        if (m_strFilename.empty())
            return true;

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        const std::wstring strInputFilenameUnicode = converter.from_bytes(m_strFilename);

        std::string strFileExtension;
        GetFileExtension(m_strFilename, strFileExtension);

        std::vector<char> textureData;

        gTexturePackageMgr->LoadFile(m_strFilename, textureData);

        if (strFileExtension == ".dds")
        {
#ifdef ENV_USE_FILE_PACKAGES
            if (textureData.empty() || 
                FAILED(DirectX::CreateDDSTextureFromMemoryEx(m_pDevice, m_pDeviceContext, (const uint8_t*)&textureData[0], textureData.size(), 0,
                                                             D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, m_bSRGB, &m_pTexture, &m_pTextureSRV)))
            {
                ENV_LOG_ERROR("Could not load texture: " + m_strFilename);
                return false;
            }
#else
            if (FAILED(DirectX::CreateDDSTextureFromFileEx(m_pDevice, m_pDeviceContext, strInputFilenameUnicode.c_str(),
                                                           0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, m_bSRGB, &m_pTexture, &m_pTextureSRV)))
            {
                ENV_LOG_ERROR("Could not load texture: " + m_strFilename);
                return false;
            }
#endif // ENV_USE_FILE_PACKAGES

        }
        else
        {
#ifdef ENV_USE_FILE_PACKAGES
            if (textureData.empty() || FAILED(DirectX::CreateWICTextureFromMemoryEx(m_pDevice, m_pDeviceContext, (const uint8_t*)&textureData[0], textureData.size(), 0,
                                                             D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, m_bSRGB, &m_pTexture, &m_pTextureSRV)))
            {
                ENV_LOG_ERROR("Could not load texture: " + m_strFilename);
                return false;
            }
#else
            if (FAILED(DirectX::CreateWICTextureFromFileEx(m_pDevice, m_pDeviceContext, strInputFilenameUnicode.c_str(), 0,
                D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, m_bSRGB, &m_pTexture, &m_pTextureSRV)))
            {
                ENV_LOG_ERROR("Could not load texture: " + m_strFilename);
                return false;
            }
#endif // ENV_USE_FILE_PACKAGES

        }

        D3D11_RESOURCE_DIMENSION textureType;
        m_pTexture->GetType(&textureType);

        if (textureType == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
        {
            ID3D11Texture2D* pTmpTex = static_cast<ID3D11Texture2D*>(m_pTexture);

            D3D11_TEXTURE2D_DESC textureDesc;
            pTmpTex->GetDesc(&textureDesc);

            m_width = textureDesc.Width;
            m_height = textureDesc.Height;
        }
        else if (textureType == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
        {
            ID3D11Texture3D* pTmpTex = static_cast<ID3D11Texture3D*>(m_pTexture);

            D3D11_TEXTURE3D_DESC textureDesc;
            pTmpTex->GetDesc(&textureDesc);

            m_width = textureDesc.Width;
            m_height = textureDesc.Height;
        }


        ENV_ASSERT_DBG(m_pTexture);
        ENV_ASSERT_DBG(m_pTextureSRV);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CTextureManaged::InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt)
    {
        m_strFilename = strName;
        m_strName = str;

        if (!LoadResource())
            return false;

        GetFileLastWriteTime(m_strFilename, pFt);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CTextureManaged::Unload()
    {
        ENV_RELEASE(m_pTexture);
        ENV_RELEASE(m_pTextureSRV);

        return true;
    }

} // env

#endif // ENV_GRAPHIC_API_D3D11
