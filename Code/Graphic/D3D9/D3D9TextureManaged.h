/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_TEXTUREMANAGED_H
#define D3D9_TEXTUREMANAGED_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>
#include <Graphic/D3D9/D3D9TextureBase.h>

namespace env
{
    class CD3D9RenderDevice;

    class CD3D9TextureManaged : public CD3D9TextureBase
    {
    public:
        struct TextureAllocationPolicy
        {
            TextureAllocationPolicy() : m_pDevice(0) {}
            TextureAllocationPolicy(CD3D9RenderDevice* pDevice);

            CD3D9TextureManaged* Create();

            CD3D9RenderDevice* m_pDevice;

            bool m_bSRGB;
        };

        typedef CResMgrBase< std::string, CD3D9TextureManaged, TextureAllocationPolicy > CTextureMgr;

        friend class CTextureMgr;

        CD3D9TextureManaged(IDirect3DDevice9* pDevice) 
            : CD3D9TextureBase(pDevice) {}

        virtual ~CD3D9TextureManaged() {}

        static std::string GetNormalMapFilename(const std::string& strTexture);
        static std::string GetSpecularMapFilename(const std::string& strTexture);
        static std::string GetEmissiveMapFilename(const std::string& strTexture);

        virtual bool Unload();
        virtual bool LoadResource();

        const std::string& GetName();
        const std::string& GetFilename();

        void SetAlphaChannel(bool bSet = true);
        bool HasAlphaChannel();

    private:
        bool InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt);

    protected:
        std::string m_strFilename;
        std::string m_strName;

        bool m_bAlpha;
    
    };

    typedef CD3D9TextureManaged CTextureManaged;
    typedef CTextureManaged::CTextureMgr CTextureMgr;
    extern CTextureMgr* gTextureMgr;
} // env
#endif // D3D9_TEXTUREMANAGED_H
#endif // D3D9