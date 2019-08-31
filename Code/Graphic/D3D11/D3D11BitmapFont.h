/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_BITMAP_FONT_H
#define D3D11_BITMAP_FONT_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/RenderObjects/TextRenderObject.h>
#include <Common/Core/ResMgrBase.h>


namespace env
{
    struct RenderQueue;
    struct SpriteRenderObject;


    class CFont
    {
    public:

        struct Glyph
        {
            uint32_t Character;
            RECT Subrect;
            float XOffset;
            float YOffset;
            float XAdvance;
        };

        struct BitmapFontAllocationPolicy
        {
            BitmapFontAllocationPolicy()
                : m_pDevice(nullptr)
                , m_pDeviceContext(nullptr) {}

            BitmapFontAllocationPolicy(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
                : m_pDevice(pDevice)
                , m_pDeviceContext(pDeviceContext) {}

            CFont* Create() { return new CFont(m_pDevice, m_pDeviceContext); }

            ID3D11Device* m_pDevice;
            ID3D11DeviceContext* m_pDeviceContext;
        };

        typedef CResMgrBase< std::string, CFont, BitmapFontAllocationPolicy > CFontMgr;

        friend class CFontMgr;

    public:

        CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
        CFont(CFont&& moveFrom);
        CFont& operator= (CFont&& moveFrom);

        ~CFont();

        bool InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt);

        void DrawText(const std::string& strText);

        void DrawString(RenderQueue& renderQueue, std::list< SpriteRenderObject >& textSprites, const std::wstring& strText, const TextRenderObject& textRenderObject);
        void OnLostDevice() {}
        void OnResetDevice() {}

        Vec2 MeasureString(const std::wstring& strText);

    private:

        bool LoadResource();

        CFont(CFont const&);
        CFont& operator= (CFont const&);

    private:

        class Impl;

        std::unique_ptr<Impl> pImpl;

        std::string m_strFilename;
        std::string m_strName;

        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pDeviceContext;
    };


    typedef CFont::CFontMgr CFontMgr;

    extern CFontMgr* gFontMgr;

} // env
#endif // D3D11_BITMAP_FONT_H
#endif // ENV_GRAPHIC_API_D3D11
