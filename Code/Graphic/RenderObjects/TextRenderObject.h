/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TEXT_RENDER_OBJECT_H
#define TEXT_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>
#include <locale>
#include <codecvt>

namespace env
{
    class FontInfo
    {
    public:
        FontInfo()
            : m_strFontFamily("CourierNew")
            , m_bItalic(false)
            , m_bBold(false)
            , m_fontSize(32) 
        {
            UpdateKey();
        }

        FontInfo(const std::string& strFontFamily,
                 unsigned int fontSize = 32, 
                 bool bItalic = false, 
                 bool bBold = false)
                 : m_strFontFamily(strFontFamily)
                 , m_fontSize(fontSize)
                 , m_bItalic(bItalic)
                 , m_bBold(bBold)
        {
            UpdateKey();
        }

        ENV_FORCEINLINE std::string GetKey() const { return m_strKey; }
        ENV_FORCEINLINE bool operator<(const FontInfo& rhs)  const { return GetKey() < rhs.GetKey(); }
        ENV_FORCEINLINE bool operator>(const FontInfo& rhs)  const { return rhs < *this; }
        ENV_FORCEINLINE bool operator<=(const FontInfo& rhs) const { return !(rhs < *this); }
        ENV_FORCEINLINE bool operator>=(const FontInfo& rhs) const { return !(rhs > *this); }

        unsigned int GetFontSize() const { return m_fontSize; }

        bool GetItalic() const { return m_bItalic; }
        bool GetBold() const { return m_bBold; }

    private:

        void UpdateKey()
        {
            char buffer[256];
            memset(buffer, '\0', 256);

            m_strFontFamily.erase(std::remove_if(m_strFontFamily.begin(), m_strFontFamily.end(), isspace), m_strFontFamily.end());

            if (m_bItalic && m_bBold)
            {
                sprintf(buffer, "%s_%d_i_b.spritefont", m_strFontFamily.c_str(), m_fontSize);
            }
            else if (m_bItalic)
            {
                sprintf(buffer, "%s_%d_i.spritefont", m_strFontFamily.c_str(), m_fontSize);
            }
            else if (m_bBold)
            {
                sprintf(buffer, "%s_%d_b.spritefont", m_strFontFamily.c_str(), m_fontSize);
            }
            else
            {
                sprintf(buffer, "%s_%d.spritefont", m_strFontFamily.c_str(), m_fontSize);
            }

            m_strKey = std::string(buffer);
        }

        std::string m_strFontFamily;
        std::string m_strKey;

        bool m_bItalic;
        bool m_bBold;

        unsigned int m_fontSize;
    };

    enum TEXT_ALIGNMENT
    {
        TEXT_ALIGNMENT_LEFT = 0,
        TEXT_ALIGNMENT_CENTER,
        TEXT_ALIGNMENT_RIGHT
    };

    struct TextRenderObject
    {
        TextRenderObject()
            : m_color(1.0f, 1.0f, 1.0f, 1.0f)
            , m_mtxTransform(1.0f)
            , m_bScreenSpace(false)
            , m_bBillboard(false)
            , m_alignment(TEXT_ALIGNMENT_LEFT) {}

        TextRenderObject(const std::string& strText)
            : m_color(1.0f, 1.0f, 1.0f, 1.0f)
            , m_mtxTransform(1.0f)
            , m_bScreenSpace(false)
            , m_bBillboard(false)
            , m_alignment(TEXT_ALIGNMENT_LEFT)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            m_strText = converter.from_bytes(strText);
        }

        TextRenderObject(const std::string& strText,
                         const Vec4& color)
                         : m_color(color)
                         , m_mtxTransform(1.0f)
                         , m_bScreenSpace(false)
                         , m_bBillboard(false)
                         , m_alignment(TEXT_ALIGNMENT_LEFT)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            m_strText = converter.from_bytes(strText);
        }

        TextRenderObject(const std::string& strText,
                         const Vec4& color,
                         const Vec2& pos)
                         : m_color(color)
                         , m_bScreenSpace(false)
                         , m_bBillboard(false)
                         , m_alignment(TEXT_ALIGNMENT_LEFT)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            m_strText = converter.from_bytes(strText);

            SetPosition(pos);
        }


        TextRenderObject(const std::wstring& strText)
            : m_strText(strText)
            , m_color(1.0f, 1.0f, 1.0f, 1.0f)
            , m_mtxTransform(1.0f)
            , m_bScreenSpace(false)
            , m_bBillboard(false)
            , m_alignment(TEXT_ALIGNMENT_LEFT) {}

        TextRenderObject(const std::wstring& strText,
                         const Vec4& color)
                         : m_strText(strText)
                         , m_color(color)
                         , m_mtxTransform(1.0f)
                         , m_bScreenSpace(false)
                         , m_bBillboard(false) 
                         , m_alignment(TEXT_ALIGNMENT_LEFT) {}

        TextRenderObject(const std::wstring& strText,
                         const Vec4& color,
                         const Vec2& pos)
                         : m_strText(strText)
                         , m_color(color)
                         , m_bScreenSpace(false)
                         , m_bBillboard(false)
                         , m_alignment(TEXT_ALIGNMENT_LEFT)
        {
            SetPosition(pos);
        }

        void SetText(const std::string& strText)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            m_strText = converter.from_bytes(strText);
        }

        void SetPosition(const Vec2& pos) 
        {
            m_mtxTransform = glm::transpose(glm::translate(Matrix4x4(1.0f), Vec3(pos.x, pos.y, 0.0f)));
        }

        std::wstring m_strText;

        Vec4 m_color;

        bool m_bScreenSpace;
        bool m_bBillboard;

        Matrix4x4 m_mtxTransform;

        TEXT_ALIGNMENT m_alignment;
    };

    typedef std::list< TextRenderObject > listTextRenderObjects;
    typedef std::map< FontInfo, listTextRenderObjects > mapFontTextRenderObjects;

} // env
#endif // TEXT_RENDER_OBJECT_H 