/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_TEXTUREMANAGED_H
#define D3D11_TEXTUREMANAGED_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/D3D11/D3D11TextureBase.h>

namespace env
{
    class CRenderDevice;

    class CTextureManaged : public CTextureBase
    {
    public:
        struct TextureAllocationPolicy
        {
            TextureAllocationPolicy();
            TextureAllocationPolicy(CRenderDevice* pDevice, bool bSRGB = true);
            TextureAllocationPolicy(const TextureAllocationPolicy& other, bool bSRGB);

            CTextureManaged* Create();

            CRenderDevice* m_pDevice;

            bool m_bSRGB;
        };


        typedef CResMgrBase< std::string, CTextureManaged, TextureAllocationPolicy > CTextureMgr;

        friend class CTextureMgr;

        CTextureManaged(ID3D11Device* pDevice,
                        ID3D11DeviceContext* pDeviceContext);

        virtual ~CTextureManaged();

        static std::string GetNormalMapFilename(const std::string& strTexture);
        static std::string GetSpecularMapFilename(const std::string& strTexture);
        static std::string GetEmissiveMapFilename(const std::string& strTexture);

        virtual bool Unload();
        virtual bool LoadResource();

        const std::string& GetName() const { return m_strName; }
        const std::string& GetFilename() const { return m_strFilename; }

        inline void SetAlphaChannel(bool bSet = true) { m_bAlpha = bSet; }
        inline bool HasAlphaChannel() const { return m_bAlpha; }

        // Requires reload of the texture!
        inline void SetSRGB(bool bSRGB = true) { bool bChanged = m_bSRGB != bSRGB; m_bSRGB = bSRGB; if (bChanged) LoadResource(); }
        inline bool IsSRGB() const { return m_bSRGB; }

    private:

        bool InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt);

    protected:
        std::string m_strFilename;
        std::string m_strName;

        bool m_bAlpha;
        bool m_bSRGB;

    };

#define TEXTURE_ALLOC_POLICY(pDevice, bSRGB) CTextureManaged::TextureAllocationPolicy(pDevice, bSRGB)

} // env
#endif // D3D11_TEXTUREMANAGED_H
#endif // ENV_GRAPHIC_API_D3D11
