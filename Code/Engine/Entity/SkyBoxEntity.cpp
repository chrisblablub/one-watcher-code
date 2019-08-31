#include <Engine/Entity/SkyBoxEntity.h>
#include <Engine/Core/Camera.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CSkyBoxEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SKYBOX;

    CSkyBoxEntity::CSkyBoxEntity()
        : m_pTexture(NULL)
    {
        SetMetaTypeID(CSkyBoxEntity::ENTITY_META_TYPEID);
    }

    CSkyBoxEntity::~CSkyBoxEntity()
    {
        if (m_pTexture)
            gTextureMgr->Release(m_pTexture->GetName());
    }

    void CSkyBoxEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        CSpriteEntity::SetRenderGame(false);
        CSpriteEntity::SetRenderEditor(false);

        m_pNode->SetScale(Vec3(10.0f, 10.0f, 10.0f));

        m_pTexture = gTextureMgr->AddGetPtr("Skies/SkyBox01.dds");

        if (m_pTexture)
        {
            CTextureSampler textureSampler(TEXTUREADDRESS_TYPE_WRAP, TEXTUREADDRESS_TYPE_WRAP,
                                           TEXTUREFILTER_TYPE_ANISOTROPIC, TEXTUREFILTER_TYPE_LINEAR, TEXTUREFILTER_TYPE_LINEAR);

            textureSampler.SetMaxAnisotropy(4);

            m_pTexture->SetTextureSampler(textureSampler);
        }
    }


    void CSkyBoxEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        m_pTexture = gTextureMgr->AddGetPtr("Skies/SkyBox01.dds");

        if (m_pTexture)
        {
            CTextureSampler textureSampler(TEXTUREADDRESS_TYPE_WRAP, TEXTUREADDRESS_TYPE_WRAP,
                                           TEXTUREFILTER_TYPE_ANISOTROPIC, TEXTUREFILTER_TYPE_LINEAR, TEXTUREFILTER_TYPE_LINEAR);

            textureSampler.SetMaxAnisotropy(4);

            m_pTexture->SetTextureSampler(textureSampler);
        }
    }

    void CSkyBoxEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);

        m_pNode->SetPosition(gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition());
    }

    void CSkyBoxEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        m_skyBoxRenderObject.m_mtxTransform = m_pNode->GetDerivedRST();
        m_skyBoxRenderObject.m_pSkyTexture = m_pTexture;

        renderQueue.m_pSkyBoxRenderObject = &m_skyBoxRenderObject;
    }

    void CSkyBoxEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }







} // env
