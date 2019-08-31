#include <Engine/Entity/PlatformEntity.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/EntityMgr.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CPlatformBaseEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PLATFORMBASE;

    //-----------------------------------------------------------------------------------
    CPlatformBaseEntity::CPlatformBaseEntity()
        : m_halfSize(0.0f, 0.0f)
        , m_prevPosition(0.0f, 0.0f)
        , m_bDisabled(false)
        , m_pRackWheel(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CPlatformBaseEntity::~CPlatformBaseEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pRackWheel);
    }

    //-----------------------------------------------------------------------------------
    void CPlatformBaseEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CPlatformBaseEntity::Update);

        CMeshEntity::Update(dt);

        // Revert the parents orientation; rotated platforms are not supported
        if (m_pNode->GetParentNode())
        {
            const CQuaternion& quat = m_pNode->GetParentNode()->GetDerivedOrientation();
            m_pNode->SetOrientation(CQuaternion(-quat.GetYaw(), -quat.GetPitch(), -quat.GetRoll()));
        }

        Vec2 currentPosition = Vec2(m_pNode->GetDerivedPosition());
        Vec2 move = currentPosition - m_prevPosition;

//         m_pRackWheel->SetRotationSpeedZ(0.0f);
// 
//         if (abs(move.y) > abs(move.x))
//         {
//             // Movement top/down
//             if (move.y > 0.0f)
//                 m_pRackWheel->SetRotationSpeedZ(1.0f);
//             else if (move.y < 0.0f)
//                 m_pRackWheel->SetRotationSpeedZ(-1.0f);
//         }
//         else
//         {
//             if (move.x > 0.0f)
//                 m_pRackWheel->SetRotationSpeedZ(1.0f);
//             else if (move.x < 0.0f)
//                 m_pRackWheel->SetRotationSpeedZ(-1.0f);
//         }

        m_prevPosition = currentPosition;


        ApplyTransform(m_pNode->GetDerivedRST());
    }

    void CPlatformBaseEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CMeshEntity::AddToRenderQueue(dt, queue);

        if (m_pNode->GetParentNode())
        {
            const CQuaternion& quat = m_pNode->GetParentNode()->GetDerivedOrientation();
            m_pNode->SetOrientation(CQuaternion(-quat.GetYaw(), -quat.GetPitch(), -quat.GetRoll()));
        }
    }

    //-----------------------------------------------------------------------------------
    void CPlatformBaseEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("PlatformBot.x");
    }

    //-----------------------------------------------------------------------------------
    void CPlatformBaseEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        if (m_pRackWheel != nullptr)
        {
            //gEngine->GetEntityMgr()->RemoveEntity(m_pRackWheel);
            m_pRackWheel = nullptr;
        }

        m_bDisabled = false;
    }

    void CPlatformBaseEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        
        m_pSound->LoadSound("Entities/Platform.wav");
        m_pSound->SetVolume(0.4f);
        m_pSound->SetPeakVolumeDistance(200.0f);
        m_pSound->SetAudibleDistance(300);
    }

    //-----------------------------------------------------------------------------------
    void CPlatformBaseEntity::InitPlatform(const Vec2& halfSize, const Vec3& rackWheelScale, const Vec3& rackWheelPos)
    {
        m_halfSize = halfSize;

        //m_pMeshInstanceNode->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));

        RemovePolygons();
        CPolygon poly;
        poly.SetNumPoints(4);
        poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
        poly.AddPoint(Vec3(-m_halfSize.x,  m_halfSize.y, 0.0f));
        poly.AddPoint(Vec3( m_halfSize.x,  m_halfSize.y, 0.0f));
        poly.AddPoint(Vec3( m_halfSize.x, -m_halfSize.y, 0.0f));
        AddPolygon(poly);

        if (m_pRackWheel != nullptr)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pRackWheel);
        }
    }

    //-----------------------------------------------------------------------------------
    void CPlatformBaseEntity::SetMeshFilename(const std::string& strName)
    {
        CMeshEntity::SetMeshFilename(strName);

        if (strName == "PlatformBot.x")
        {
            InitPlatform(Vec2(93.0f, 79.0f), Vec3(0.67f), Vec3(0.0f, 117.0f, 0.0f));
        }
        else if (strName == "PlatformTop.x")
        {
            InitPlatform(Vec2(93.0f, 79.0f), Vec3(0.67f), Vec3(0.0f, -117.0f, 0.0f));
        }
        else if (strName == "PlatformTopBot.x")
        {
            InitPlatform(Vec2(93.0f, 79.0f * 2.0f), Vec3(0.67f), Vec3(0.0f, -117.0f, 0.0f));
        }
        else if (strName == "PlatformSmall.x")
        {
            InitPlatform(Vec2(93.0f, 29.91412 * 1.0f), Vec3(0.67f), Vec3(0.0f, -117.0f, 0.0f));
        }
        else
        {
            ENV_LOG_WARNING("Platform with name " + strName + " is not supported.");

            gEngine->GetEntityMgr()->RemoveEntity(m_pRackWheel);
            
            m_pRackWheel = nullptr;
        }
    }

    //-----------------------------------------------------------------------------------
    void CPlatformBaseEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);

        CPlatformBaseEntity& other = static_cast<CPlatformBaseEntity&>(entity);

        if (m_pRackWheel)
            m_pRackWheel->OnEntityCloned(*other.m_pRackWheel);

        m_halfSize = other.m_halfSize;
        m_prevPosition = other.m_prevPosition;
    }











    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CPlatformEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PLATFORM;

    //-----------------------------------------------------------------------------------
    CPlatformEntity::CPlatformEntity()
        : m_halfSize(0.0f, 0.0f),
        m_bDisabled(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CPlatformEntity::~CPlatformEntity()
    {
        // 		////if(m_pSawblade)
        // 		//{
        // 			gEngine->GetEntityMgr()->RemoveEntity(m_pSawblade->GetName());
        // 			m_pSawblade = NULL;
        // 		}

        for (int i = 0; i < m_attachedEntities.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_attachedEntities[i]->GetName());
        }

        m_attachedEntities.clear();
    }

    //-----------------------------------------------------------------------------------
    void CPlatformEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CPlatformEntity::Update);

        CMeshEntity::Update(dt);

        Vec2 currentPosition = Vec2(m_pNode->GetDerivedPosition());
        Vec2 move = currentPosition - m_prevPosition;

        for (int i = 0; i < m_attachedEntities.size(); ++i)
        {
            CMeshEntity* p = static_cast<CMeshEntity*>(m_attachedEntities[i]);
            p->SetRotationSpeedZ(0.0f);
        }

        if (abs(move.y) > abs(move.x))
        {
            // Movement top/down
            if (move.y > 0.01f)
            {
                for (int i = 0; i < m_attachedEntities.size(); ++i)
                {
                    CMeshEntity* p = static_cast<CMeshEntity*>(m_attachedEntities[i]);
                    p->SetRotationSpeedZ(1.0f);
                }
            }
            else if (move.y < -0.01f)
            {
                for (int i = 0; i < m_attachedEntities.size(); ++i)
                {
                    CMeshEntity* p = static_cast<CMeshEntity*>(m_attachedEntities[i]);
                    p->SetRotationSpeedZ(-1.0f);
                }
            }
        }
        else
        {
            if (move.x > 0.01f)
            {
                for (int i = 0; i < m_attachedEntities.size(); ++i)
                {
                    CMeshEntity* p = static_cast<CMeshEntity*>(m_attachedEntities[i]);
                    p->SetRotationSpeedZ(1.0f);
                }
            }
            else if (move.x < -0.01f)
            {
                for (int i = 0; i < m_attachedEntities.size(); ++i)
                {
                    CMeshEntity* p = static_cast<CMeshEntity*>(m_attachedEntities[i]);
                    p->SetRotationSpeedZ(-1.0f);
                }
            }
        }


        m_prevPosition = currentPosition;

        if (m_pNode->GetParentNode())
        {
            const CQuaternion& quat = m_pNode->GetParentNode()->GetDerivedOrientation();
            m_pNode->SetOrientation(CQuaternion(-quat.GetYaw(), -quat.GetPitch(), -quat.GetRoll()));
        }


        ApplyTransform(m_pNode->GetDerivedRST());
    }

    //-----------------------------------------------------------------------------------
    void CPlatformEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetType(PLATFORM_TYPE_RACKWHEEL_TOP);

        SetFireOncePerHit(false);
        SetFireOnce(false);
    }

    //-----------------------------------------------------------------------------------
    void CPlatformEntity::OnCreatePlatform()
    {
        if (m_type == PLATFORM_TYPE_RACKWHEEL_TOP)
        {
            m_halfSize.x = 93.0f;
            m_halfSize.y = 102.0f;

            //GetMeshInstanceNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));

            RemovePolygons();
            CPolygon poly;
            poly.SetNumPoints(4);
            poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(-m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, -m_halfSize.y, 0.0f));
            AddPolygon(poly);

            CMeshEntity::SetMeshFilename("platform01.x");

            CEntity* p = gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_RACKWHEEL);
            p->SetAutoDelete(false);
            p->SetPickable(false);
            p->GetNode()->SetPosition(Vec3(0.0f, m_halfSize.y - 20.0f, 0.0f));
            p->GetNode()->SetScale(Vec3(0.62f, 0.62f, 0.62f));
            m_pNode->AttachChildNode(p->GetNode());

            m_attachedEntities.push_back(p);
        }
        else if (m_type == PLATFORM_TYPE_RACKWHEEL_BOTTOM)
        {
            m_halfSize.x = 93.0f;
            m_halfSize.y = 102.0f;

            //GetMeshInstanceNode()->SetOrientation(CQuaternion(0.0f, 0.0f, PI));

            RemovePolygons();
            CPolygon poly;
            poly.SetNumPoints(4);
            poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(-m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, -m_halfSize.y, 0.0f));
            AddPolygon(poly);

            CMeshEntity::SetMeshFilename("platform01.x");

            CEntity* pSawbladeBottom = gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_RACKWHEEL);
            pSawbladeBottom->SetAutoDelete(false);
            pSawbladeBottom->SetPickable(false);
            pSawbladeBottom->GetNode()->SetPosition(Vec3(0.0f, -m_halfSize.y + 20.0f, 0.0f));
            pSawbladeBottom->GetNode()->SetScale(Vec3(0.62f, 0.62f, 0.62f));
            m_pNode->AttachChildNode(pSawbladeBottom->GetNode());

            m_attachedEntities.push_back(pSawbladeBottom);
        }
        else if (m_type == PLATFORM_TYPE_RACKWHEEL_TOPBOTTOM)
        {
            m_halfSize.x = 93.0f;
            m_halfSize.y = 204.0f;

            RemovePolygons();
            CPolygon poly;
            poly.SetNumPoints(4);
            poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(-m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, -m_halfSize.y, 0.0f));
            AddPolygon(poly);

            CMeshEntity::SetMeshFilename("platform02.x");

            CEntity* pSawbladeTop = gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_RACKWHEEL);
            pSawbladeTop->SetAutoDelete(false);
            pSawbladeTop->SetPickable(false);
            pSawbladeTop->GetNode()->SetPosition(Vec3(0.0f, m_halfSize.y - 20.0f, 0.0f));
            pSawbladeTop->GetNode()->SetScale(Vec3(0.62f, 0.62f, 0.62f));
            m_pNode->AttachChildNode(pSawbladeTop->GetNode());

            CEntity* pSawbladeBottom = gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_RACKWHEEL);
            pSawbladeBottom->SetAutoDelete(false);
            pSawbladeBottom->SetPickable(false);
            pSawbladeBottom->GetNode()->SetPosition(Vec3(0.0f, -m_halfSize.y + 20.0f, 0.0f));
            pSawbladeBottom->GetNode()->SetScale(Vec3(0.62f, 0.62f, 0.62f));
            m_pNode->AttachChildNode(pSawbladeBottom->GetNode());

            m_attachedEntities.push_back(pSawbladeTop);
            m_attachedEntities.push_back(pSawbladeBottom);
        }
        else if (m_type == PLATFORM_TYPE_GATE)
        {
            m_halfSize.x = 78.0f;
            m_halfSize.y = 16.0f;

            RemovePolygons();
            CPolygon poly;
            poly.SetNumPoints(4);
            poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(-m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, -m_halfSize.y, 0.0f));
            AddPolygon(poly);

            CMeshEntity::SetMeshFilename("platform03.x");
        }
        if (m_type == PLATFORM_TYPE_SMALLRACKWHEEL_TOP)
        {
            m_halfSize.x = 93.0f;
            m_halfSize.y = 36.0f;

           // GetMeshInstanceNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));

            RemovePolygons();
            CPolygon poly;
            poly.SetNumPoints(4);
            poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(-m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, -m_halfSize.y, 0.0f));
            AddPolygon(poly);

            CMeshEntity::SetMeshFilename("platform05.x");

            CEntity* p = gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_RACKWHEEL);
            p->SetAutoDelete(false);
            p->SetPickable(false);
            p->GetNode()->SetPosition(Vec3(0.0f, m_halfSize.y - 20.0f, 0.0f));
            p->GetNode()->SetScale(Vec3(0.62f, 0.62f, 0.62f));
            m_pNode->AttachChildNode(p->GetNode());

            m_attachedEntities.push_back(p);
        }
        else if (m_type == PLATFORM_TYPE_SMALLRACKWHEEL_BOTTOM)
        {
            m_halfSize.x = 93.0f;
            m_halfSize.y = 20.0f;

            //GetMeshInstanceNode()->SetOrientation(CQuaternion(0.0f, 0.0f, PI));

            RemovePolygons();
            CPolygon poly;
            poly.SetNumPoints(4);
            poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(-m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, m_halfSize.y, 0.0f));
            poly.AddPoint(Vec3(m_halfSize.x, -m_halfSize.y, 0.0f));
            AddPolygon(poly);

            CMeshEntity::SetMeshFilename("platform05.x");

            CEntity* pSawbladeBottom = gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_RACKWHEEL);
            pSawbladeBottom->SetAutoDelete(false);
            pSawbladeBottom->SetPickable(false);
            pSawbladeBottom->GetNode()->SetPosition(Vec3(0.0f, -m_halfSize.y - 40.0f, 0.0f));
            pSawbladeBottom->GetNode()->SetScale(Vec3(0.62f, 0.62f, 0.62f));
            m_pNode->AttachChildNode(pSawbladeBottom->GetNode());

            m_attachedEntities.push_back(pSawbladeBottom);
        }
    }

    //-----------------------------------------------------------------------------------
    void CPlatformEntity::SetType(PLATFORM_TYPE type)
    {
        m_type = type;


        for (int i = 0; i < m_attachedEntities.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_attachedEntities[i]->GetName());
        }

        m_attachedEntities.clear();

        OnCreatePlatform();
    }

    //-----------------------------------------------------------------------------------
    void CPlatformEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        SetType(m_type);

    }

    //-----------------------------------------------------------------------------------s
    void CPlatformEntity::OnGameModeChanged(bool bGameMode)
    {
        //CMeshEntity::OnModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    void CPlatformEntity::SetSize(float width, float height)
    {
        m_halfSize.x = width;
        m_halfSize.y = height;
    }

    const float CPlatformDrone::BLINK_TIME = 0.25f;

    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CPlatformDrone::ENTITY_META_TYPEID = ENTITY_TYPEID_PLATFORMDRONE;

    //-----------------------------------------------------------------------------------
    CPlatformDrone::CPlatformDrone()
        : m_pExplosion(nullptr)
        , m_pFan(nullptr)
        , m_currentBlinkTime(BLINK_TIME)
        , m_bBlinkAppear(false)
        , m_delay(-1.0f)
        , m_currentDelay(-1.0f)
        , m_currentSpawnTime(-1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CPlatformDrone::~CPlatformDrone()
    {
        if (m_pExplosion)
            gEngine->GetEntityMgr()->RemoveEntity(m_pExplosion->GetName());

        if (m_pFan)
            gEngine->GetEntityMgr()->RemoveEntity(m_pFan->GetName());
    }

    //-----------------------------------------------------------------------------------
    bool CPlatformDrone::HasCollision(CEntity& e)
    {
        if (e.GetMetaTypeId() == ENTITY_TYPEID_EXPLOSION && &e != m_pExplosion && CGeometryCollidable::HasCollision(e))
        {
            OnPlayerOnPlatform();

            return true;
        }
        return false;
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::SetEnabled()
    {
        CPlatformEntity::SetEnabled();

        SetGhost(false);

        m_currentSpawnTime = 0.2f;

        m_pNode->SetScale(Vec3(0, 0, 0));

        m_currentDelay = -1.0f;

        m_accumExplosionTime = 0.4f;
        m_bExploding = false;

        GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetRender(true); });

        m_pFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetRender(true); });
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::SetDisabled()
    {
        CPlatformEntity::SetDisabled();

        SetGhost(true);

        m_currentDelay = m_delay;
        m_currentSpawnTime = -1.0f;
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CPlatformDrone::Update);

        CPlatformEntity::Update(dt);

        m_spriteNode.SetPosition(m_pNode->GetDerivedPosition() + Vec3(0.0f, 0.0f, -5.0f));

        m_pExplosion->GetNode()->SetPosition(m_pNode->GetDerivedPosition());

        if (m_bDisabled && m_delay > 0.0f)
        {
            m_currentDelay -= dt;

            if (m_currentDelay < 0.0f)
            {
                SetEnabled();
            }
        }

        if (m_currentSpawnTime >= 0.0f)
        {
            m_currentSpawnTime -= dt;

            float alpha = 1.0f - m_currentSpawnTime / 0.2f;

            m_pNode->SetScale(Vec3(1.45f * alpha, 1.45f * alpha, 1.45f * alpha));
        }
        else
        {
            if (m_bExploding)
            {
                m_accumExplosionTime -= dt;

                if (m_accumExplosionTime >= 0.0f)
                {
                    m_sprite.SetAlpha(2.0f);

                    float alpha = 1.0f - m_accumExplosionTime / 0.25f;

                    m_sprite.GetNode()->SetScale(Lerp(Vec3(0.15f, 0.15f, 0.15f), Vec3(0.6f, 0.6f, 0.6f), alpha));
                }
                else
                {
                    m_sprite.GetNode()->SetScale(Vec3(0.0f, 0.0f, 0.0f));
                }


                if (m_accumExplosionTime < 0.0f)
                {
                    if (m_pExplosion && !m_pExplosion->GetExploding() && !GetDisabled())
                    {
                        m_pExplosion->SetEnabled();
                        SetDisabled();
                    }

                    if (m_accumExplosionTime < -0.15f)
                    {
                        if (m_accumExplosionTime > -0.25f)
                        {
                            float alpha = 1.0f - (abs(m_accumExplosionTime) - 0.15f) / 0.1f;

                            m_pNode->SetScale(Vec3(1.45f * alpha, 1.45f * alpha, 1.45f * alpha));
                        }
                        else
                        {
                            GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetRender(false); });
                            m_pFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetRender(false); });
                        }
                    }
                }
            }
            else
            {
                m_currentBlinkTime -= dt;
                if (m_currentBlinkTime < 0.0f)
                {
                    m_bBlinkAppear = !m_bBlinkAppear;
                    m_currentBlinkTime = BLINK_TIME;
                }
                else
                {
                    float alpha = m_currentBlinkTime / BLINK_TIME;
                    if (m_bBlinkAppear)
                        alpha = 1.0f - alpha;

                    m_sprite.SetAlpha(alpha * 2.0f);
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::InitializeAtCreation()
    {
        CPlatformEntity::InitializeAtCreation();

        m_pExplosion = static_cast<CExplosionEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_EXPLOSION));
        m_pExplosion->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -25.0f));
        m_pExplosion->SetAutoDelete(false);
        m_pExplosion->SetPickable(false);
        m_pExplosion->SetRenderEditor(false);

        m_pFan = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
        m_pFan->SetMeshFilename("FanHeavy01.x");
        m_pFan->SetAutoDelete(false);
        m_pFan->SetPickable(true);
        m_pFan->SetRotationSpeedZ(-1.0);
        m_pFan->GetNode()->SetScale(Vec3(0.270f, 0.270f, 0.270f));
        m_pFan->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 3.2f));
        m_pNode->AttachChildNode(m_pFan->GetNode());

        m_sprite.SetMaterialFilename("Sprites/circle.dds");
        m_sprite.AttachNode(&m_spriteNode);
        m_sprite.SetColor(Vec4(1.0f, 0.5f, 0.0f, 2.0f));
        m_spriteNode.SetScale(Vec3(0.15f, 0.15f, 0.15f));

        //m_pNode->AttachChildNode(m_pExplosion->GetNode());
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CPlatformEntity::AddToRenderQueue(dt, queue);

        //if (!m_bExploding)
            queue.m_spriteRenderObjects.push_back(m_sprite.GetRenderObject());
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::InitializeAtLoad()
    {
        CPlatformEntity::InitializeAtLoad();

        m_pFan->SetMeshFilename("FanHeavy01.x");
        m_pFan->SetPickable(true);
        m_pFan->GetNode()->SetScale(Vec3(0.3f, 0.3f, 0.3f));
        m_pFan->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 3.2f));

        m_sprite.SetMaterialFilename("Sprites/circle.dds");
        m_sprite.AttachNode(&m_spriteNode);
        m_sprite.SetColor(Vec4(1.0f, 0.5f, 0.0f, 2.0f));
        m_spriteNode.SetScale(Vec3(0.15f, 0.15f, 0.15f));

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->SetIsAutoplay(false);
        m_pSound->SetVolume(4);
        m_pSound->LoadSound("Entities/PlatformDrone.wav");
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::OnGameModeChanged(bool bGameMode)
    {
        CPlatformEntity::OnGameModeChanged(bGameMode);



        m_currentSpawnTime = -1.0f;
        m_currentBlinkTime = BLINK_TIME;

        m_sprite.GetNode()->SetScale(Vec3(0.15f, 0.15f, 0.15f));

        /*m_accumExplosionTime = 0.4f;
        m_bExploding = false;

        m_pMeshInstance->SetRenderInEditor(true);
        m_pMeshInstance->SetRenderInGame(true);

        m_pFan->GetMeshInstance()->SetRenderInEditor(true);
        m_pFan->GetMeshInstance()->SetRenderInGame(true);

        m_pNode->SetScale(VECTOR3(1.45f, 1.45f, 1.45f));

        m_currentDelay = -1.0f;*/

        SetEnabled();
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::OnCreatePlatform()
    {
        m_halfSize.x = 32.0f;
        m_halfSize.y = 32.0f;

        RemovePolygons();
        CPolygon poly;
        poly.SetNumPoints(4);
        poly.AddPoint(Vec3(-m_halfSize.x, -m_halfSize.y, 0.0f));
        poly.AddPoint(Vec3(-m_halfSize.x, m_halfSize.y, 0.0f));
        poly.AddPoint(Vec3(m_halfSize.x, m_halfSize.y, 0.0f));
        poly.AddPoint(Vec3(m_halfSize.x, -m_halfSize.y, 0.0f));
        AddPolygon(poly);

        CMeshEntity::SetMeshFilename("PlatformDrone.x");
    }

    //-----------------------------------------------------------------------------------
    void CPlatformDrone::OnPlayerOnPlatform()
    {
        if (!m_bExploding)
        {
            m_accumExplosionTime = 0.25f;
            m_bExploding = true;
            m_pSound->PlaySound();
        }
    }
} // env