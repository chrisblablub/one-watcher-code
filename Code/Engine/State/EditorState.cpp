#include <Engine/State/EditorState.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Scene/TerrainMgr.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Audio/AudioSystem.h>
#include <Engine/Scene/SceneMgr.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>
#include <Graphic/RenderSystems/RenderSystem.h>

#include <sstream>

namespace env
{
    const std::string CEditorState::STATE_NAME = "EditorState";

    //-----------------------------------------------------------------------------------
    CEditorState::CEditorState()
        : CState(STATE_NAME)
        , m_clickHandler(CLICK_HANDLER_PICK)
        , m_bFreeLook(false)
        , m_bValidGridIndex(false)
        , m_bSelectedTerrain(false)
        , m_lightingMode(RenderQueue::LIGHTING_MODE_DYNAMIC) {}

    //-----------------------------------------------------------------------------------
    CEditorState::~CEditorState() {}

    //-----------------------------------------------------------------------------------
    bool CEditorState::Load(const std::string& strPreviousState)
    {
        gEngine->GetInput()->SetListener(this, 2);
        gEngine->GetWorldMgr()->SetGameMode(false);

        m_bFreeLook = false;
        m_bSelectedTerrain = false;
        m_rightBtnDownTime = 0.0f;

        gEngine->GetWorldMgr()->SetActiveCamera(gEngine->GetWorldMgr()->GetDefaultCamera());

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEditorState::Unload()
    {
        CState::Unload();

        gEngine->GetWorldMgr()->SetGameMode(true);

        if(gEngine->GetWorldMgr()->GetLevelMgr() &&
            gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetLevelRenderObject())
        {
            gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetLevelRenderObject()->DestroyPreview();
        }

        if(gEngine->GetInput()->GetListener() == this)
            gEngine->GetInput()->UnsetListener();

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CEditorState::Update(float time, float dt, RenderQueue& renderQueue)
    {
//         gEngine->GetAudioSystem()->SetVolume(AUDIO_CHANNELGROUP_MUSIC, 0.0f);
//         gEngine->GetAudioSystem()->SetVolume(AUDIO_CHANNELGROUP_EFFECTS, 0.0f);
//         gEngine->GetAudioSystem()->SetVolume(AUDIO_CHANNELGROUP_AMBIENT, 0.0f);

        ProcessInput(dt, renderQueue);

        CWorldMgr* pWorldMgr = gEngine->GetWorldMgr();
        CCamera* pCamera = pWorldMgr->GetDefaultCamera();

        ComputeViewingRayDirection(pCamera->GetProjectionMatrix(), pCamera->GetViewMatrix(),
            gRenderSystem->GetRenderQualityParams().screenWidth, 
            gRenderSystem->GetRenderQualityParams().screenHeight,
            (float)gEngine->GetInput()->GetMouseX(), (float)gEngine->GetInput()->GetMouseY(), m_viewingRay);

        m_viewingOrigin = pCamera->GetAttachedNode()->GetDerivedPosition();


        unsigned int cellHoverX = 0, cellHoverY = 0;
        CTerrainMgr* pLevel = pWorldMgr->GetLevelMgr();
        if(pLevel->ComputeIntersectedGrid(m_viewingOrigin, m_viewingRay, cellHoverX, cellHoverY))
        {
            m_bValidGridIndex = true;

            if(m_clickHandler & CLICK_HANDLER_TERRAIN_MASK)
            {
                if(gEngine->GetInput()->IsKeyJustDown(VK_ADD))
                    ++pLevel->GetLevelGrid().GetCellEditParameters().brushSize;
                else if(gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
                    pLevel->GetLevelGrid().GetCellEditParameters().brushSize = std::max((int)(pLevel->GetLevelGrid().GetCellEditParameters().brushSize - 1), 1);

                pLevel->GetLevelGrid().GetLevelRenderObject()->CreatePreview(gEngine->GetInput()->IsKeyDown(VK_CONTROL), cellHoverX, cellHoverY, gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetCellEditParameters(), pLevel->GetLevelGrid().GetCellEditParameters().brushSize);
            }
            else
                pLevel->GetLevelGrid().GetLevelRenderObject()->DestroyPreview();
        }
        else
            m_bValidGridIndex = false;

        if (m_clickHandler == CLICK_HANDLER_PICK)
        {
            pLevel->GetLevelSelection().ClearSelection();
        }
        else if (m_clickHandler & CLICK_HANDLER_TERRAIN_MASK)
        {
            pLevel->GetLevelSelection().ClearSelection();
        }
        else if (m_clickHandler == CLICK_HANDLER_SELECTTERRAIN)
        {
            if (!m_bSelectedTerrain)
            {
                if (!gEngine->GetInput()->IsButtonDown(0))
                {
                    // Update selection dummy preview (single tile)
                    pLevel->GetLevelSelection().CreateSelection(pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY(), pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY());
                    pLevel->GetMouseHoverCell(m_selectionCellX, m_selectionCellY);
                }
                else if (!gEngine->GetInput()->IsButtonJustDown(0))
                {
                    if (!m_bSelectedTerrain)
                    {
                        // Start new selection if no selection has been made already
                        m_selectMaxX = pLevel->GetMouseHoverCellX();
                        m_selectMaxY = pLevel->GetMouseHoverCellY();

                        int oldSelectMinX = m_selectMinX;
                        int oldSelectMinY = m_selectMinY;

                        if (m_selectMinX > m_selectMaxX)
                            std::swap(m_selectMinX, m_selectMaxX);

                        if (m_selectMinY > m_selectMaxY)
                            std::swap(m_selectMinY, m_selectMaxY);

                        pLevel->GetLevelSelection().CreateSelection(m_selectMinX, m_selectMinY, m_selectMaxX, m_selectMaxY);
                        pLevel->GetMouseHoverCell(m_selectionCellX, m_selectionCellY);

                        m_selectMinX = oldSelectMinX;
                        m_selectMinY = oldSelectMinY;
                    }
                }
            }
            else if(gEngine->GetInput()->IsKeyDown(VK_CONTROL))
            {
                if (gEngine->GetInput()->IsKeyJustDown('1'))
                    pLevel->GetLevelSelection().SetSelectionLayer(0);
                else if (gEngine->GetInput()->IsKeyJustDown('2'))
                    pLevel->GetLevelSelection().SetSelectionLayer(1);
                else if (gEngine->GetInput()->IsKeyJustDown('3'))
                    pLevel->GetLevelSelection().SetSelectionLayer(2);
                else if (gEngine->GetInput()->IsKeyJustDown('4'))
                    pLevel->GetLevelSelection().SetSelectionLayer(3);
                else if (gEngine->GetInput()->IsKeyJustDown('5'))
                    pLevel->GetLevelSelection().SetSelectionLayer(4);
                else if (gEngine->GetInput()->IsKeyJustDown('6'))
                    pLevel->GetLevelSelection().SetSelectionLayer(5);
                else if (gEngine->GetInput()->IsKeyJustDown('7'))
                    pLevel->GetLevelSelection().SetSelectionLayer(6);
                else if (gEngine->GetInput()->IsKeyJustDown('8'))
                    pLevel->GetLevelSelection().SetSelectionLayer(7);
                else if (gEngine->GetInput()->IsKeyJustDown('9'))
                    pLevel->GetLevelSelection().SetSelectionLayer(8);
            }
            
        }
        /*else if (m_clickHandler == CLICK_HANDLER_SELECTTERRAIN)
        {
            if (!gEngine->GetInput()->IsButtonDown(0))
            {
                int cellX = pLevel->GetMouseHoverCellX();
                int cellY = pLevel->GetMouseHoverCellY();

                int brushOffset = pLevel->GetLevelGrid().GetCellEditParameters().brushSize / 2;

                if (pLevel->GetLevelGrid().GetCellEditParameters().brushSize % 2 == 0)
                    pLevel->GetLevelSelection().CreateSelection(cellX - brushOffset, cellY - brushOffset, cellX + brushOffset - 1, cellY + brushOffset - 1);
                else
                    pLevel->GetLevelSelection().CreateSelection(cellX - brushOffset, cellY - brushOffset, cellX + brushOffset, cellY + brushOffset);

                pLevel->GetMouseHoverCell(m_selectionCellX, m_selectionCellY);
            }
        }*/



        
        float moveSpeed = 700.0f * dt;
        CSceneNode* pCamNode = pCamera->GetAttachedNode();
        if(m_bFreeLook)
        {
            pCamNode->Translate(pCamNode->GetDirection() * (gEngine->GetInput()->IsKeyDown('W') * moveSpeed));
            pCamNode->Translate(-pCamNode->GetDirection() * (gEngine->GetInput()->IsKeyDown('S') * moveSpeed));
            pCamNode->Translate(-pCamNode->GetRight() * (gEngine->GetInput()->IsKeyDown('A') * moveSpeed));
            pCamNode->Translate(pCamNode->GetRight() * (gEngine->GetInput()->IsKeyDown('D') * moveSpeed));


            Vec3 vCamPos = pCamNode->GetPosition();

            if(!gEngine->GetInput()->IsKeyDown(VK_CONTROL) && !gEngine->GetInput()->IsKeyDown(VK_SHIFT))
                vCamPos += pCamNode->GetDirection() * (gEngine->GetInput()->GetMouseDZ() * 60.0f);

            pCamNode->SetPosition(vCamPos);

        }
        else
        {
            Vec3 vCamPos = pCamNode->GetPosition();

            if(!gEngine->GetInput()->IsKeyDown(VK_CONTROL) && !gEngine->GetInput()->IsKeyDown(VK_SHIFT))
                vCamPos += pCamNode->GetDirection() * (gEngine->GetInput()->GetMouseDZ() * 60.0f);

            vCamPos += pCamNode->GetUp() * (gEngine->GetInput()->IsKeyDown('W') * moveSpeed);
            vCamPos += -pCamNode->GetUp() * (gEngine->GetInput()->IsKeyDown('S') * moveSpeed);
            vCamPos += -pCamNode->GetRight() * (gEngine->GetInput()->IsKeyDown('A') * moveSpeed);
            vCamPos += pCamNode->GetRight() * (gEngine->GetInput()->IsKeyDown('D') * moveSpeed);

            pCamNode->SetPosition(vCamPos);
        }

        if (gEngine->GetInput()->IsButtonDown(2))
        {
            Vec3 vCamPos = pCamNode->GetPosition();

            vCamPos += pCamNode->GetUp() * (gEngine->GetInput()->GetMouseDY() * moveSpeed * 0.1f);
            vCamPos += pCamNode->GetUp() * (gEngine->GetInput()->GetMouseDY() * moveSpeed * 0.1f);
            vCamPos += pCamNode->GetRight() * (-gEngine->GetInput()->GetMouseDX() * moveSpeed * 0.1f);
            vCamPos += pCamNode->GetRight() * (-gEngine->GetInput()->GetMouseDX() * moveSpeed * 0.1f);

            pCamNode->SetPosition(vCamPos);
        }

        std::wstringstream str2;
        if(!pWorldMgr->GetGameMode())
        {
            str2 << std::endl << "MODE: EDIT" << std::endl;

            TextRenderObject text;
            text.m_strText = str2.str();
            //text.m_position = VECTOR2(0.9f, 0.1f);
            //renderQueue.m_textRenderObjects["FontGameHeader"].push_back(text);
        }


        /*if(pWorldMgr->GetDefaultCamera())
        {
            const VECTOR3& pos = pWorldMgr->GetDefaultCamera()->GetAttachedNode()->GetDerivedPosition();
            const CQuaternion& quat = pWorldMgr->GetDefaultCamera()->GetAttachedNode()->GetDerivedOrientation();

            std::stringstream str1;
            str1 << "CAM POSITION: " << pos.x << "  " << pos.y << "  " << pos.z << std::endl;
            renderQueue.text["FontGameHeader"].push_back(str1.str());

            std::stringstream str2;
            str2 << "CAM ORIENTATION: " << quat.GetYaw() << "  " << quat.GetPitch() << "  " << quat.GetRoll() << std::endl;
            renderQueue.text["FontGameHeader"].push_back(str2.str());
        }*/

        
    }

    //-----------------------------------------------------------------------------------
    void CEditorState::ProcessInput(float dt, RenderQueue& renderQueue)
    {
        CEntity* pActiveEntity = gEngine->GetEntityMgr()->GetActiveEntity();
        CTerrainMgr* pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

        static bool bSFX = true;
        static bool bMusic = false;
        static bool bRenderMeshes = true;
        static bool bRenderLevelTerrain = true;
        static bool bRenderLevelBorder = true;
        static bool bRenderParticles = true;
        static bool bRenderDebugData = false;
        static bool bShowFPS = true;
        static bool bPaused = false;


        if (gEngine->GetInput()->IsKeyDown('U'))
        {
            std::string strFade;
            std::string strStripe;

            if (gEngine->GetInput()->IsKeyJustDown('1'))
            {
                strFade = "../Fades/tmpl_chap1_orig_01.fds";
                strStripe = "../Stripes/tmpl_chap1_orig_01.stp";
            }
            else if (gEngine->GetInput()->IsKeyJustDown('2'))
            {
                strFade = "../Fades/tmpl_chap2_orig_01.fds";
                strStripe = "../Stripes/tmpl_chap2_orig_01.stp";
            }
            else if (gEngine->GetInput()->IsKeyJustDown('3'))
            {
                strFade = "../Fades/tmpl_chap3_orig_01.fds";
                strStripe = "../Stripes/tmpl_chap3_orig_01.stp";
            }
            else if (gEngine->GetInput()->IsKeyJustDown('4'))
            {
                strFade = "../Fades/tmpl_chap4_orig_01.fds";
                strStripe = "../Stripes/tmpl_chap4_orig_01.stp";
            }

            if (!strFade.empty())
            {
                CFadeEntity* pFade = static_cast<CFadeEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_FADE));
                CStripesEntity* pStripe = static_cast<CStripesEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_STRIPES));
                CCheckpointOriginEntity* pOrigin = static_cast<CCheckpointOriginEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_CHECKPOINT_ORIGIN));
                
                pOrigin->SetResetTrigger(false);

                pFade->LoadTemplate(strFade);
                pStripe->LoadTemplate(strStripe);

                gEngine->GetEntityMgr()->AddEntityOutput(pOrigin, CEntity::EntityOutput(pStripe, CStripesEntity::STRIPE_EVENT_EXECUTE));
                gEngine->GetEntityMgr()->AddEntityOutput(pOrigin, CEntity::EntityOutput(pFade, CFadeEntity::FADE_EVENT_FRAME_EXECUTE));

                pFade->GetNode()->SetPosition(pOrigin->GetNode()->GetDerivedPosition() + Vec3(-25.0f, 50.0f, 0.0f));
                pStripe->GetNode()->SetPosition(pOrigin->GetNode()->GetDerivedPosition() + Vec3(25.0f, 50.0f, 0.0f));
            }
        }
        if (gEngine->GetInput()->IsKeyDown('I'))
        {
            std::string strFade;
            std::string strStripe;

            if (gEngine->GetInput()->IsKeyJustDown('1'))
            {
                strFade = "../Fades/tmpl_chap1_dest_01.fds";
                strStripe = "../Stripes/tmpl_chap1_dest_01.stp";
            }
            else if(gEngine->GetInput()->IsKeyJustDown('2'))
            {
                strFade = "../Fades/tmpl_chap2_dest_01.fds";
                strStripe = "../Stripes/tmpl_chap2_dest_01.stp";
            }
            else if (gEngine->GetInput()->IsKeyJustDown('3'))
            {
                strFade = "../Fades/tmpl_chap3_dest_01.fds";
                strStripe = "../Stripes/tmpl_chap3_dest_01.stp";
            }
            else if (gEngine->GetInput()->IsKeyJustDown('4'))
            {
                strFade = "../Fades/tmpl_chap4_dest_01.fds";
                strStripe = "../Stripes/tmpl_chap4_dest_01.stp";
            }

            if (!strFade.empty())
            {
                CFadeEntity* pFade = static_cast<CFadeEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_FADE));
                CStripesEntity* pStripe = static_cast<CStripesEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_STRIPES));
                CCheckpointDestinatonEntity* pDest = static_cast<CCheckpointDestinatonEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_CHECKPOINT_DESTINATION));

                pFade->LoadTemplate(strFade);
                pStripe->LoadTemplate(strStripe);

                gEngine->GetEntityMgr()->AddEntityOutput(pDest, CEntity::EntityOutput(pStripe, CStripesEntity::STRIPE_EVENT_EXECUTE, 1.0f));
                gEngine->GetEntityMgr()->AddEntityOutput(pDest, CEntity::EntityOutput(pFade, CFadeEntity::FADE_EVENT_FRAME_EXECUTE));

                pFade->GetNode()->SetPosition(pDest->GetNode()->GetDerivedPosition() + Vec3(-25.0f, 50.0f, 0.0f));
                pStripe->GetNode()->SetPosition(pDest->GetNode()->GetDerivedPosition() + Vec3(25.0f, 50.0f, 0.0f));
            }
        }
        if (gEngine->GetInput()->IsKeyJustDown(VK_SPACE))
        {
            bPaused = !bPaused;
        }
        if (gEngine->GetInput()->IsKeyJustDown(VK_RETURN))
        {
            gEngine->SetNewState("GameState");
        }
        if (gEngine->GetInput()->IsKeyJustDown(VK_ESCAPE))
        {
            gEngine->GetWorldMgr()->SetActiveCamera(gEngine->GetWorldMgr()->GetDefaultCamera());
        }

        if (gEngine->GetInput()->IsKeyDown(VK_SHIFT))
        {
            if (gEngine->GetInput()->GetMouseDZ() > 0)
                pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation = (CELL_ROTATION)((pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation + 1) % ((int)CELL_ROTATION_270DEG_CW + 1));
            else if (gEngine->GetInput()->GetMouseDZ() < 0)
            {
                if (pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation == 0)
                    pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation = CELL_ROTATION_270DEG_CW;
                else
                    pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation = (CELL_ROTATION)(((int)pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation - 1) % ((int)CELL_ROTATION_270DEG_CW + 1));
            }
        }

        if (gEngine->GetInput()->IsKeyJustDown(VK_F5))
        {
            bRenderMeshes = !bRenderMeshes;
        }
        else if (gEngine->GetInput()->IsKeyJustDown(VK_F6))
        {
            bRenderLevelTerrain = !bRenderLevelTerrain;
        }
        else if (gEngine->GetInput()->IsKeyJustDown(VK_F7))
        {
            bRenderLevelBorder = !bRenderLevelBorder;
        }
        else if (gEngine->GetInput()->IsKeyJustDown(VK_F8))
        {
            bRenderParticles = !bRenderParticles;
        }
        else if (gEngine->GetInput()->IsKeyJustDown(VK_F9))
        {
            bRenderDebugData = !bRenderDebugData;
        }

        unsigned int cellX, cellY;
        pLevel->GetMouseHoverCell(cellX, cellY);

        int diffX = (int)cellX - (int)m_selectionCellX;
        int diffY = (int)cellY - (int)m_selectionCellY;
        pLevel->GetLevelSelection().SetSelectionOffset(diffX, diffY);

        if (m_clickHandler == CLICK_HANDLER_EDITTERRAIN_DIFFUSE ||
            m_clickHandler == CLICK_HANDLER_EDITTERRAIN_EMISSIVE)
        {
            TerrainCell* pTerrainCell = nullptr;
            if (pLevel->GetLevelGrid().FindCell(&pTerrainCell, pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY()))
            {
                LevelCellEditParameters& params = pLevel->GetLevelGrid().GetCellEditParameters();

                LevelHalfCell* pCell = nullptr;

                if (pTerrainCell->pFirst)
                    pCell = pTerrainCell->pFirst;
                else
                    pCell = pTerrainCell->pSecond;

                if (pCell)
                {
                    params.currentEditSequence = pCell->sequence;
                    params.currentEditLayer = pCell->depthLayer;
                    params.currentEditCellLayout = CELL_LAYOUT_FULL;
                    params.currentEditRotation = pCell->cellRotation;

                    if (m_clickHandler == CLICK_HANDLER_EDITTERRAIN_DIFFUSE)
                        params.currentEditEmissiveColor = pCell->emissiveColor;
                    else
                        params.currentEditColor = pCell->layerColor;
                }
            }
        }

        if (gEngine->GetInput()->IsKeyDown(VK_CONTROL))
        {
            if (gEngine->GetInput()->IsKeyJustDown(VK_F12))
            {
                bMusic = !bMusic;

                gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_MUSIC, !bMusic);
                gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_EFFECTS, !bMusic);
                gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_AMBIENT, !bMusic);
                gEngine->GetAudioSystem()->SetMute(AUDIO_CHANNELGROUP_VOICE, !bMusic);
            }
            else if (gEngine->GetInput()->IsKeyJustDown('F'))
            {
                bShowFPS = !bShowFPS;
            }
            else if (gEngine->GetInput()->IsKeyJustDown('R'))
            {
                gEngine->SetReloadResources(!gEngine->GetReloadResources());
            }

            if (gEngine->GetInput()->GetMouseDZ() > 0.0)
                pLevel->GetLevelGrid().SetEditSequence(pLevel->GetLevelGrid().GetCellEditParameters().currentEditSequence + 1);
            else if (gEngine->GetInput()->GetMouseDZ() < 0.0)
                pLevel->GetLevelGrid().SetEditSequence(pLevel->GetLevelGrid().GetCellEditParameters().currentEditSequence - 1);

            if (!pActiveEntity)
            {
                for (int i = 0; i < 9; ++i)
                {
                    if (gEngine->GetInput()->IsKeyJustDown(49 + i))
                    {
                        pLevel->GetLevelGrid().GetCellEditParameters().currentEditColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(i);
                        pLevel->GetLevelGrid().GetCellEditParameters().currentEditEmissiveColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(i);
                        pLevel->GetLevelGrid().GetCellEditParameters().currentEditLayer = i;
                    }
                }
            }
            else
            {
                if (gEngine->GetInput()->IsKeyJustDown('1'))
                    pActiveEntity->SetCollision(true);
                else if (gEngine->GetInput()->IsKeyJustDown('2'))
                    pActiveEntity->SetCollision(false);
                else if (gEngine->GetInput()->IsKeyJustDown('3'))
                    pActiveEntity->SetCollision(false);
                else if (gEngine->GetInput()->IsKeyJustDown('4'))
                    pActiveEntity->SetCollision(false);
            }
        }
        else
        {
            if (gEngine->GetInput()->IsKeyJustDown('T'))
            {
                TerrainCell* pCell = nullptr;
                gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().FindCell(&pCell, cellX, cellY);

                static int t = 0;
                if (pCell)
                {
                    LevelHalfCell* pEditCell = nullptr;
                    if (pCell->pFirst &&
                        pCell->pSecond)
                    {
                        if (t % 2 == 0)
                            pEditCell = pCell->pFirst;
                        else
                            pEditCell = pCell->pSecond;

                        ++t;
                    }
                    else if (pCell->pFirst)
                        pEditCell = pCell->pFirst;
                    else if (pCell->pSecond)
                        pEditCell = pCell->pSecond;

                    if (pEditCell)
                    {
                        LevelCellEditParameters& params = gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetCellEditParameters();

                        params.currentEditBorderColor = pEditCell->borderColor;
                        params.currentEditCellLayout = pEditCell->cellLayout;
                        params.currentEditColor = pEditCell->layerColor;
                        params.currentEditEmissiveColor = pEditCell->emissiveColor;
                        params.currentEditLayer = pEditCell->depthLayer;
                        params.currentEditRotation = pEditCell->cellRotation;
                        params.currentEditSequence = pEditCell->sequence;
                    }
                }
            }
            else if (gEngine->GetInput()->IsKeyJustDown('Q'))
            {
                gEngine->GetWorldMgr()->GetEntitySlider().SetMode(CEntitySlider::TransformMode::MOVE);
            }
            else if (gEngine->GetInput()->IsKeyJustDown('E'))
            {
                gEngine->GetWorldMgr()->GetEntitySlider().SetMode(CEntitySlider::TransformMode::ROTATE);
            }
            else if (gEngine->GetInput()->IsKeyJustDown('H'))
            {
                gEngine->GetEntityMgr()->SetSelectionOutline(!gEngine->GetEntityMgr()->GetSelectionOutline());
            }
            else if (gEngine->GetInput()->IsKeyJustDown('M'))
            {
                gEngine->GetWorldMgr()->GetEntitySlider().SetMode(CEntitySlider::TransformMode::MOVE);
            }
            else if (gEngine->GetInput()->IsKeyJustDown('R'))
            {
                if (m_clickHandler == CLICK_HANDLER_EDITTERRAIN)
                {
                    if (gEngine->GetInput()->IsKeyDown(VK_SHIFT))
                    {
                        if (pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation == 0)
                            pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation = CELL_ROTATION_270DEG_CW;
                        else
                            pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation = (CELL_ROTATION)(((int)pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation - 1) % ((int)CELL_ROTATION_270DEG_CW + 1));
                    }
                    else
                        pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation = (CELL_ROTATION)((pLevel->GetLevelGrid().GetCellEditParameters().currentEditRotation + 1) % ((int)CELL_ROTATION_270DEG_CW + 1));
                }
            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_F2))
            {
                std::list< CEntity* > particleSystems;
                gEngine->GetEntityMgr()->GetEntities(particleSystems, ENTITY_TYPEID_PARTICLESYSTEM);

                for(CEntity* p : particleSystems)
                {
                    CParticleSystemEntity* pParticleSys = static_cast<CParticleSystemEntity*>(p);

                    if (pParticleSys && !pParticleSys->GetParticleSystemTemplate().empty())
                    {
                        pParticleSys->LoadFromTemplate(pParticleSys->GetParticleSystemTemplate());
                    }
                }
            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_F1))
            {
                if (!m_bFreeLook)
                {
                    m_bFreeLook = true;

                    m_clickHandler = CLICK_HANDLER_PICK;
                }
                else
                {
                    m_bFreeLook = false;

                    CSceneNode* pCamNode = gEngine->GetWorldMgr()->GetDefaultCamera()->GetAttachedNode();
                    Vec3 vPos = pCamNode->GetPosition();
                    vPos.z = -1000.0f;
                    pCamNode->SetPosition(vPos);
                    pCamNode->SetOrientation(CQuaternion());

                    m_clickHandler = CLICK_HANDLER_PICK;
                }
            }
            else if (gEngine->GetInput()->IsKeyJustDown('O'))
            {
                CEntity* pEntity = gEngine->GetEntityMgr()->GetActiveEntity();
                if (pEntity)
                {
                    CEntity* pNewEntity = gEngine->GetEntityMgr()->CreateEntity(pEntity);

                    gEngine->GetWorldMgr()->GetEntitySlider().DeselectSliders();
                    gEngine->GetWorldMgr()->GetEntitySlider().Attach(pNewEntity->GetNode());

                    gEngine->GetEntityMgr()->SetActiveEntity(pNewEntity);
                }
            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_DELETE))
            {
                // Delete entity or selected terrain
                //
                if (pActiveEntity && pActiveEntity->GetDeletable())
                {
                    gEngine->GetWorldMgr()->GetEntitySlider().Detach();

                    gEngine->GetEntityMgr()->RemoveEntity(pActiveEntity->GetName());

                    pActiveEntity = 0;
                }
                else if (m_bSelectedTerrain)
                {
                    unsigned int cellX, cellY;
                    pLevel->GetMouseHoverCell(cellX, cellY);

                    int diffX = (int)cellX - (int)m_selectionCellX;
                    int diffY = (int)cellY - (int)m_selectionCellY;
                    pLevel->GetLevelSelection().SetSelectionOffset(diffX, diffY);
                    pLevel->GetLevelSelection().RemoveSelection();
                }
            }
            else if (gEngine->GetInput()->IsKeyJustDown('F'))
            {
                gEngine->GetWorldMgr()->GetEntitySlider().EnableSnap(!gEngine->GetWorldMgr()->GetEntitySlider().IsSnapEnabled());
            }
            else if (gEngine->GetInput()->IsKeyJustDown('B'))
            {
                gEngine->GetSceneMgr()->SetBoundingBoxesVisible(!gEngine->GetSceneMgr()->AreBoundingBoxesVisible());
            }
            else if (gEngine->GetInput()->IsKeyJustDown('1'))
            {
                pLevel->GetLevelGrid().GetCellEditParameters().currentEditCellLayout = CELL_LAYOUT_FULL;
            }
            else if (gEngine->GetInput()->IsKeyJustDown('2'))
            {
                pLevel->GetLevelGrid().GetCellEditParameters().currentEditCellLayout = CELL_LAYOUT_BOTLEFT;
            }
            else if (gEngine->GetInput()->IsKeyJustDown('3'))
            {
                pLevel->GetLevelGrid().GetCellEditParameters().currentEditCellLayout = CELL_LAYOUT_BOTRIGHT;
            }
            else if (gEngine->GetInput()->IsKeyJustDown('4'))
            {
                pLevel->GetLevelGrid().GetCellEditParameters().currentEditCellLayout = CELL_LAYOUT_TOPLEFT;
            }
            else if (gEngine->GetInput()->IsKeyJustDown('5'))
            {
                pLevel->GetLevelGrid().GetCellEditParameters().currentEditCellLayout = CELL_LAYOUT_TOPRIGHT;
            }
        }


        if (pActiveEntity &&
            pActiveEntity->GetSlidable())
        {
            CSceneNode* pNode = pActiveEntity->GetNode();

            if (gEngine->GetInput()->IsKeyJustDown(VK_ADD))
            {
                if (gEngine->GetInput()->IsKeyDown('X'))
                {
                    Vec3 scale = pNode->GetScale();
                    scale.x += 0.05f;
                    pNode->SetScale(scale);
                }
                else if (gEngine->GetInput()->IsKeyDown('Y'))
                {
                    Vec3 scale = pNode->GetScale();
                    scale.y += 0.05f;
                    pNode->SetScale(scale);
                }
                else if (gEngine->GetInput()->IsKeyDown('Z'))
                {
                    Vec3 scale = pNode->GetScale();
                    scale.z += 0.05f;
                    pNode->SetScale(scale);
                }
                else
                {
                    Vec3 scale = pNode->GetScale();
                    scale += Vec3(0.05f, 0.05f, 0.05f);
                    pNode->SetScale(scale);
                }

            }
            else if (gEngine->GetInput()->IsKeyJustDown(VK_SUBTRACT))
            {
                if (gEngine->GetInput()->IsKeyDown('X'))
                {
                    Vec3 scale = pNode->GetScale();
                    if (scale.x > 0.05f)
                        scale.x -= 0.05f;
                    pNode->SetScale(scale);
                }
                else if (gEngine->GetInput()->IsKeyDown('Y'))
                {
                    Vec3 scale = pNode->GetScale();
                    if (scale.y > 0.05f)
                        scale.y -= 0.05f;
                    pNode->SetScale(scale);
                }
                else if (gEngine->GetInput()->IsKeyDown('Z'))
                {
                    Vec3 scale = pNode->GetScale();
                    if (scale.z > 0.05f)
                        scale.z -= 0.05f;
                    pNode->SetScale(scale);
                }
                else
                {
                    Vec3 scale = pNode->GetScale();
                    if (scale.x > 0.05f && scale.y > 0.05f && scale.z > 0.05f)
                        scale -= Vec3(0.05f, 0.05f, 0.05f);
                    pNode->SetScale(scale);
                }
            }
        }

        gEngine->SetPaused(bPaused);

        renderQueue.bRenderMeshes = bRenderMeshes;
        renderQueue.bRenderLevelTerrain = bRenderLevelTerrain;
        renderQueue.bRenderLevelBorder = bRenderLevelBorder;
        renderQueue.bRenderParticles = bRenderParticles;
        renderQueue.bRenderDebugData = bRenderDebugData;
        renderQueue.lightingMode = m_lightingMode;

        if (bShowFPS)
        {
            std::stringstream str;
            str << "FPs_*: " << (int)gEngine->GetFramerate() << std::endl;

            TextRenderObject fpsText(str.str(), Vec4(0, 1, 0, 1));
            fpsText.m_bScreenSpace = true;
            //renderQueue.m_textRenderObjects[FontInfo("FranklinGothicMedium", 32)].push_back(fpsText);
        }

        /*
        HACK! Removes specific particle systems.
        if(gEngine->GetInput()->IsKeyJustDown(VK_CONTROL))
        {
            const CEntityMgr::mapEntities& entities = gEngine->GetEntityMgr()->GetEntities();
            std::list< CEntity* > toBeRemoved;
            for(CEntityMgr::mapEntities::const_iterator it = entities.begin(); it != entities.end(); ++it)
            {
                if(it->second->GetMetaTypeId() == ENTITY_TYPEID_PARTICLESYSTEM)
                {
                    CParticleSystemEntity* pEntity = (CParticleSystemEntity*)it->second;

                    if(pEntity->GetParticleSystemTemplate() == "BEAM_T2")
                    {
                        toBeRemoved.push_back(pEntity);
                    }
                }
            }

            for(std::list< CEntity* >::iterator it = toBeRemoved.begin(); it != toBeRemoved.end(); ++it)
            {
                gEngine->GetEntityMgr()->RemoveEntity((*it)->GetName());
            }
            }*/
    }

    //-----------------------------------------------------------------------------------
    void CEditorState::LoadTerrainTemplate(const std::string& strFilename)
    {
        gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelSelection().LoadTemplate(strFilename);

        CEditorState* pEditorState = static_cast<CEditorState*>(gEngine->GetState("EditorState"));
        m_bSelectedTerrain = true;

        CTerrainSelection& terrainSelection = gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelSelection();
        m_selectionCellX = terrainSelection.GetSelectionMinX() + (terrainSelection.GetSelectionMaxX() - terrainSelection.GetSelectionMinX()) / 2;
        m_selectionCellY = terrainSelection.GetSelectionMinY() + (terrainSelection.GetSelectionMaxY() - terrainSelection.GetSelectionMinY()) / 2;

        m_selectMinX = terrainSelection.GetSelectionMinX();
        m_selectMaxX = terrainSelection.GetSelectionMaxX();
        m_selectMinY = terrainSelection.GetSelectionMinY();
        m_selectMaxY = terrainSelection.GetSelectionMaxY();
    }

    //-----------------------------------------------------------------------------------
    void CEditorState::OnNotified(unsigned long id, const MouseInputEvent& Event)
    {
        
        if(m_clickHandler == CLICK_HANDLER_PAINTTERRAIN)
        {
            if (Event.event == KEY_STATUS_DOWN &&
                Event.btn < 2)
            {
                CTerrainMgrCPU& grid = gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();
                grid.Paint(Event.btn == 0, gEngine->GetWorldMgr()->GetLevelMgr()->GetMouseHoverCellX(), gEngine->GetWorldMgr()->GetLevelMgr()->GetMouseHoverCellY(), 
                    gEngine->GetWorldMgr()->GetLevelMgr()->GetMouseIntersection(), gEngine->GetTimeElapsed());
            }

        }
        else if(m_clickHandler == CLICK_HANDLER_PICK)
        {
            if(Event.btn == 0 && Event.event == KEY_STATUS_JUST_DOWN)
            {
                // Picking entities..
                float distance = 0, lastDistance = FLT_MAX;
                CSceneNode* pNode;

                const CEntityMgr::mapEntities& entities = gEngine->GetEntityMgr()->GetEntities();
                
                static unsigned int lastSelected = 0;
                typedef std::pair<CEntity*, CSceneNode* > pairt;
                std::vector< pairt > candidates;
                for(CEntityMgr::mapEntities::const_iterator it = entities.begin(); it != entities.end(); ++it)
                {
#ifdef ENV_PLATFORM_WINDOWS
                    CEntity* pEntity = it->second;

                    switch(m_entityPickType)
                    {
                    case ENTITY_PICK_TYPE_ALL:
                        break;
                    case ENTITY_PICK_TYPE_SPRITES:
                        if(!dynamic_cast< CSpriteEntity* >(pEntity))
                            continue;
                        break;
                    case ENTITY_PICK_TYPE_ENTITIES:
                        if(dynamic_cast< CSpriteEntity* >(pEntity) || 
                            (dynamic_cast< CMeshEntity* >(pEntity) && 
                             pEntity->GetMetaTypeId() == CMeshEntity::ENTITY_META_TYPEID))
                            continue;
                        break;
                    case ENTITY_PICK_TYPE_MESHES:
                        if(!dynamic_cast< CMeshEntity* >(pEntity) || pEntity->GetMetaTypeId() != CMeshEntity::ENTITY_META_TYPEID)
                            continue;
                        break;
                    }
#endif // ENV_PLATFORM_WINDOWS

                    if((it->second->GetPickable() || (gEngine->GetInput()->IsKeyDown(VK_SHIFT))) && 
                       it->second->HasCollisionWithRay(m_viewingOrigin, m_viewingRay, distance, &pNode) /*&& distance < lastDistance*/ )
                    {
                        lastDistance = distance;

                        candidates.push_back(pairt(it->second, pNode));
                    }
                }

                CEntitySlider& slider = gEngine->GetWorldMgr()->GetEntitySlider();
                if(!slider.IsSliderSelected() && !candidates.empty())
                {
                    if(lastSelected >= candidates.size())
                        lastSelected = 0;

                    CEntity* p = candidates[lastSelected].first;

                    if(gEngine->GetEntityMgr()->GetActiveEntity() != p)
                    {
                        slider.DeselectSliders();

                        if (p->GetSlidable())
                            slider.Attach(candidates[lastSelected].second);
                        else
                            slider.Detach();

                        gEngine->GetEntityMgr()->SetActiveEntity(p);	

                        ++lastSelected;
                    }
                }
            }
            else if(Event.btn == 1 && Event.event == KEY_STATUS_JUST_UP && m_rightBtnDownTime < 0.2f)
            {
                CEntitySlider& slider = gEngine->GetWorldMgr()->GetEntitySlider();
                slider.DeselectSliders();
                slider.Detach();

                gEngine->GetEntityMgr()->SetActiveEntity(NULL);

                if(gEngine->GetInput()->IsKeyDown(VK_CONTROL))
                {
                    gEngine->GetInput()->UnsetListener();
                } 
            }
        }
        else if(!m_bFreeLook)
        {
            CTerrainMgr* pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

            if (m_clickHandler & CLICK_HANDLER_TERRAIN_MASK)
            {
                gEngine->GetWorldMgr()->GetEntitySlider().Detach();
                gEngine->GetEntityMgr()->SetActiveEntity(nullptr);
            }

            if (Event.event == KEY_STATUS_DOWN && (m_clickHandler & CLICK_HANDLER_TERRAIN_MASK) && m_bValidGridIndex)
            {
                if (m_clickHandler == CLICK_HANDLER_EDITTERRAIN)
                {
                    if (Event.btn == 0)
                    {
                        if (gEngine->GetInput()->IsKeyDown('Q'))
                        {
                            LevelCellEditParameters& editParams = pLevel->GetLevelGrid().GetCellEditParameters();
                            editParams.bRandomRotation = true;
                        }
                        else
                        {
                            LevelCellEditParameters& editParams = pLevel->GetLevelGrid().GetCellEditParameters();
                            editParams.bRandomRotation = false;
                        }

                        pLevel->GetLevelGrid().SetCell(pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY(), gEngine->GetInput()->IsKeyDown(VK_CAPITAL));
                    }
                    else if (Event.btn == 1)
                    {
                        pLevel->GetLevelGrid().UnsetCell(pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY());
                    }
                }
                else if (m_clickHandler == CLICK_HANDLER_EDITTERRAIN_DIFFUSE && Event.btn == 0)
                {
                    TerrainCell* pTerrainCell = nullptr;
                    if (pLevel->GetLevelGrid().FindCell(&pTerrainCell, pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY()))
                    {
                        if (pTerrainCell->pFirst)
                            pTerrainCell->pFirst->layerColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(pTerrainCell->pFirst->depthLayer);

                        if (pTerrainCell->pSecond)
                            pTerrainCell->pSecond->layerColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(pTerrainCell->pSecond->depthLayer);

                        std::vector<unsigned int> cells;
                        cells.push_back(pLevel->GetMouseHoverCellIndex());
                        pLevel->GetLevelGrid().GetLevelRenderObject()->UpdateTerrainBuffers(cells);
                    }
                }
                else if (m_clickHandler == CLICK_HANDLER_EDITTERRAIN_EMISSIVE && Event.btn == 0)
                {
                    TerrainCell* pTerrainCell = nullptr;
                    if (pLevel->GetLevelGrid().FindCell(&pTerrainCell, pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY()))
                    {
                        if (pTerrainCell->pFirst)
                            pTerrainCell->pFirst->emissiveColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(pTerrainCell->pFirst->depthLayer);

                        if (pTerrainCell->pSecond)
                            pTerrainCell->pSecond->emissiveColor = gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(pTerrainCell->pSecond->depthLayer);

                        std::vector<unsigned int> cells;
                        cells.push_back(pLevel->GetMouseHoverCellIndex());
                        pLevel->GetLevelGrid().GetLevelRenderObject()->UpdateTerrainBuffers(cells);
                    }
                }
//                 else if (m_clickHandler == CLICK_HANDLER_PICKTERRAIN && Event.btn == 0)
//                 {
// 
//                     TerrainCell* pTerrainCell = nullptr;
//                     if (pLevel->GetLevelGrid().FindCell(&pTerrainCell, pLevel->GetMouseHoverCellX(), pLevel->GetMouseHoverCellY()))
//                     {
//                         LevelCellEditParameters& params = pLevel->GetLevelGrid().GetCellEditParameters();
// 
//                         LevelHalfCell* pCell = nullptr;
// 
//                         if (pTerrainCell->pFirst)
//                             pCell = pTerrainCell->pFirst;
//                         else
//                             pCell = pTerrainCell->pSecond;
// 
//                         if (pCell)
//                         {
//                             params.currentEditSequence = pCell->sequence;
//                             params.currentEditLayer = pCell->depthLayer;
//                             params.currentEditCellLayout = CELL_LAYOUT_FULL;
//                             params.currentEditRotation = pCell->cellRotation;
// 
//                             if (m_clickHandler == CLICK_HANDLER_EDITTERRAIN_DIFFUSE)
//                                 params.currentEditEmissiveColor = pCell->emissiveColor;
//                             else
//                                 params.currentEditColor = pCell->layerColor;
//                         }
//                     }
//                 }
            }
            else if(m_clickHandler == CLICK_HANDLER_SELECTTERRAIN)
            {
                if(Event.btn == 1)
                {
                    m_selectMinX = m_selectMinY = m_selectMaxX = m_selectMaxY = 0;
                    m_bSelectedTerrain = false;
                    pLevel->GetLevelGrid().GetLevelRenderObject()->RemoveSelection();
                }
                else if(Event.btn == 0)
                {
                    if(Event.event == KEY_STATUS_JUST_DOWN)
                    {
                        if(!m_bSelectedTerrain)
                        {
                            m_selectMinX = pLevel->GetMouseHoverCellX();
                            m_selectMinY = pLevel->GetMouseHoverCellY();
                        }
                        else
                        {
                            pLevel->GetLevelSelection().MergeSelection();
                        }
                    }
                    else if(Event.event == KEY_STATUS_JUST_UP)
                    {
                        if(!m_bSelectedTerrain)
                        {
                            m_selectMaxX = pLevel->GetMouseHoverCellX();
                            m_selectMaxY = pLevel->GetMouseHoverCellY();

                            if(m_selectMinX > m_selectMaxX)
                                std::swap(m_selectMinX, m_selectMaxX);

                            if(m_selectMinY > m_selectMaxY)
                                std::swap(m_selectMinY, m_selectMaxY);

                            pLevel->GetLevelSelection().CreateSelection(m_selectMinX, m_selectMinY, m_selectMaxX, m_selectMaxY);
                            pLevel->GetMouseHoverCell(m_selectionCellX, m_selectionCellY);

                            m_bSelectedTerrain = true;
                        }
                    }
                }
            }
        }
        //else 
        {
            if(m_clickHandler != CLICK_HANDLER_EDITTERRAIN  && Event.btn == 1 && Event.event == KEY_STATUS_DOWN && m_bFreeLook)
            {
                CCamera* pCam = gEngine->GetWorldMgr()->GetDefaultCamera();

                CQuaternion quat = pCam->GetAttachedNode()->GetOrientation();
                quat.AddYaw((float)gEngine->GetInput()->GetMouseDX() * gEngine->GetTimeElapsed() * 0.2f);
                quat.AddPitch((float)gEngine->GetInput()->GetMouseDY() * gEngine->GetTimeElapsed() * 0.2f);
                pCam->GetAttachedNode()->SetOrientation(quat);
            }
        }


        if(Event.btn == 1 && Event.event == KEY_STATUS_DOWN)
            m_rightBtnDownTime += gEngine->GetTimeElapsed();
        else
            m_rightBtnDownTime = 0.0f;
        

    }
} // env