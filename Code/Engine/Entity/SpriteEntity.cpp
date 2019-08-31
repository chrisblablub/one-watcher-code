#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/PolygonMousePicker.h>
#include <Engine/Scene/SceneMgr.h>
#include <Graphic/RenderSystems/RenderSystem.h>

namespace env
{
    ENTITY_TYPEID CSpriteEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SPRITE;

    //-----------------------------------------------------------------------------------
    CSpriteEntity::CSpriteEntity()
        : m_bBillboard(false)
        , m_bAlphaBlending(false)
        , m_bScreenSpace(false)
        , m_vCenter(0.0, 0.0, 0.0)
        , m_screenPosition(0.0f, 0.0f)
        , m_bRenderEditor(true)
        , m_bRenderGamemode(true)
        , m_bRender(true)
        , m_bInitiallyVisible(true)
        , m_bScalingSet(false)
        , m_currentAlphaTime(-1.0f)
        , m_alphaDuration(1.0f)
        , m_alphaBegin(1.0f)
        , m_alphaEnd(1.0f)
        , m_fadeInTime(0.6f)
        , m_fadeOutTime(0.6f)
        , m_initialColor(1.0f, 1.0f, 1.0f, 1.0f)
        , m_scalingBegin(1.0f)
        , m_scalingEnd(1.0f)
        , m_scalingInitial(1.0f)
        , m_scalingDuration(0.0f)
        , m_scalingTime(-1.0f)
        , m_version(0)
        , m_bForceUpdate(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Sprite] Show", SPRITE_EVENT_SHOW);
        RegisterEventParameter(SPRITE_EVENT_SHOW, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100000.0f);
        RegisterEventParameter(SPRITE_EVENT_SHOW, "Begin", CBaseParam::PARAMETER_TYPE_FLOAT, 0.0f, 0.0f, 1.0f);
        RegisterEventParameter(SPRITE_EVENT_SHOW, "End", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 1.0f);

        RegisterEvent("[Sprite] Hide", SPRITE_EVENT_HIDE);

        RegisterEvent("[Sprite] SetScaling", SPRITE_EVENT_SET_SCALING);
        RegisterEventParameter(SPRITE_EVENT_SET_SCALING, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100000.0f);
        RegisterEventParameter(SPRITE_EVENT_SET_SCALING, "Begin", CBaseParam::PARAMETER_TYPE_VECTOR3, Vec3(1.0f), Vec3(0.0f), Vec3(10.0f));
        RegisterEventParameter(SPRITE_EVENT_SET_SCALING, "End", CBaseParam::PARAMETER_TYPE_VECTOR3, Vec3(1.0f), Vec3(0.0f), Vec3(10.0f));

        m_spriteRect.bottom = 1;
        m_spriteRect.left = 0;
        m_spriteRect.right = 1;
        m_spriteRect.top = 0;

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CSpriteEntity::~CSpriteEntity()
    {
        gEngine->GetSceneMgr()->RemoveNode(m_pNode->GetName());
    }

    //-----------------------------------------------------------------------------------
    bool CSpriteEntity::HasCollision(CEntity& entity)
    {
        if (entity.GetMetaTypeId() == CCharacter3DEntity::ENTITY_META_TYPEID)
        {
            if (CGeometryCollidable::HasCollision(entity))
            {
                return true;
            }
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CSpriteEntity::HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode)
    {
        if (!GetPickable())
            return false;

        if (CEntity::HasCollisionWithRay(vRayOrigin, vRayDirection, distance, ppNode))
            return true;

        Vec3 vCenter = m_sprite.GetCenter();

        if (m_sprite.GetMaterial() == nullptr)
        {
            return false;
        }

        CPolygonMousePicker mousePickerFront;
        mousePickerFront.AddPoint(Vec3(-vCenter.x, -vCenter.y, 0.0));
        mousePickerFront.AddPoint(Vec3(-vCenter.x, vCenter.y, 0.0));
        mousePickerFront.AddPoint(Vec3(vCenter.x, vCenter.y, 0.0));
        mousePickerFront.AddPoint(Vec3(vCenter.x, -vCenter.y, 0.0));

        *ppNode = m_pNode;

        if (mousePickerFront.CheckPicked(m_pNode->GetDerivedRST(), gEngine->GetWorldMgr()->GetActiveCamera(), &distance))
            return true;

        CPolygonMousePicker mousePickerBack;
        mousePickerBack.AddPoint(Vec3(-vCenter.x, -vCenter.y, 0.0));
        mousePickerBack.AddPoint(Vec3(vCenter.x, -vCenter.y, 0.0));
        mousePickerBack.AddPoint(Vec3(vCenter.x, vCenter.y, 0.0));
        mousePickerBack.AddPoint(Vec3(-vCenter.x, vCenter.y, 0.0));

        *ppNode = m_pNode;

        if (mousePickerBack.CheckPicked(m_pNode->GetDerivedRST(), gEngine->GetWorldMgr()->GetActiveCamera(), &distance))
            return true;

        return false;
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::InitializeAtCreation()
    {
        CEntity::InitializeAtCreation();

        m_pNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pNode->Serialize();
        gEngine->GetSceneMgr()->GetRootNode()->AttachChildNode(m_pNode);

        Vec3 vOrigin, vDirection;
        CCamera* pCamera = gEngine->GetWorldMgr()->GetActiveCamera();
        vOrigin = pCamera->GetAttachedNode()->GetDerivedPosition();
        vDirection = pCamera->GetAttachedNode()->GetDerivedDirection();

        float dist = 0.0f;
        glm::intersectRayPlane(vOrigin, vDirection, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), dist);

        Vec3 intersection = vOrigin + glm::normalize(vDirection) * dist;

        m_pNode->SetPosition(intersection);

        SetMaterialFilename("Sprites/dummy.png");
        m_sprite.SetAlphaBlending(true);
        m_sprite.AttachNode(m_pNode);

        InitBoundingBox();
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::InitializeAtLoad()
    {
        CEntity::InitializeAtLoad();

        m_sprite.AttachNode(m_pNode);

        m_sprite.InitializeAtLoad();

        SetScreenSpace(m_bScreenSpace);

        InitBoundingBox();
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::InitBoundingBox()
    {
        m_boundingBox.Reserve(8);
        m_boundingBox.SetPrimitiveType(PRIMITIVE_TYPE_LINELIST);

        m_boundingBox.Add(PolygonVertex(Vec3(-1.0f, -1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));
        m_boundingBox.Add(PolygonVertex(Vec3(-1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));

        m_boundingBox.Add(PolygonVertex(Vec3(-1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));
        m_boundingBox.Add(PolygonVertex(Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));

        m_boundingBox.Add(PolygonVertex(Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));
        m_boundingBox.Add(PolygonVertex(Vec3(1.0f, -1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));

        m_boundingBox.Add(PolygonVertex(Vec3(1.0f, -1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));
        m_boundingBox.Add(PolygonVertex(Vec3(-1.0f, -1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::OnEntityCloned(CEntity& entity)
    {
        CEntity::OnEntityCloned(entity);

        CSpriteEntity& other = static_cast<CSpriteEntity&>(entity);

        SetMaterialFilename(other.GetSprite().GetMaterial()->GetName());

        SetRenderEditor(other.GetRenderEditor());
        SetRenderGame(other.GetRenderGame());


        m_sprite.SetBillboard(other.GetSprite().GetBillboard());
        m_sprite.SetColor(other.GetSprite().GetColor());
        m_sprite.SetScreenSpace(other.GetScreenSpace());
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::SetScreenSpace(bool bScreenSpace)
    {
        m_bScreenSpace = bScreenSpace;

        m_sprite.SetScreenSpace(bScreenSpace);

        if (bScreenSpace)
        {
            m_sprite.AttachNode(&m_nodeScreenSpace);
        }
        else
        {
            m_sprite.AttachNode(m_pNode);
        }

    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case SPRITE_EVENT_SHOW:

            if (eventParameter.IsParameterSet("Begin"))
                eventParameter.GetParameter("Begin", m_alphaBegin);

            if (eventParameter.IsParameterSet("End"))
                eventParameter.GetParameter("End", m_alphaEnd);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_alphaDuration);

            m_currentAlphaTime = m_alphaDuration;

            break;

        case SPRITE_EVENT_SET_SCALING:

            if (eventParameter.IsParameterSet("Begin"))
                eventParameter.GetParameter("Begin", m_scalingBegin);

            if (eventParameter.IsParameterSet("End"))
                eventParameter.GetParameter("End", m_scalingEnd);

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_scalingDuration);

            m_scalingTime = m_scalingDuration;

            m_scalingInitial = m_sprite.GetNode()->GetScale();

            m_bScalingSet = true;

            break;

        case SPRITE_EVENT_HIDE:
            break;
        default:
            CEntity::OnInput(eventId, eventParameter);
        }
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::Update(float dt)
    {
        if (m_bForceUpdate == false && m_bRender == false)
        {
            return;
        }

        CEntity::Update(dt);

        ApplyTransform(m_pNode->GetDerivedRST());
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        ENV_SCOPED_ACCUM_CPU_PROFILING(Entity, CSpriteEntity::AddToRenderQueue);

        if (m_bRender == false)
        {
            return;
        }

        CEntity::AddToRenderQueue(dt, queue);

        const RenderQualityParams& videoParams = gRenderSystem->GetRenderQualityParams();

        m_nodeScreenSpace.SetPosition(Vec3(m_screenPosition.x, m_screenPosition.y, 0.0f));
        m_nodeScreenSpace.SetScale(m_pNode->GetDerivedScale() * 2.0f);

        if (GetGameMode() || gEngine->GetEntityMgr()->GetActiveEntity() == this)
        {
            m_sprite.SetScreenSpace(m_bScreenSpace);

            if (m_bScreenSpace)
            {
//                 Vec3 scaling = Vec3(2.0f / (videoParams.screenWidth * 0.5f),
//                                     2.0f / (videoParams.screenHeight * 0.5f),
//                                     1.0f);

                m_sprite.AttachNode(&m_nodeScreenSpace);
            }
            else
            {
                m_sprite.AttachNode(m_pNode);
            }
        }
        else
        {
            m_sprite.AttachNode(m_pNode);
            m_sprite.SetScreenSpace(false);
        }

        if (m_pNode)
        {
            float alpha = 1.0f;

            if (m_currentAlphaTime >= 0.0f)
            {
                m_currentAlphaTime -= dt;

                m_sprite.SetAlpha(Lerp(m_alphaEnd, m_alphaBegin, Clamp(m_currentAlphaTime / m_alphaDuration, 0.0f, 1.0f)));
            }


            if (m_scalingTime >= 0.0f)
            {
                float a = 1.0f;
                
                if (m_scalingDuration > 0.0f)
                    a = m_scalingTime / m_scalingDuration;

                Vec3 scaling = Lerp(m_scalingEnd, m_scalingBegin, a);

                scaling *= m_sprite.GetNode()->GetScale();

                m_sprite.GetNode()->SetScale(scaling);

                m_scalingTime -= dt;
            }
//             if (m_bFadingIn)
//             {
//                 if (m_currentFadeInTime >= 0.0f)
//                 {
//                     alpha = 1.0f - m_currentFadeInTime / m_fadeInTime;
// 
//                     m_currentFadeInTime -= dt;
// 
//                     m_sprite.SetAlpha(alpha);
//                 }
//                 else
//                 {
//                     m_bFadingIn = false;
// 
//                     m_sprite.SetAlpha(1.0f);
//                 }
//             }
//             else if (m_bFadingOut)
//             {
//                 if (m_currentFadeOutTime >= 0.0f)
//                 {
//                     alpha = m_currentFadeOutTime / m_fadeOutTime;
// 
//                     m_currentFadeOutTime -= dt;
// 
//                     m_sprite.SetAlpha(alpha);
//                 }
//                 else
//                 {
//                     m_bFadingOut = false;
// 
//                     m_sprite.SetAlpha(0.0f);
//                 }
//             }

            if (m_bRender && m_sprite.GetMaterial())
            {
                if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
                {
                    SpriteRenderObject spriteRenderObj = m_sprite.GetRenderObject();

                    m_boundingBox.Transform(spriteRenderObj.m_mtxRST);

                    queue.m_spriteRenderObjects.push_back(spriteRenderObj);

                    if (gEngine->GetEntityMgr()->GetSelectionOutline())
                        queue.m_vector3Caches.push_back(&m_boundingBox);
                }
                else
                {
                    queue.m_spriteRenderObjects.push_back(m_sprite.GetRenderObject());
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::OnGameModeChanged(bool bGameMode)
    {
        CEntity::OnGameModeChanged(bGameMode);

        m_sprite.SetAlpha(1.0f);

        m_currentAlphaTime = -1.0f;

        if (bGameMode)
        {
            m_bRender = m_bRenderGamemode;
            SetText(std::string());
        }
        else
        {
            m_bRender = m_bRenderEditor;
            SetText(GetName());
        }

        m_initialColor = m_sprite.GetColor();

        if (!m_bInitiallyVisible)
            m_sprite.SetAlpha(0.0f);

        if (m_bScalingSet)
        {
            m_sprite.GetNode()->SetScale(m_scalingInitial);
            m_bScalingSet = false;
        }

        m_scalingTime = -1.0f;
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::SetTextPosition(const Vec2& vPosition)
    {
        m_sprite.SetTextPosition(vPosition);
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::SetText(const std::string& strText)
    {
        m_strText = strText;
        m_sprite.SetText(strText);
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::SetMaterialFilename(const std::string& strMaterial)
    {
        m_strTextureFilename = strMaterial;
        m_sprite.SetMaterialFilename(strMaterial);
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::SetRender(bool bRender)
    {
        m_bRenderGamemode = bRender;
        m_bRenderEditor = bRender;
        m_bRender = bRender;
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::SetRenderGame(bool bGame)
    {
        m_bRenderGamemode = bGame;
        if (GetGameMode())
            m_bRender = m_bRenderGamemode;
    }

    //-----------------------------------------------------------------------------------
    void CSpriteEntity::SetRenderEditor(bool bEditor)
    {
        m_bRenderEditor = bEditor;
        if (!GetGameMode())
            m_bRender = m_bRenderEditor;
    }












    ENTITY_TYPEID CTriggerEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_TRIGGER;

    //-----------------------------------------------------------------------------------
    CTriggerEntity::CTriggerEntity()
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CTriggerEntity::~CTriggerEntity()
    {

    }

    //-----------------------------------------------------------------------------------
    void CTriggerEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        SetMaterialFilename("Sprites/trigger.png");

        SetFireOnce(true);

        SetRenderGame(false);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CTriggerEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();

        SetFireOnce(true);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CTriggerEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    void CTriggerEntity::OnTriggerEntered(CEntity& e)
    {
        CSpriteEntity::OnTriggerEntered(e);

        if (e.GetMetaTypeId() == ENTITY_TYPEID_CHARACTER)
        {
            //SetRenderGame(false);

            //m_bSwitchTriggered = true;
        }
    }

    //-----------------------------------------------------------------------------------
    void CTriggerEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    //-----------------------------------------------------------------------------------
    void CTriggerEntity::SetMaterialFilename(const std::string& strMaterial)
    {
        CSpriteEntity::SetMaterialFilename(strMaterial);

        Vec3 vCenter = m_sprite.GetCenter();

        RemovePolygons();

        CPolygon poly(4);
        poly.AddPoint(Vec3(-vCenter.x, -vCenter.y, 0.0));
        poly.AddPoint(Vec3(-vCenter.x, vCenter.y, 0.0));
        poly.AddPoint(Vec3(vCenter.x, vCenter.y, 0.0));
        poly.AddPoint(Vec3(vCenter.x, -vCenter.y, 0.0));

        AddPolygon(poly);
    }



    env::ENTITY_TYPEID CCounterV2Entity::ENTITY_META_TYPEID = ENTITY_TYPEID_COUNTERV2;

    //-----------------------------------------------------------------------------------
    CCounterV2Entity::CCounterV2Entity()
        : m_counter(2)
        , m_currentCounter(0)
    {
        SetMetaTypeID(CCounterV2Entity::ENTITY_META_TYPEID);

        RegisterEvent("[Counter] Increment", COUNTER_EVENT_INCREMENT);
        RegisterEvent("[Counter] Decrement", COUNTER_EVENT_DESCREMENT);

    }

    //-----------------------------------------------------------------------------------
    CCounterV2Entity::~CCounterV2Entity()
    {

    }

    //-----------------------------------------------------------------------------------
    void CCounterV2Entity::InitializeAtCreation()
    {
        CTriggerEntity::InitializeAtCreation();

        SetGhost(true);
    }

    //-----------------------------------------------------------------------------------
    void CCounterV2Entity::InitializeAtLoad()
    {
        CTriggerEntity::InitializeAtLoad();

        SetGhost(true);
    }

    //-----------------------------------------------------------------------------------
    void CCounterV2Entity::OnGameModeChanged(bool bGameMode)
    {
        CTriggerEntity::OnGameModeChanged(bGameMode);

        m_currentCounter = 0;
    }

    //-----------------------------------------------------------------------------------
    void CCounterV2Entity::Update(float dt)
    {
        CTriggerEntity::Update(dt);

        if (m_counter <= m_currentCounter)
        {
            OnFired(0);

            m_currentCounter = 0;
        }
    }

    //-----------------------------------------------------------------------------------
    void CCounterV2Entity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case COUNTER_EVENT_INCREMENT:
            m_currentCounter++;
            break;

        case COUNTER_EVENT_DESCREMENT:
            m_currentCounter--;
            break;

        default:
            CTriggerEntity::OnInput(eventId, eventParameter);
        }
    }

} // env