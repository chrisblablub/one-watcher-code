#include <Engine/Entity/TextEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#include <Graphic/Core/RenderQueue.h>
#include <locale>
#include <codecvt>

namespace env
{
    ENTITY_TYPEID CTextEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_TEXT;

    CTextEntity::CTextEntity()
        : m_fontSize(32)
        , m_bItalic(false)
        , m_bBold(false)
        , m_textAlignment(TEXT_ALIGNMENT_LEFT)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CTextEntity::~CTextEntity()
    {

    }

    void CTextEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        SetMaterialFilename("Sprites/text.png");
    }

    void CTextEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        m_textRenderObject.SetText(m_strText);
    }

    void CTextEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    void CTextEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }

    void CTextEntity::SetText(const std::string& strText)
    {
        m_strText = strText; 

        m_textRenderObject.SetText(m_strText);
    }

    void CTextEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        CSpriteEntity::AddToRenderQueue(dt, renderQueue);

        if (m_strText.empty() || m_strFont.empty())
            return;

        if (GetScreenSpace() && gEngine->GetEntityMgr()->GetActiveEntity() != this)
            return;

        m_textRenderObject.m_bScreenSpace = m_bScreenSpace;
        m_textRenderObject.m_bBillboard = m_bBillboard;
        m_textRenderObject.m_color = m_sprite.GetColor();
        m_textRenderObject.m_alignment = m_textAlignment;
        m_textRenderObject.m_mtxTransform = m_bScreenSpace ? m_nodeScreenSpace.GetDerivedRST() : m_pNode->GetDerivedRST();
        
        renderQueue.m_textRenderObjects[FontInfo(m_strFont, m_fontSize, m_bItalic, m_bBold)].push_back(m_textRenderObject);
    }
} // env