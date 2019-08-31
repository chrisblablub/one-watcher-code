#include <Graphic/Renderer/CharacterRenderer.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Renderer/ParticleSystemRenderer.h>
#include <Graphic/Renderer/SpriteRenderer.h>
#include <Graphic/Renderer/FullscreenTextureRenderer.h>
#include <Graphic/RenderSystems/RenderSystem.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CCharacterRenderer::CCharacterRenderer(CRenderDevice* pDevice,
                                           CSpriteRenderer* pSpriteRenderer)
        : m_pDevice(pDevice)
        , m_pSpriteRenderer(pSpriteRenderer)
        , m_scaling(3.5f, 3.5f, 0.003f, 0.003f)
        , m_currentStandingDistortionTime(-1.0f)
        , m_charPrevVelocity(0.0f, 0.0f)
        , m_currentEnergyDir(0.0f, 0.01f)
        , m_pVBBackground(nullptr)
        , m_pCharacterShader(nullptr)
        , m_pBackgroundShader(nullptr)
        , m_pWaterNormalMap(nullptr)
        , m_pCharacterRT(nullptr)
        , m_pCharacterVertexDecl(nullptr)
        , m_pTextureBackground(nullptr)
        , m_time(0.0f)
        , m_charPrevPos(0.0f, 0.0f, 0.0f) {}

    //-----------------------------------------------------------------------------------
    CCharacterRenderer::~CCharacterRenderer()
    {
        ENV_DELETE(m_pVBBackground);

        gTextureMgr->Release(m_pTextureBackground);
        gTextureMgr->Release(m_pWaterNormalMap);

        gShaderMgr->Release(m_pBackgroundShader);
        gShaderMgr->Release(m_pCharacterShader);
    }

    //-----------------------------------------------------------------------------------
    bool CCharacterRenderer::Initialize()
    {
        screenQuad.Initialize(m_pDevice);

        const RenderQualityParams& params = gRenderSystem->GetRenderQualityParams();

        m_pCharacterRT = m_pDevice->CreateRenderTarget(params.screenWidth, params.screenHeight, TEXTUREFORMAT_TYPE_A16B16G16R16F, true);

        m_pWaterNormalMap = gTextureMgr->AddGetPtr("water_nmap01.png");
        m_pTextureBackground = gTextureMgr->AddGetPtr("background_cell.png");

        if (!m_pWaterNormalMap)
            return false;

        m_pCharacterRT->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pWaterNormalMap->SetTextureSampler(CDefaultTextureSampler::GetLinearWrapSampler());
        m_pWaterNormalMap->SetSRGB(false);


        m_pCharacterShader = gShaderMgr->AddGetPtr("Character");
        m_pCharacterShader->LinkConstantBufferCPU("cb_CharacterVS", m_constBufCharVS);
        m_pCharacterShader->LinkConstantBufferCPU("cb_CharacterPS", m_constBufCharPS);

        m_pCharacterVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pCharacterShader);

        m_currentScaling = m_scaling;
        m_currentOffsetOpacity = Vec2(0.008f, 0.0f);

        //         m_pCharBackgroundShader = gShaderMgr->AddGetPtr("CharacterBackground");
        //         m_pCharBackgroundShader->SetUniform("gCamInvPMatrix", &m_uniformCamInvPMatrix);
        //         m_pCharBackgroundShader->SetUniform("gCamInvVMatrix", &m_uniformCamInvVMatrix);
        // 
        //         m_screenQuad.Initialize(gRenderSystem->GetRenderDevice());
        //         m_pCharacterBackgroundVertexDecl = m_pDevice->CreateVertexDeclaration(VertexPT::VertexElements, sizeof(VertexPT), m_pCharBackgroundShader);



        m_pBackgroundShader = gShaderMgr->AddGetPtr("LevelBackground");
        m_pBackgroundShader->LinkConstantBufferCPU("cb_LevelBackgroundVS", m_constBufCellsVS);

        //m_pBackgroundShader->SetUniform("gVP", &m_uniformVP);
        //m_pBackgroundShader->SetUniform("gCharPos", &m_uniformCharPos);
        //m_pBackgroundShader->SetUniform("gPosition", &m_uniformPosition);
        //m_pBackgroundShader->SetUniform("gAlpha", &m_uniformAlpha);
        //m_pBackgroundShader->SetUniform("gTexCoordOffset", &m_uniformTexCoordOffset);
        //m_pBackgroundShader->SetUniform("gInvScreenSize", &m_uniformInvScreenSize);

        m_pVBBackground = m_pDevice->CreateVertexBuffer(6 * sizeof(PosTexVertex), USAGE_TYPE_VERTEXINDEXBUFFERS);

        PosTexVertex* pLvl = static_cast<PosTexVertex*>(m_pVBBackground->Lock(0, 0, 0));

        pLvl->p = Vec3(-CELL_EVEN_SIZE_X, -CELL_EVEN_SIZE_Y, 0.0f);
        pLvl->t = Vec2(0.0f, 0.0f);
        ++pLvl;

        pLvl->p = Vec3(-CELL_EVEN_SIZE_X, CELL_EVEN_SIZE_Y, 0.0f);
        pLvl->t = Vec2(0.0f, 0.5f);
        ++pLvl;

        pLvl->p = Vec3(CELL_EVEN_SIZE_X, CELL_EVEN_SIZE_Y, 0.0f);
        pLvl->t = Vec2(1.0f, 0.5f);
        ++pLvl;

        pLvl->p = Vec3(CELL_EVEN_SIZE_X, CELL_EVEN_SIZE_Y, 0.0f);
        pLvl->t = Vec2(1.0f, 0.5f);
        ++pLvl;

        pLvl->p = Vec3(CELL_EVEN_SIZE_X, -CELL_EVEN_SIZE_Y, 0.0f);
        pLvl->t = Vec2(1.0f, 0.0f);
        ++pLvl;

        pLvl->p = Vec3(-CELL_EVEN_SIZE_X, -CELL_EVEN_SIZE_Y, 0.0f);
        pLvl->t = Vec2(0.0f, 0.0f);
        ++pLvl;

        m_pVBBackground->Unlock();

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::RenderBackground(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        //         m_uniformCamInvPMatrix.SetData(renderQueue.m_pCameraRenderObject->m_mtxInverseProjection);
        //         m_uniformCamInvVMatrix.SetData(renderQueue.m_pCameraRenderObject->m_mtxInverseView);
        // 
        //         CTextureManaged* pTex = gTextureMgr->AddGetPtr("background.png");
        //         CTextureManaged* pNormal = gTextureMgr->AddGetPtr("background_nmap.png");
        //         pNormal->SetSRGB(false);
        // 
        //         m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
        //         m_pDevice->ResetToDefaultRenderStates();
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        //         m_pDevice->SetTextureSampler(0, renderSet.m_pCurrentColorSource->GetTextureSampler());
        //         m_pDevice->SetTexture(0, renderSet.m_pCurrentColorSource->GetTexture());
        //         m_pDevice->SetTextureSampler(1, renderSet.m_pRenderTargetDepths->GetTextureSampler());
        //         m_pDevice->SetTexture(1, renderSet.m_pRenderTargetDepths->GetTexture());
        //         m_pDevice->SetTexture(2, pTex);
        //         m_pDevice->SetTextureSampler(2, CDefaultTextureSampler::GetLinearWrapSampler());
        //         m_pDevice->SetTexture(3, pNormal);
        //         m_pDevice->SetTextureSampler(3, CDefaultTextureSampler::GetLinearWrapSampler());
        // 
        //         
        // 
        //         m_pDevice->SetVertexDeclaration(m_pCharacterBackgroundVertexDecl);
        //         m_pDevice->SetShader(m_pCharBackgroundShader);
        //         m_screenQuad.RenderQuad();
        //         m_pDevice->SetTexture(0, NULL);
        //         m_pDevice->SetTexture(1, NULL);
    }

    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::OnLevelChanged()
    {
    }

    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::RenderCells(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_time += gEngine->GetTimeElapsed();

        if (!renderQueue.m_pCharacter3DRenderObject ||
            !renderQueue.m_pCharacter3DRenderObject->m_bRenderCells)
        {
            return;
        }

        float CELL_LIEFTIME = 1.0f;
        float CELL_FADE = 0.25f;


        Vec3 charPos = renderQueue.m_pCharacter3DRenderObject->m_position;

        Vec3 velo = charPos - m_charPrevPos;

        float speed = glm::length(velo);
        speed /= 3.0f;
        speed = std::max(speed, 1.0f);
        CELL_LIEFTIME = CELL_LIEFTIME / speed;
        CELL_FADE = CELL_LIEFTIME * 0.25f;

        m_charPrevPos = charPos;

        charPos = (charPos + Vec3(-16.0f, -16.0, 0.0f));


        int closesCellY = charPos.y / (CELL_EVEN_DISTANCE_Y);
        int closesCellX = charPos.x / (CELL_EVEN_DISTANCE_X);

        int min_range_x = 0;
        int max_range_x = 1;
        int min_range_y = 1;
        int max_range_y = 1;

        if (abs(velo.y) > 0)
        {
            if (velo.y < -1)
                min_range_y += 1;

            if (velo.y > 1)
                max_range_y += 1;
        }

        if (abs(velo.x) > 0)
        {
            if (velo.x < -1)
            {
                min_range_x += 1;
                //max_range_x -= 1;
            }

            if (velo.x > 1)
            {
                max_range_x += 1;
                //min_range_x -= 1;
            }
        }

        // Update existing cells
        std::list< Cell >::iterator it = m_evenCells.begin();
        while (it != m_evenCells.end())
        {
            Cell& c = *it;

            if ((c.emitTime + c.fadeTime + c.lifeTime * 0.3f) < m_time && !c.bFadeOut &&
                (glm::length(velo) < 0.1))
            {
                c.emitTime = m_time;
                c.fadeTime = 0.4f;
                c.bFadeOut = true;
                c.bFadeIn = false;
            }
            else if ((c.cellX < (closesCellX - min_range_x) ||
                      c.cellX >(closesCellX + max_range_x) ||
                      c.cellY < (closesCellY - min_range_y) ||
                      c.cellY >(closesCellY + max_range_y)) &&
                      (c.emitTime + c.fadeTime + c.lifeTime) < m_time && !c.bFadeOut)
            {
                c.emitTime = m_time;
                c.fadeTime = 0.4f;
                c.bFadeOut = true;
                c.bFadeIn = false;
            }

            if (c.emitTime + c.fadeTime < m_time && c.bFadeOut)
            {
                m_evenCells.erase(it++);
            }
            else
            {
                if (c.bFadeIn)
                {
                    if (c.emitTime + c.fadeTime >= m_time)
                        c.alpha = 1.0f - ((c.emitTime + c.fadeTime) - m_time) / c.fadeTime;
                    else
                    {
                        c.bFadeIn = false;
                    }
                }
                else if (c.bFadeOut)
                {
                    c.alpha = ((c.emitTime + c.fadeTime) - m_time) / c.fadeTime;
                }
                else
                {
                    c.alpha = 1.0f;
                }

                ++it;
            }
        }




        // Emit new cells
        if (glm::length(velo) > 0)
        {
             for (int i = closesCellX - min_range_x; i <= closesCellX + max_range_x; ++i)
            {
                for (int j = closesCellY - min_range_y; j <= closesCellY + max_range_y; ++j)
                {
                    bool bEvenCellFound = false;
                    bool bOddCellFound = false;
                    for (std::list< Cell >::iterator it = m_evenCells.begin(); it != m_evenCells.end(); ++it)
                    {
                        Cell& c = *it;
                        if (c.cellX == i && c.cellY == j)
                        {
                            if (!bEvenCellFound && c.bEven)
                                bEvenCellFound = true;
                            if (!bOddCellFound && !c.bEven)
                                bOddCellFound = true;

                        }
                    }



                    float r = GetRandomFloat(0.0f, 1.0f);
                    float lifeTime = GetRandomFloat(CELL_LIEFTIME * 0.5, CELL_LIEFTIME * 2);
                    float r2 = GetRandomFloat(0.0f, 1.0f);

                    if (!bEvenCellFound && r > 0.8f)
                        m_evenCells.push_back(Cell(i, j, m_time, lifeTime * 1, lifeTime * 0.4f, true, (int)(bool)(r2 > 0.8f)));
                    //else
                    if (!bOddCellFound && r > 0.8f)
                        m_evenCells.push_back(Cell(i, j, m_time, lifeTime * 1, lifeTime * 0.4f, false, (int)(bool)(r2 > 0.8f)));
                }

            }
        }

//         m_uniformInvScreenSize.SetData(VECTOR2(1.0f / gRenderSystem->GetRenderQualityParams().screenWidth,
//                                                1.0f / gRenderSystem->GetRenderQualityParams().screenHeight));
// 
//         CBaseRenderDevice* pDevice = gRenderSystem->GetRenderDevice();

        
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
//        pDevice->SetRenderState(RENDERSTATE_TYPE_SEPARATEALPHABLENDENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetSamplerState(0, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        m_pDevice->SetSamplerState(5, SAMPLERSTATE_TYPE_SRGBTEXTURE, true);
        m_pDevice->SetShader(m_pBackgroundShader);
        m_pDevice->SetVertexBuffer(0, 0, m_pVBBackground);
        m_pDevice->SetVertexDeclaration(m_pCharacterVertexDecl.get());
        m_pDevice->SetTexture(0, m_pTextureBackground);
        //m_pTextureBackground->Bind(5);


        float maxDepth = 350.0f; //gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetLayerMaxDepth();

        m_constBufCellsVS.GetDataForWriting().m_viewProj = renderQueue.m_pCameraRenderObject->m_mtxViewProjection;
        
        for (std::list< Cell >::iterator it = m_evenCells.begin(); it != m_evenCells.end(); ++it)
        {
            Cell& c = *it;
//             m_uniformAlpha.SetData(VECTOR2(c.alpha, c.glowAlpha));
            
            m_constBufCellsVS.GetDataForWriting().m_alpha = Vec4(renderQueue.m_pEnvironmentRenderObject->m_fogColor * 2.0f, c.alpha);

            if (c.bEven)
                m_constBufCellsVS.GetDataForWriting().m_position = Vec4(c.cellX * CELL_EVEN_DISTANCE_X, c.cellY * CELL_EVEN_DISTANCE_Y, maxDepth + 5.0f, 0);
            else
                m_constBufCellsVS.GetDataForWriting().m_position = Vec4(c.cellX * CELL_EVEN_DISTANCE_X + CELL_EVEN_SIZE_X + 0.5f * CELL_OFFSET_X, c.cellY * CELL_EVEN_DISTANCE_Y + CELL_EVEN_SIZE_Y, maxDepth + 5.0f, 0);
// 
//             m_uniformTexCoordOffset.SetData(VECTOR2(0.0f, 0.5f) * c.sequence);
//             m_uniformCharPos.SetData(renderQueue.characterPos);
//             m_uniformVP.SetData(renderQueue.cameraState.m_mtxViewProjection);

             
            m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 2);
        }

        m_pDevice->SetSamplerState(5, SAMPLERSTATE_TYPE_SRGBTEXTURE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);
    }


    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::Render(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (!renderQueue.m_pCharacterRenderObject)
            return;

        UpdateRenderTargetSize(renderQueue);

        Vec4 projCharPos(renderQueue.m_pCharacterRenderObject->m_position.x,
                         renderQueue.m_pCharacterRenderObject->m_position.y,
                         renderQueue.m_pCharacterRenderObject->m_position.z, 1.0);

        projCharPos = projCharPos * renderQueue.m_pCameraRenderObject->m_mtxViewProjection;

        Vec2 screenCharacterPos(projCharPos.x / projCharPos.w * 0.5f + 0.5f,
                                -projCharPos.y / projCharPos.w * 0.5f + 0.5f);


        m_constBufCharPS.GetDataForWriting().x = renderQueue.timeElapsed * 4.0f * 0.01f;

        CRenderDevice* m_pDevice = gRenderSystem->GetRenderDevice();

        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_ONE);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        //         m_pDevice->SetRenderTarget(0, renderSet.m_pRenderTargetPostProcess02);
        //         m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
        // 
        //         m_pSpriteRenderer->Render(renderQueue.m_pCharacterRenderObject->m_sprites, renderQueue, renderSet, true, true, true);
        // 
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        //         m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);

                // Render character super particle system

        float l, r, t, b;
        ComputeCharacterFrustum(renderQueue, l, r, t, b);

        Matrix4x4 mtxCharProj(1.0f);
        ComputeProjectionMatrix(mtxCharProj, l, r, t, b, renderQueue.m_pCameraRenderObject->m_near, renderQueue.m_pCameraRenderObject->m_far);

        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderTarget(0, m_pCharacterRT.get());
        m_pDevice->SetDepthStencilTarget(m_pCharacterRT.get());
        m_pDevice->SetViewport(Vec2(0.0f, 0.0f), m_pCharacterRT->GetWidth(), m_pCharacterRT->GetHeight());
        m_pDevice->Clear(Vec4(0, 0, 0, 1), CLEAR_TYPE_TARGET | CLEAR_TYPE_ZBUFFER);

        CParticleSystemColorRenderer* pRenderer = static_cast<CParticleSystemColorRenderer*>(gRenderSystem->GetRenderer("ParticleColorRenderer"));
        pRenderer->Render(renderQueue.m_particleSystems["ParticleCharacterRenderer"], renderQueue.m_pCameraRenderObject->m_mtxView * mtxCharProj);

        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_ONE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_ONE);

        m_pDevice->SetRenderTarget(0, renderSet.m_pRenderTargetPostProcess02);
        m_pDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
        m_pDevice->SetTextureSampler(0, m_pCharacterRT->GetTextureSampler());
        m_pDevice->SetTexture(0, m_pCharacterRT->GetTexture());
        m_pDevice->SetTextureSampler(1, m_pWaterNormalMap->GetTextureSampler());
        m_pDevice->SetTexture(1, m_pWaterNormalMap);

        UpdateQuadTransform(renderQueue, screenCharacterPos);


        m_pDevice->SetShader(m_pCharacterShader);
        m_pDevice->SetVertexDeclaration(m_pCharacterVertexDecl.get());

        screenQuad.RenderQuad();

        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_ONE);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
    }


    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::ComputeCharacterFrustum(RenderQueue& renderQueue, float& l, float& r, float& t, float& b)
    {
        // Render character super particle system
        Vec3 charLeft = renderQueue.m_pCharacterRenderObject->m_position + Vec3(-60.0f, 0.0f, 0.0f);
        Vec3 charRight = renderQueue.m_pCharacterRenderObject->m_position + Vec3(60.0f, 0.0f, 0.0f);
        Vec3 charTop = renderQueue.m_pCharacterRenderObject->m_position + Vec3(0.0f, 60.0f, 0.0f);
        Vec3 charBottom = renderQueue.m_pCharacterRenderObject->m_position + Vec3(0.0f, -60.0f, 0.0f);

        charLeft = Vec3(Vec4(charLeft, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxView);
        charRight = Vec3(Vec4(charRight, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxView);
        charTop = Vec3(Vec4(charTop, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxView);
        charBottom = Vec3(Vec4(charBottom, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxView);

        l = charLeft.x / charLeft.z * renderQueue.m_pCameraRenderObject->m_near;
        r = charRight.x / charRight.z * renderQueue.m_pCameraRenderObject->m_near;
        t = charTop.y / charTop.z * renderQueue.m_pCameraRenderObject->m_near;
        b = charBottom.y / charBottom.z * renderQueue.m_pCameraRenderObject->m_near;
    }

    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::UpdateQuadTransform(RenderQueue& renderQueue, const Vec2& charScreenPos)
    {
        /*float l, r, t, b;

        // Compute character bounding box on the near plane
        ComputeCharacterFrustum(renderQueue, l, r, t, b);

        // Map character bounding box to [-1, 1] range
        RenderCoordRect renderCoords;
        renderCoords.m_left = -l / renderQueue.m_pCameraRenderObject->m_l;
        renderCoords.m_right = r / renderQueue.m_pCameraRenderObject->m_r;
        renderCoords.m_top = t / renderQueue.m_pCameraRenderObject->m_t;
        renderCoords.m_bottom = -b / renderQueue.m_pCameraRenderObject->m_b;

        // Compute quad scalings based on the horizontal and vertical extent of the character bounding box
        float horizontalScaling = fabs(renderCoords.m_right - renderCoords.m_left) / 2.0f;
        float verticalScaling = fabs(renderCoords.m_top - renderCoords.m_bottom) / 2.0f;

        VECTOR2 pixelsToTexels = CQuad::GetPixelsToTexelsOffset(m_pCharacterRT->GetWidth(), m_pCharacterRT->GetHeight());

        VECTOR4 quadTransform(horizontalScaling, verticalScaling, charScreenPos * VECTOR2(2.0f, -2.0f) + VECTOR2(-1.0f, 1.0f) + pixelsToTexels.x);*/

        Matrix4x4 scaling = glm::scale(Matrix4x4(1.0f), Vec3(60.0f, 60.0f, 1.0f));
        Matrix4x4 translation = glm::transpose(glm::translate(Matrix4x4(1.0f), renderQueue.m_pCharacterRenderObject->m_position));

        m_constBufCharVS.GetDataForWriting() = scaling * translation * renderQueue.m_pCameraRenderObject->m_mtxViewProjection;
    }

    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::UpdateRenderTargetSize(RenderQueue& renderQueue)
    {
        float l, r, t, b;

        // Compute character bounding box on the near plane
        ComputeCharacterFrustum(renderQueue, l, r, t, b);

        // Map character bounding box to [-1, 1] range
        RenderCoordRect renderCoords;
        renderCoords.m_left = -l / renderQueue.m_pCameraRenderObject->m_l;
        renderCoords.m_right = r / renderQueue.m_pCameraRenderObject->m_r;
        renderCoords.m_top = t / renderQueue.m_pCameraRenderObject->m_t;
        renderCoords.m_bottom = -b / renderQueue.m_pCameraRenderObject->m_b;

        renderCoords.m_left = renderCoords.m_left * 0.5f + 0.5f;
        renderCoords.m_right = renderCoords.m_right * 0.5f + 0.5f;
        renderCoords.m_top = renderCoords.m_top * 0.5f + 0.5f;
        renderCoords.m_bottom = renderCoords.m_bottom * 0.5f + 0.5f;

        Vec2 size(renderCoords.m_right - renderCoords.m_left, renderCoords.m_top - renderCoords.m_bottom);

        const RenderQualityParams& params = gRenderSystem->GetRenderQualityParams();

        int textureSizeX = (int)(fabs(size.x) * (float)params.screenWidth);
        int textureSizeY = (int)(fabs(size.y) * (float)params.screenHeight);

        if ((m_pCharacterRT->GetWidth() != textureSizeX || m_pCharacterRT->GetHeight() != textureSizeY) &&
            textureSizeX > 0 && textureSizeY > 0)
        {
            textureSizeX = std::min(textureSizeX, 1024);
            textureSizeY = std::min(textureSizeY, 1024);

            m_pCharacterRT->SetSize(textureSizeX, textureSizeY);
            m_pCharacterRT->ApplyChanges();
        }

    }

    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::OnLostDevice()
    {
        m_pCharacterRT->OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CCharacterRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_pCharacterRT->SetSize((int)screenWidth, (int)screenHeight);
        m_pCharacterRT->OnResetDevice();
    }
} // env