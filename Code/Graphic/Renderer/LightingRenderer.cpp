#include <Graphic/Renderer/LightingRenderer.h>
#include <Graphic/Renderer/GeometryRenderer.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderTarget.h>
#include <Graphic/Core/Texture.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/MeshContainer.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CLightingRenderer::CLightingRenderer(CRenderDevice* pDevice)
        : m_pDevice(pDevice)
        , m_pShaderLightingOmni(nullptr)
        , m_pShaderLightingDirectional(nullptr)
        , m_pShaderLightingReflection(nullptr)
        , m_pShaderLightingEmissive(nullptr)
        , m_pShaderLightingUnlit(nullptr)
        , m_pSphere(nullptr) {}

    //-----------------------------------------------------------------------------------
    CLightingRenderer::~CLightingRenderer()
    {
        ENV_DELETE(m_pSphere);

        gShaderMgr->Release(m_pShaderLightingReflection);
        gShaderMgr->Release(m_pShaderLightingUnlit);
        gShaderMgr->Release(m_pShaderLightingOmni);
        gShaderMgr->Release(m_pShaderLightingEmissive);
        gShaderMgr->Release(m_pShaderLightingDirectional);
    }

    //-----------------------------------------------------------------------------------
    bool CLightingRenderer::Initialize()
    {
        m_renderQuad.Initialize(m_pDevice);

        m_pShaderLightingUnlit = gShaderMgr->AddGetPtr("LightingUnlit");
        m_pShaderLightingOmni = gShaderMgr->AddGetPtr("LightingOmni");
        m_pShaderLightingDirectional = gShaderMgr->AddGetPtr("LightingDirectional");
        m_pShaderLightingReflection = gShaderMgr->AddGetPtr("LightingReflection");
        m_pShaderLightingEmissive = gShaderMgr->AddGetPtr("LightingEmissive");

        if (!m_pShaderLightingUnlit ||
            !m_pShaderLightingOmni ||
            !m_pShaderLightingDirectional ||
            !m_pShaderLightingReflection ||
            !m_pShaderLightingEmissive)
        {
            return false;
        }

        m_pShaderLightingDirectional->LinkConstantBufferCPU("cb_LightingDirectionalVS", m_constBufDirectionalVS);
        m_pShaderLightingDirectional->LinkConstantBufferCPU("cb_LightingDirectionalPS", m_constBufDirectionalPS);

        m_pShaderLightingEmissive->LinkConstantBufferCPU("cb_LightingEmissiveVS", m_constBufEmissiveVS);
        m_pShaderLightingEmissive->LinkConstantBufferCPU("cb_LightingEmissivePS", m_constBufEmissivePS);

        m_pShaderLightingOmni->LinkConstantBufferCPU("cb_LightingOmniVS", m_constBufOmniVS);
        m_pShaderLightingOmni->LinkConstantBufferCPU("cb_LightingOmniPS", m_constBufOmniPS);

        m_pShaderLightingReflection->LinkConstantBufferCPU("cb_LightingReflectionVS", m_constBufReflectionVS);
        m_pShaderLightingReflection->LinkConstantBufferCPU("cb_LightingReflectionPS", m_constBufReflectionPS);

        m_pShaderLightingUnlit->LinkConstantBufferCPU("cb_LightingUnlitVS", m_constBufUnlitVS);
        m_pShaderLightingUnlit->LinkConstantBufferCPU("cb_LightingUnlitPS", m_constBufUnlitPS);

        m_pSphere = new CMeshContainer();
        m_pSphere->InitResource("../Resources/Meshes/Sphere.x", "Sphere.x", 0);

        m_pUnlitVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pShaderLightingUnlit);
        m_pOmniVertexDecl = m_pDevice->CreateVertexDeclaration(PosNormalMapTexVertex::VertexElements, sizeof(PosNormalMapTexVertex), m_pShaderLightingOmni);
        m_pDirectionalVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pShaderLightingDirectional);

        mapSubMeshes& meshes = m_pSphere->GetMeshes();
        m_pSphereMesh = meshes.begin()->second;

        return true;
    }


    //-----------------------------------------------------------------------------------
    void CLightingRenderer::Render(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        renderQueue.m_lightRenderObjects.sort(LightRenderObject::cmp);

        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_LESS);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_KEEP);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_KEEP);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILZFAIL, STENCILOP_TYPE_KEEP);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILWRITEMASK, 0x0F);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREADMASK, 0x0F);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        m_pDevice->SetRenderTarget(0, renderSet.m_pRenderTargetPostProcess01);
        m_pDevice->SetTextureSampler(0, renderSet.m_pRenderTargetColors->GetTextureSampler());
        m_pDevice->SetTexture(0, renderSet.m_pRenderTargetColors->GetTexture());
        m_pDevice->SetTextureSampler(1, renderSet.m_pRenderTargetNormals->GetTextureSampler());
        m_pDevice->SetTexture(1, renderSet.m_pRenderTargetNormals->GetTexture());
        m_pDevice->SetTextureSampler(3, renderSet.m_pRenderTargetDepths->GetTextureSampler());
        m_pDevice->SetTexture(3, renderSet.m_pRenderTargetDepths->GetTexture());


        m_constBufDirectionalVS.GetDataForWriting() = renderQueue.m_pCameraRenderObject->m_mtxInverseProjection;


        Matrix4x4 camInvViewTransMatrix(renderQueue.m_pCameraRenderObject->m_mtxInverseView);
        camInvViewTransMatrix = glm::transpose(camInvViewTransMatrix);

        if (renderQueue.m_lightRenderObjects.empty() ||
            renderQueue.lightingMode == RenderQueue::LIGHTING_MODE_OFF ||
            renderQueue.lightingMode == RenderQueue::LIGHTING_MODE_PRECOMPUTEDONLY)
        {
            m_pDevice->SetVertexDeclaration(m_pUnlitVertexDecl.get());
            m_pDevice->SetShader(m_pShaderLightingUnlit);
            m_renderQuad.RenderQuad();
        }
        else
        {
            m_constBufUnlitVS.GetDataForWriting() = renderQueue.m_pCameraRenderObject->m_mtxInverseProjection;
            m_constBufUnlitPS.GetDataForWriting().x = 1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenWidth;
            m_constBufUnlitPS.GetDataForWriting().y = 1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenHeight;

            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_EQUAL);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_GEOMETRY_NODIRECTIONAL);
            m_pDevice->SetVertexDeclaration(m_pUnlitVertexDecl.get());
            m_pDevice->SetShader(m_pShaderLightingUnlit);
            m_renderQuad.RenderQuad();

            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_GREATER);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_GEOMETRY_NODIRECTIONAL);

            bool bFirstLight = true;

            listLightRenderObjects::iterator it = renderQueue.m_lightRenderObjects.begin();

            {
                ENV_SCOPED_GPU_PROFILING(LIGHTING_RENDERER, DirectionalLighting);

                m_constBufDirectionalPS.GetDataForWriting().m_invScreenSize = Vec2(1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenWidth,
                                                                         1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenHeight);

                    while (it != renderQueue.m_lightRenderObjects.end() && (*it)->m_type == LT_DIRECTIONAL)
                    {
                        LightRenderObject* pLight = *it;

                        Vec3 lightPos = Vec3(Vec4(pLight->m_position, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxView);
                        Vec3 lightDir = Vec3(Vec4(pLight->m_direction, 0.0f) * camInvViewTransMatrix);

                        m_constBufDirectionalPS.GetDataForWriting().m_diffuse = pLight->m_diffuseColor * pLight->m_energy;
                        m_constBufDirectionalPS.GetDataForWriting().m_specular = pLight->m_specularColor * pLight->m_energy;
                        m_constBufDirectionalPS.GetDataForWriting().m_direction = Vec4(lightDir, 0.0f);
                        m_constBufDirectionalPS.GetDataForWriting().m_bSpecular = pLight->m_bSpecular;

                        m_pDevice->SetVertexDeclaration(m_pDirectionalVertexDecl.get());
                        m_pDevice->SetShader(m_pShaderLightingDirectional);
                        m_renderQuad.RenderQuad();

                        if (bFirstLight)
                        {
                            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
                            m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_ONE);
                            m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_ONE);
                            bFirstLight = false;
                        }

                        ++it;
                    }
            }

            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_LESS);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_TERRAIN);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
            m_pDevice->SetVertexDeclaration(m_pOmniVertexDecl.get());
            m_pDevice->SetVertexBuffer(0, 0, m_pSphereMesh->GetMeshRenderObject()->m_pVertexBuffer);
            m_pDevice->SetIndexBuffer(m_pSphereMesh->GetMeshRenderObject()->m_pIndexBuffer);

            unsigned int numVertices = m_pSphereMesh->GetMeshRenderObject()->m_numVertices;
            unsigned int numFaces = m_pSphereMesh->GetMeshRenderObject()->m_numFaces;

            {
                ENV_SCOPED_GPU_PROFILING(LIGHTING_RENDERER, OmniLighting);

                for (; it != renderQueue.m_lightRenderObjects.end(); ++it)
                {
                    LightRenderObject* pLight = *it;

                    bool bLightPrecomputed = pLight->m_bStatic && !pLight->m_bSpecular;

                    if (bLightPrecomputed && !pLight->m_bAffectsMeshes && renderQueue.lightingMode != RenderQueue::LIGHTING_MODE_DYNAMIC)
                    {
                        // The light has been precomputed (for the terrain) and does not affect any meshes -> skip
                        continue;
                    }

                    if (bLightPrecomputed && renderQueue.lightingMode == RenderQueue::LIGHTING_MODE_DYNAMICONLY)
                    {
                        // Render only dynamic lights -> skip all precomputed lights
                        continue;
                    }
                    else if (bLightPrecomputed && renderQueue.lightingMode != RenderQueue::LIGHTING_MODE_DYNAMIC)
                    {
                        // Render lighting only for fragments that belong not to the terrain, because
                        // that lighting is precomputed
                        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_LESS);
                    }
                    else
                    {
                        // Render lighting completely for meshes as well as terrain
                        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_LESSEQUAL);
                    }

                    Vec3 lightPos(pLight->m_position);
                    Vec3 lightDir(pLight->m_direction);
                    lightPos = Vec3(Vec4(lightPos, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxView);
                    lightDir = Vec3(Vec4(lightDir, 0.0f) * camInvViewTransMatrix);

                    const Vec3& scaling = pLight->m_scale;
                    const Matrix4x4& derivedRST = pLight->m_mtxTransform;

                    Matrix4x4 mtxScaling = glm::scale(Matrix4x4(1.0f), Vec3(pLight->m_radius));
                    Matrix4x4 mtxTransform = m_pSphereMesh->GetMeshRenderObject()->m_mtxTransform * mtxScaling * derivedRST;

                    m_constBufOmniPS.GetDataForWriting().m_position = Vec4(lightPos, 1.0f);
                    m_constBufOmniPS.GetDataForWriting().m_diffuse = pLight->m_diffuseColor * pLight->m_energy;
                    m_constBufOmniPS.GetDataForWriting().m_specular = pLight->m_specularColor * pLight->m_energy;
                    m_constBufOmniPS.GetDataForWriting().m_radius = pLight->m_radius * std::min(scaling.x, std::min(scaling.y, scaling.z));
                    m_constBufOmniPS.GetDataForWriting().m_bSpecular = pLight->m_bSpecular;
                    m_constBufOmniPS.GetDataForWriting().m_invScreenSize = Vec2(1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenWidth,
                                                                      1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenHeight);

                    m_constBufOmniVS.GetDataForWriting().m_view = mtxTransform * renderQueue.m_pCameraRenderObject->m_mtxView;
                    m_constBufOmniVS.GetDataForWriting().m_worldViewProj = mtxTransform * renderQueue.m_pCameraRenderObject->m_mtxViewProjection;

                    m_pDevice->SetShader(m_pShaderLightingOmni);
                    m_pDevice->DrawIndexedPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 0, numVertices, 0, numFaces);


                    if (bFirstLight)
                    {
                        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
                        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_ONE);
                        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_ONE);
                        bFirstLight = false;
                    }

                }
            }
        }

        {
            //             Uncomment this to get global cubemap reflections
            //             // Reflections
            //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
            //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
            //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
            //             m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
            // 
            //             m_constBufReflectionVS.GetData() = renderQueue.m_pCameraRenderObject->m_mtxInverseProjection;
            //             m_constBufReflectionPS.GetData() = Vec2(1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenWidth,
            //                                                      1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenHeight);
            //
            //             m_pShaderLightingReflection->SetConstantBufferCPU(m_constBufReflectionVS);
            //             m_pShaderLightingReflection->SetConstantBufferCPU(m_constBufReflectionPS);
            //             m_pDevice->SetVertexDeclaration(m_pDirectionalVertexDecl);
            //             m_pDevice->SetShader(m_pShaderLightingReflection);
            //             m_renderQuad.RenderQuad();


                        // Emissive
            m_constBufEmissiveVS.GetDataForWriting() = renderQueue.m_pCameraRenderObject->m_mtxInverseProjection;
            m_constBufEmissivePS.GetDataForWriting().x = 1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenWidth;
            m_constBufEmissivePS.GetDataForWriting().y = 1.0f / (float)renderQueue.m_pCameraRenderObject->m_screenHeight;


            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_ONE);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_ONE);
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_LESSEQUAL);

            m_pDevice->SetTexture(0, renderSet.m_pRenderTargetEmissive->GetTexture());
            m_pDevice->SetTextureSampler(0, renderSet.m_pRenderTargetEmissive->GetTextureSampler());

            m_pDevice->SetVertexDeclaration(m_pDirectionalVertexDecl.get());
            m_pDevice->SetShader(m_pShaderLightingEmissive);
            m_renderQuad.RenderQuad();
        }

        m_pDevice->SetTexture(4, NULL);
    }

    //-----------------------------------------------------------------------------------
    void CLightingRenderer::OnLostDevice()
    {
    }

    //-----------------------------------------------------------------------------------
    void CLightingRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        RenderCoordRect renderCoords;

#ifdef D3D9
        Vec2 pixelsToTexels = CGPUQuad::GetPixelsToTexelsOffset(screenWidth, screenHeight);

        renderCoords.m_left += pixelsToTexels.x;
        renderCoords.m_right += pixelsToTexels.x;
        renderCoords.m_top += pixelsToTexels.y;
        renderCoords.m_bottom += pixelsToTexels.y;
#endif // D3D9

        m_renderQuad.SetRenderRect(renderCoords);
        m_renderQuad.UpdateVertexBuffer();
    }


} // env