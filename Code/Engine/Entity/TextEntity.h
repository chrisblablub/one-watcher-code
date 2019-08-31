/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TEXT_ENTITY_H
#define TEXT_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Graphic/RenderObjects/TextRenderObject.h>

namespace env
{

    class CTextEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CTextEntity();
        virtual ~CTextEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);

        void SetText(const std::string& strText);
        const std::string& GetText() const { return m_strText; }

        void SetFont(const std::string& strFont) { m_strFont = strFont; }
        const std::string& GetFont() const { return m_strFont; }

        void SetFontSize(unsigned int fontSize) { m_fontSize = fontSize; }
        unsigned int GetFontSize() const { return m_fontSize; }

        void SetAlignment(TEXT_ALIGNMENT alignment) { m_textAlignment = alignment; }
        TEXT_ALIGNMENT GetAlignment() const { return m_textAlignment; }

        void SetItalic(bool bItalic) { m_bItalic = bItalic; }
        bool GetItalic() const { return m_bItalic; }

        void SetBold(bool bBold) { m_bBold = bBold; }
        bool GetBold() const { return m_bBold; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_strText;
            ar & m_strFont;

            if (version > 0)
            {
                ar & m_textAlignment;
                ar & m_fontSize;
                ar & m_bItalic;
                ar & m_bBold;
            }
        }

    protected:

        TEXT_ALIGNMENT m_textAlignment;

        unsigned int m_fontSize;
        
        bool m_bItalic;
        bool m_bBold;

        FontInfo m_fontInfo;
        TextRenderObject m_textRenderObject;

        std::string m_strText;
        std::string m_strFont;
    };
} // env
#endif // TEXT_ENTITY_H