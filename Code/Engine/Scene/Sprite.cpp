#include <Engine/Scene/Sprite.h>
#include <Engine/Scene/SceneNode.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CSprite::CSprite()
        : m_pMaterial(nullptr)
        , m_pNode(nullptr)
        , m_flags(0)
        , m_bBillboard(false)
        , m_bAlphaBlending(true)
        , m_bScreenSpace(false)
        , m_bUpdateRect(true)
        , m_bApplyFade(true)
        , m_bApplyPostProcessing(true)
        , m_bDepthTest(true)
        , m_color(0)
        , m_vColor(1.0f, 1.0f, 1.0f, 1.0f)
        , m_vCenter(0.0f, 0.0f, 0.0f)
        , m_vPosition(0.0f, 0.0f, 0.0f)
        , m_vTextPosition(0.0f, 0.0f)
        , m_pipeline(SPRITE_PIPELINE_BEFORE_POSTPROCESSING)
    {
        m_mtxTransform = Matrix4x4(1.0f);
    }

    //-----------------------------------------------------------------------------------
    CSprite::CSprite(const CSprite& other)
    {
        m_bDepthTest = other.m_bDepthTest;
        m_bApplyFade = other.m_bApplyFade;
        m_bApplyPostProcessing = other.m_bApplyPostProcessing;
        m_strMaterial = other.m_strMaterial;
        m_bAlphaBlending = other.m_bAlphaBlending;
        m_bBillboard = other.m_bBillboard;
        m_bScreenSpace = other.m_bScreenSpace;
        m_vPosition = other.m_vPosition;
        m_flags = other.m_flags;
        m_mtxTransform = other.m_mtxTransform;
        m_strText = other.m_strText;
        m_vTextPosition = other.m_vTextPosition;
        m_color = other.m_color;
        m_vColor = other.m_vColor;
        m_vCenter = other.m_vCenter;
        m_logicalRect = other.m_logicalRect;
        m_bUpdateRect = other.m_bUpdateRect;
        m_bMaterialInstance = other.m_bMaterialInstance;
        m_pNode = other.m_pNode;
        m_pipeline = other.m_pipeline;

        if (!m_bMaterialInstance)
        {
            m_pMaterial = other.m_pMaterial;
            if (m_pMaterial)
            {
                // Increase the reference counter on copying, because
                // initialize is not called!
                gMaterialMgr->AddGetPtr(m_pMaterial->GetName());
            }
        }
        else
        {
            if (other.m_pMaterial)
                SetMaterialFilename(m_strMaterial, true);
            else
                m_pMaterial = NULL;
        }
    }

    //-----------------------------------------------------------------------------------
    CSprite::~CSprite()
    {
        if (m_pMaterial)
        {
            if (!m_bMaterialInstance)
                gMaterialMgr->Release(m_pMaterial->GetName());
            else
                ENV_DELETE(m_pMaterial);
        }
    }

    //-----------------------------------------------------------------------------------
    void CSprite::GetAbsoluteRect(TextureRect& spriteRect)
    {
        if (!m_pMaterial)
            return;

        // Work around!
        //if(m_pMaterial->GetNumFrames() > 1)
        if (m_bUpdateRect)
            m_pMaterial->GetTextureCoordinates(m_logicalRect);

        spriteRect.bottom = static_cast<float>(m_logicalRect.bottom * m_pMaterial->GetHeight());
        spriteRect.left = static_cast<float>(m_logicalRect.left * m_pMaterial->GetWidth());
        spriteRect.right = static_cast<float>(m_logicalRect.right * m_pMaterial->GetWidth());
        spriteRect.top = static_cast<float>(m_logicalRect.top * m_pMaterial->GetHeight());
    }

    //-----------------------------------------------------------------------------------
    void CSprite::GetLogicalRect(float& left, float& right, float& top, float& bottom)
    {
        if (!m_pMaterial)
            return;

        m_pMaterial->GetTextureCoordinates(left, top, right, bottom);
    }

    //-----------------------------------------------------------------------------------
    void CSprite::GetLogicalRect(TextureRect& logicalRect)
    {
        if (m_bUpdateRect && m_pMaterial)
            m_pMaterial->GetTextureCoordinates(m_logicalRect);

        logicalRect = m_logicalRect;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::SetMaterialFilename(const std::string& strTexture, bool bCreateNewMtrlInstance)
    {
        Reset();

        if (!bCreateNewMtrlInstance)
        {
            m_pMaterial = gMaterialMgr->AddGetPtr(strTexture);
        }
        else
        {
            m_pMaterial = new CManagedMaterial();
            m_pMaterial->InitResource(strTexture, strTexture);
        }

        m_bMaterialInstance = bCreateNewMtrlInstance;

        m_strMaterial = strTexture;

        if (!m_pMaterial)
            return;

        if (m_pMaterial->GetTexture())
        {
            m_pMaterial->GetTexture()->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        }

        m_pMaterial->GetTextureCoordinates(m_logicalRect);

        TextureRect absoluteRect;
        GetAbsoluteRect(absoluteRect);
        m_vCenter.x = (absoluteRect.right - absoluteRect.left) * 0.5f;
        m_vCenter.y = (absoluteRect.bottom - absoluteRect.top) * 0.5f;
        m_vCenter.z = 0.0f;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CSprite::GetCenter()
    {
        return m_vCenter;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::Reset()
    {
        if (m_pMaterial)
        {
            if (!m_bMaterialInstance)
            {
                gMaterialMgr->Release(m_pMaterial->GetName());
                m_pMaterial = NULL;
            }
            else
            {
                ENV_DELETE(m_pMaterial);
            }
        }

        m_bMaterialInstance = false;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::InitializeAtLoad()
    {
        SetMaterialFilename(m_strMaterial);
    }

    //-----------------------------------------------------------------------------------
    void CSprite::SetAlphaBlending(bool bAlphaBlending)
    {
        m_bAlphaBlending = bAlphaBlending;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::SetBillboard(bool bBillboard)
    {
        m_bBillboard = bBillboard;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::SetScreenSpace(bool bSet)
    {
        m_bScreenSpace = bSet;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::SetAlpha(float alpha)
    {
        m_vColor.w = alpha;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::SetColor(const Vec3& clr)
    {
        m_vColor.x = clr.x;
        m_vColor.y = clr.y;
        m_vColor.z = clr.z;
    }

    //-----------------------------------------------------------------------------------
    void CSprite::SetColor(const Vec4& clr)
    {
        m_vColor = clr;
    }

    //-----------------------------------------------------------------------------------
    SpriteRenderObject& CSprite::GetRenderObject()
    {
#pragma message("--> IMPROVE!")
        TextureRect logicalRect;
        GetLogicalRect(logicalRect);

        m_spriteRenderObject.m_bAlphaBlending = GetAlphaBlending();
//         m_spriteRenderObject.m_bApplyFade = GetApplyFade();
//         m_spriteRenderObject.m_bApplyPostProcessing = GetApplyPostProcessing();
        m_spriteRenderObject.m_bBillboard = GetBillboard();
        m_spriteRenderObject.m_pipeline = m_pipeline;
        m_spriteRenderObject.m_bScreenSpace = GetScreenSpace();
        m_spriteRenderObject.m_bDepthTest = GetDepthTest();
        //m_spriteRenderObject.m_center = GetCenter();
        m_spriteRenderObject.m_color = GetColor();
        m_spriteRenderObject.m_pTexture = m_pMaterial->GetTexture();

        m_spriteRenderObject.m_texRect = Vec4(logicalRect.left,
                                              logicalRect.top,
                                              logicalRect.right - logicalRect.left,
                                              logicalRect.bottom - logicalRect.top);

        Vec3 spriteSize(m_spriteRenderObject.m_pTexture->GetWidth() * 0.5f * m_spriteRenderObject.m_texRect.z,
                        m_spriteRenderObject.m_pTexture->GetHeight() * -0.5f * m_spriteRenderObject.m_texRect.w,
                        1.0f);

        Vec2 spriteOffset(m_vCenter.x - spriteSize.x,
                          m_vCenter.y + spriteSize.y);

        // Scales sprite from size [-1, 1] to [-textureWidth * 0.5f, +textureWidth * 0.5f] 
        Matrix4x4 mtxScaling = glm::transpose(glm::scale(Matrix4x4(1.0f), spriteSize));

        // Translates sprite to account for the center parameter
        Matrix4x4 mtxTranslationCenter = glm::transpose(glm::translate(Matrix4x4(1.0f), Vec3(-spriteOffset.x, spriteOffset.y, 0)));

        m_spriteRenderObject.m_mtxRST = mtxScaling * mtxTranslationCenter *m_pNode->GetDerivedRST();

//         if (!m_bBillboard)
//             m_spriteRenderObject.m_mtxRST = m_spriteRenderObject.m_mtxRST * m_pNode->GetDerivedRST();
//         else
//             m_spriteRenderObject.m_mtxRST = m_spriteRenderObject.m_mtxRST * m_pNode->GetDerivedST();

        return m_spriteRenderObject;
    }

} // env