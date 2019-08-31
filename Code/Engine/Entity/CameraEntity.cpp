#include <Engine/Entity/CameraEntity.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>
#include <Engine/Scene/SceneMgr.h>
#include <Graphic/RenderSystems/RenderSystem.h>

namespace env
{
    ENTITY_TYPEID CCameraEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_CAMERA;

    //-----------------------------------------------------------------------------------
    CCameraEntity::CCameraEntity()
        : m_pTarget(nullptr)
        , m_rotationSpeed(1.0f)
        , m_smoothEnableTime(0.0f)
        , m_currentSmoothEnableTime(-1.0f)
        , m_smoothTargetPos(0.0f, 0.0f, 0.0f)
        , m_smoothStartPos(0.0f, 0.0f, 0.0f)
        , m_initialPos(0.0f, 0.0f, 0.0f)
        , m_currentShakeTime(-1.0f)
        , m_shakeStrength(-500.0f, 500.0f)
        , m_shakePosition(0.0f, 0.0f, 0.0f)
        , m_bRollToTarget(true)
    {
        SetMetaTypeID(ENTITY_TYPEID_CAMERA);

        RegisterEvent("[Camera] Enable", CAMERA_EVENT_ENABLE);
        RegisterEvent("[Camera] Smooth Enable", CAMERA_EVENT_SMOOTHENABLE);
        RegisterEvent("[Camera] Disable", CAMERA_EVENT_DISABLE);
        RegisterEvent("[Camera] Shake light", CAMERA_EVENT_SHAKELIGHT);
        RegisterEvent("[Camera] Shake strong", CAMERA_EVENT_SHAKESTRONG);

        RegisterEvent("[Camera] Shake", CAMERA_EVENT_SHAKE);
        RegisterEventParameter(CAMERA_EVENT_SHAKE, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 0.0f, 0.0f, 100.0f);
        RegisterEventParameter(CAMERA_EVENT_SHAKE, "Intensity", CBaseParam::PARAMETER_TYPE_FLOAT);
    }

    //-----------------------------------------------------------------------------------
    CCameraEntity::~CCameraEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CCameraEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("camera.x");

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRenderInGame(false); });

        m_camera.AttachNode(m_pNode);


        //std::swap(m_pNode, m_pCamNode);

    }

    //-----------------------------------------------------------------------------------
    void CCameraEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        //m_pCamNode = gEngine->GetSceneMgr()->CreateSceneNode();
        //m_pNode->AttachChildNode(m_pCamNode);

        m_camera.AttachNode(m_pNode);
    }

    //-----------------------------------------------------------------------------------
    void CCameraEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CCameraEntity::Update);

        CMeshEntity::Update(dt);

        if(!GetGameMode() && m_bPicked)
        {
            m_initialPos = m_pNode->GetPosition();
            m_initialOrientation = m_pNode->GetOrientation();
        }
        else
        {
            if(m_currentSmoothEnableTime >= 0.0f)
            {
                if(m_currentSmoothEnableTime >= 0.0f)
                {
                    float alpha = m_currentSmoothEnableTime / m_smoothEnableTime;
                    
                    m_pNode->SetPosition(alpha * m_smoothStartPos + (1.0f - alpha) * m_smoothTargetPos);

                    if(m_pTarget)
                    {
                        m_smoothTargetOrientation.SetDirection(m_pTarget->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());

                        if (m_bRollToTarget)
                            m_smoothTargetOrientation.SetRoll(m_pTarget->GetNode()->GetDerivedOrientation().GetRoll());
                        else
                            m_smoothTargetOrientation.SetRoll(m_pNode->GetOrientation().GetRoll());

                    }
                    
                    CQuaternion quat(m_smoothTargetOrientation);
                    quat.Slerp(m_smoothStartOrientation, alpha);

                    m_pNode->SetOrientation(quat);
                    
                    m_currentSmoothEnableTime -= dt;
                }
            }
            else
            {
                if(m_pTarget)
                {
                    CQuaternion targetOrientation(m_pTarget->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());

                    if (m_bRollToTarget)
                        targetOrientation.SetRoll(m_pTarget->GetNode()->GetDerivedOrientation().GetRoll());
                    else
                        targetOrientation.SetRoll(m_pNode->GetOrientation().GetRoll());

                    m_pNode->SetOrientation(targetOrientation);
            
                }
            }

            OnShake(dt);
            
        }
    }

    //-----------------------------------------------------------------------------------
    void CCameraEntity::OnShake(float dt)
    {
        if(m_currentShakeTime >= 0.0f)
        {
            Vec3 pos = m_pNode->GetPosition();

            pos.x += GetRandomFloat(m_shakeStrength.x, m_shakeStrength.y) * dt;
            pos.y += GetRandomFloat(m_shakeStrength.x, m_shakeStrength.y) * dt;

            m_pNode->SetPosition(pos);

            m_currentShakeTime -= dt;

            if (m_currentShakeTime < 0.0f)
            {
                m_pNode->SetPosition(m_shakePosition);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CCameraEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_currentSmoothEnableTime = -1.0f;
        m_currentShakeTime = -1.0f;

        if(m_pNode)
        {
            m_pNode->SetPosition(m_initialPos);
            m_pNode->SetOrientation(m_initialOrientation);
        }
        
    }

    //-----------------------------------------------------------------------------------
    void CCameraEntity::OnResetDevice()
    {
        CMeshEntity::OnResetDevice();

        const RenderQualityParams& videoParams = gRenderSystem->GetRenderQualityParams();

        CFrustum& frustum = m_camera.GetFrustum();
        frustum.SetAspectRatio((float)videoParams.screenHeight / (float)(videoParams.screenWidth));
        frustum.SetNearClipping(videoParams.zNear);
        frustum.SetFarClipping(videoParams.zFar);
        frustum.SetWidth(static_cast<float>(videoParams.screenWidth));
        frustum.SetHeight(static_cast<float>(videoParams.screenHeight));
        m_camera.BuildProjectionMatrix();
    }

    void CCameraEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case CAMERA_EVENT_ENABLE:
            gEngine->GetWorldMgr()->SetActiveCamera(&m_camera);
            {
                if (m_pTarget)
                {
                    CQuaternion targetOrientation(m_pTarget->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());
                    CQuaternion currentOrienation(m_pNode->GetDerivedOrientation());
                    currentOrienation.Slerp(targetOrientation, 1.0f);
                    currentOrienation.SetRoll(m_pTarget->GetNode()->GetDerivedOrientation().GetRoll());
                    m_pNode->SetOrientation(currentOrienation);
                }

            }
            break;

        case CAMERA_EVENT_SMOOTHENABLE:
        {
            m_currentSmoothEnableTime = m_smoothEnableTime;

            m_smoothTargetPos = m_pNode->GetPosition();
            m_smoothTargetOrientation = m_pNode->GetOrientation();

            if (m_pTarget)
            {
                //CSceneNode n;
                //n.SetDirection(m_pTarget->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());

                m_smoothTargetOrientation = CQuaternion(m_pTarget->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());
                m_smoothTargetOrientation.SetRoll(m_pTarget->GetNode()->GetDerivedOrientation().GetRoll());
            }


            Matrix4x4 mtxRST = glm::inverse(m_pNode->GetParentNode()->GetDerivedRST());

            m_smoothStartPos = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition();
            m_smoothStartPos = Vec3(Vec4(m_smoothStartPos, 1.0f) * mtxRST);

            //m_smoothStartOrientation = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedOrientation();
            //m_smoothStartOrientation = m_smoothTargetOrientation;
            m_smoothStartOrientation = m_pNode->GetParentNode()->GetDerivedOrientation();

            float yaw = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedOrientation().GetYaw() - m_smoothStartOrientation.GetYaw();
            float pitch = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedOrientation().GetPitch() - m_smoothStartOrientation.GetPitch();
            float roll = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedOrientation().GetRoll() - m_smoothStartOrientation.GetRoll();

            m_smoothStartOrientation.SetYawPitchRoll(yaw, pitch, roll);

            gEngine->GetWorldMgr()->SetActiveCamera(&m_camera);

            m_pNode->SetPosition(m_smoothStartPos);
            m_pNode->SetOrientation(m_smoothStartOrientation);

        }
        break;
        case CAMERA_EVENT_DISABLE:
            gEngine->GetWorldMgr()->SetActiveCamera(gEngine->GetWorldMgr()->GetDefaultCamera());
            break;

        case CAMERA_EVENT_SHAKELIGHT:
            m_currentShakeTime = 0.2f;

            m_shakeStrength = Vec2(-500.0f, 500.0f);

            m_shakePosition = m_pNode->GetPosition();

            break;

        case CAMERA_EVENT_SHAKESTRONG:
            m_currentShakeTime = 0.2f;

            m_shakeStrength = Vec2(-6000.0f, 6000.0f);

            m_shakePosition = m_pNode->GetPosition();

            break;

        case CAMERA_EVENT_SHAKE:
        {
            float intensity = 0.0f;

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_currentShakeTime);

            if (eventParameter.IsParameterSet("Intensity"))
                eventParameter.GetParameter("Intensity", intensity);

            m_shakeStrength = Vec2(-3000.0f, 3000.0f) * intensity;

            m_shakePosition = m_pNode->GetPosition();
        } break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }


    ENTITY_TYPEID CPlayerCameraEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PLAYERCAMERA;
    
    //-----------------------------------------------------------------------------------
    CPlayerCameraEntity::CPlayerCameraEntity()
        : m_pCharacter(nullptr)
        , m_currentZoomInTime(-1.0f)
        , m_currentZoomOutTime(-1.0f)
        , m_zoomTime(0.2f)
        , m_zoomDistance(-800.0f)
        , m_bRespectLevelBoundaries(true)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Player Camera] Zoom In", PLAYERCAMERA_EVENT_ZOOMIN);
        RegisterEvent("[Player Camera] Zoom Out", PLAYERCAMERA_EVENT_ZOOMOUT);
    }

    //-----------------------------------------------------------------------------------
    CPlayerCameraEntity::~CPlayerCameraEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CPlayerCameraEntity::InitializeAtCreation()
    {
        CCameraEntity::InitializeAtCreation();
    }

    //-----------------------------------------------------------------------------------
    void CPlayerCameraEntity::InitializeAtLoad()
    {
        CCameraEntity::InitializeAtLoad();
    }

    //-----------------------------------------------------------------------------------
    void CPlayerCameraEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CPlayerCameraEntity::Update);

        CMeshEntity::Update(dt);

        if(!GetGameMode() && m_bPicked)
        {
            m_initialPos = m_pNode->GetPosition();
            m_initialOrientation = m_pNode->GetOrientation();
        }

        CTerrainMgr* pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

        /*if(gEngine->GetInput()->IsActionTriggered(ACTION_SECONDARY))
            m_camera.GetFrustum().SetFieldOfView(PI * 0.5f);
        else
            m_camera.GetFrustum().SetFieldOfView(PI * 0.25f);*/

        unsigned int cellsX = 0, cellsY = 0;
        float cellWidth = 0.0f, levelWidth = 0.0f, levelHeight = 0.0f;
        pLevel->GetLevelGrid().GetSize(cellsX, cellsY, cellWidth);
        levelWidth = (float)cellsX * cellWidth;
        levelHeight = (float)cellsY * cellWidth;

#ifndef ENV_FINAL
        if (gEngine->GetEditor() == true &&
            gEngine->GetEntityMgr()->IsValid(m_pCharacter) == false)
        {
            m_pCharacter = nullptr;

            gEngine->GetWorldMgr()->SetActiveCamera(gEngine->GetWorldMgr()->GetDefaultCamera());
        }
#endif // ENV_FINAL
        
        if(m_pCharacter)
        {
            Vec3 vCamTargetPos(0.0f, 0.0f, 0.0f);
            if(m_pTarget)
                vCamTargetPos = m_pTarget->GetNode()->GetDerivedPosition();
            else
                vCamTargetPos = m_pCharacter->GetNode()->GetDerivedPosition();

            Vec3 vCamPos = m_pNode->GetPosition();

            if(!m_pCharacter->GetKilled())
            {
                Vec3 vNewPos = vCamTargetPos;
                vNewPos.z = vCamPos.z;

                CFrustum& frustum = m_camera.GetFrustum();
                float hFovY = frustum.GetFieldOfView() * 0.5f;
                float yCam = tan(hFovY) * frustum.GetNearClipping();
                float xCam = yCam * frustum.GetAspectRatio();
                float hFovX = atan(xCam / frustum.GetNearClipping());
                float yWorld = abs(tan(hFovY) * vNewPos.z);
                float xWorld = abs(tan(hFovX) * vNewPos.z);

                LevelBorderOffsets offsets = gEngine->GetWorldMgr()->GetScenario()->GetLevelBorderOffsets();

                float levelTop = levelHeight - offsets.top * CELL_SIZE;

                levelWidth -= (offsets.right + offsets.left) * CELL_SIZE;
                levelHeight -= (offsets.top + offsets.bottom) * CELL_SIZE;//32.0f;

                

                if((2 * xWorld) > levelWidth)
                {
                    if((vNewPos.x - xWorld) < (offsets.left * CELL_SIZE))
                        vNewPos.x += abs((offsets.left * CELL_SIZE) - (vNewPos.x - xWorld));
                }
                else
                {
                    if((vNewPos.x - xWorld) < (offsets.left * CELL_SIZE))
                        vNewPos.x += abs((offsets.left * CELL_SIZE) - (vNewPos.x - xWorld));
                    else
                        vNewPos.x = vNewPos.x - std::max((vNewPos.x + xWorld) - (levelWidth + offsets.left * CELL_SIZE), 0.0f);
                }

                if((vNewPos.y - yWorld) < (offsets.bottom * CELL_SIZE))
                    vNewPos.y += abs((offsets.bottom * CELL_SIZE) - (vNewPos.y - yWorld));
                else if((vNewPos.y + yWorld) > levelTop && (2 * yWorld) <= levelHeight)
                    vNewPos.y -= ((vNewPos.y + yWorld) - levelTop);

                static float camSpeed = 0.0f;

                if(m_currentZoomInTime >= 0.0f)
                {
                    float alpha = m_currentZoomInTime / m_zoomTime;
                    alpha = pow(alpha, 2);

                    float targetDistance = m_zoomDistance * 0.7f;
                    float distance = alpha * m_zoomDistance + (1.0f - alpha) * targetDistance;

                    vNewPos.z = distance;

                    m_currentZoomInTime -= dt;
                }
                else if(m_currentZoomOutTime >= 0.0f)
                {
                    float alpha = m_currentZoomOutTime / m_zoomTime;
                    alpha = pow(alpha, 2);

                    float targetDistance = m_zoomDistance + m_zoomDistance * 0.3f;
                    float distance = alpha * m_zoomDistance + (1.0f - alpha) * targetDistance;

                    vNewPos.z = distance;

                    m_currentZoomOutTime -= dt;
                }
                
                if(gEngine->GetWorldMgr()->GetLevelJustStarted())
                {
                    vCamPos = vNewPos;
                    camSpeed = 0.0f;
                }
                else
                {
                    vCamPos = vNewPos;
                    camSpeed = 0.0f;
                }

                if(m_pTarget)
                {
                    // Check if player outside current screen
                    yWorld = abs(tan(hFovY) * vCamPos.z);
                    xWorld = abs(tan(hFovX) * vCamPos.z);

                    int charWidth = m_pCharacter->GetWidth();
                    int charHeight = m_pCharacter->GetHeight();

                    const Vec3& charPos = m_pCharacter->GetNode()->GetDerivedPosition();

                    float xWorldMin = vCamPos.x - xWorld;
                    float xWorldMax = vCamPos.x + xWorld;
                    float yWorldMin = vCamPos.y - yWorld;
                    float yWorldMax = vCamPos.y + yWorld;

                    if(charPos.x + charWidth < xWorldMin ||
                       charPos.x - charWidth >  xWorldMax ||
                       charPos.y + charHeight < yWorldMin ||
                       charPos.y - charHeight > yWorldMax)
                    {
                        m_pCharacter->SetKilled(true);
                    }
                }
            }

            if (m_bRespectLevelBoundaries)
                m_pNode->SetPosition(vCamPos);

            m_shakePosition = m_pNode->GetPosition();

            CCameraEntity::OnShake(dt);
        }
    }

    //-----------------------------------------------------------------------------------
    void CPlayerCameraEntity::OnGameModeChanged(bool bGameMode)
    {
        CCameraEntity::OnGameModeChanged(bGameMode);

        m_currentZoomInTime = -1.0f;
        m_currentZoomOutTime = -1.0f;

        m_pCharacter = static_cast<CCharacter3DEntity*>(gEngine->GetEntityMgr()->GetCharacter());

        Vec3 camPosition(0.0f, 0.0f, 0.0f);
        if(m_pCharacter)
            camPosition = m_pCharacter->GetNode()->GetDerivedPosition();

        if(m_pTarget)
            camPosition = m_pTarget->GetNode()->GetDerivedPosition();

        camPosition.z = m_initialPos.z;

        m_zoomDistance = camPosition.z;

        m_pNode->SetPosition(camPosition);

        m_bRespectLevelBoundaries = true;

        gEngine->GetWorldMgr()->SetActiveCamera(&m_camera);
    }

    //-----------------------------------------------------------------------------------
    void CPlayerCameraEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch(eventId)
        {
        case PLAYERCAMERA_EVENT_ZOOMIN:
            m_currentZoomInTime = m_zoomTime;
            m_currentZoomOutTime = -1.0f;
            m_zoomDistance = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition().z;
            break;

        case PLAYERCAMERA_EVENT_ZOOMOUT:
            m_currentZoomOutTime = m_zoomTime;
            m_currentZoomInTime = -1.0f;
            m_zoomDistance = gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition().z;
            break;

        default:
            CCameraEntity::OnInput(eventId, eventParameter);
            break;
        }
    }
} // env