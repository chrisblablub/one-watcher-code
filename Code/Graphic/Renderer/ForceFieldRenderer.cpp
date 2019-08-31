#include <Graphic/Renderer/ForceFieldRenderer.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CForceFieldRenderer::CForceFieldRenderer(CRenderDevice* pDevice)
        : m_pDevice(pDevice)
        , m_pShader(nullptr)
        , m_pForceField(nullptr)
        , m_pWaterSpecularityMap(nullptr)
        , m_pWaterNormalMap(nullptr)
        , m_pVB(nullptr)
        , m_time(0.0f) {}

    //-----------------------------------------------------------------------------------
    CForceFieldRenderer::~CForceFieldRenderer()
    {
        if (m_pShader)
            gShaderMgr->Release(m_pShader->GetName());

        if (m_pWaterSpecularityMap)
            gTextureMgr->Release(m_pWaterSpecularityMap->GetName());

        if (m_pForceField)
            gTextureMgr->Release(m_pForceField->GetName());

        if (m_pWaterNormalMap)
            gTextureMgr->Release(m_pWaterNormalMap->GetName());

        ENV_DELETE(m_pVB);
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldRenderer::PreRender(RenderQueue& renderQueue, RenderSet& renderSet)
    {

    }

    //-----------------------------------------------------------------------------------
    void CForceFieldRenderer::Render(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        bool bUpdate = true;

        m_time += renderQueue.timeElapsedFrame;

        if (bUpdate)
            ENV_DELETE(m_pVB);

        if (!m_pVB)
        {
            InitBuffer(renderQueue);
        }

        m_constBufForcefieldVS.GetDataForWriting() = renderQueue.m_pCameraRenderObject->m_mtxViewProjection;

        

        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ALPHABLENDENABLE, true);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_BLENDOP, BLENDOP_TYPE_ADD);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_SRCBLEND, BLEND_TYPE_SRC_ALPHA);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_DESTBLEND, BLEND_TYPE_INV_SRC_ALPHA);
        m_pDevice->SetTextureSampler(0, m_pWaterSpecularityMap->GetTextureSampler());
        m_pDevice->SetTexture(0, m_pWaterSpecularityMap);
        m_pDevice->SetTextureSampler(2, m_pWaterNormalMap->GetTextureSampler());
        m_pDevice->SetTexture(2, m_pWaterNormalMap);
        m_pDevice->SetTextureSampler(3, m_pForceField->GetTextureSampler());
        m_pDevice->SetTexture(3, m_pForceField);
        m_pDevice->SetVertexDeclaration(m_pVertexDeclaration.get());
        m_pDevice->SetVertexBuffer(0, 0, m_pVB);
        m_pDevice->SetShader(m_pShader);

        int i = 0;
        for (RenderQueue::listForceFields::iterator it = renderQueue.m_forceFields.begin(); it != renderQueue.m_forceFields.end(); ++it, ++i)
        {
            ForceFieldRenderObject* ro = *it;

            Vec4 timeDir(m_time * 0.05f, m_time * 0.2f, m_time * 0.2f, m_time * 0.8f);
            Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), -ro->m_orientation.GetRoll(), Vec3(0.0f, 0.0f, 1.0f)));
            Vec3 v(timeDir.x, timeDir.y, 0.0f);
            v = Vec3(Vec4(v, 1.0f) * mtxRot);

            timeDir.x = v.x;
            timeDir.y = v.y;

            Vec4 rot2x2;
            rot2x2.x = mtxRot[0][0];
            rot2x2.y = mtxRot[0][1];
            rot2x2.z = mtxRot[1][0];
            rot2x2.w = mtxRot[1][1];

            m_constBufForcefieldPS.GetDataForWriting().m_timeDirection = timeDir;
            m_constBufForcefieldPS.GetDataForWriting().m_direction = Vec2(ro->m_up);
            m_constBufForcefieldPS.GetDataForWriting().m_rotMatrix = rot2x2;
            m_constBufForcefieldPS.GetDataForWriting().m_color = ro->m_color;
            
            m_pDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, i * 6, 2);
        }


        for (int i = 0; i < 4; ++i)
            m_pDevice->SetTexture(i, NULL);
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldRenderer::PostRender(RenderQueue& renderQueue, RenderSet& renderSet)
    {

    }

    //-----------------------------------------------------------------------------------
    void CForceFieldRenderer::InitBuffer(RenderQueue& renderQueue)
    {
        ENV_DELETE(m_pVB);

        if (renderQueue.m_forceFields.empty())
            return;

        m_pVB = m_pDevice->CreateVertexBuffer(6 * renderQueue.m_forceFields.size() * m_pVertexDeclaration->GetStride(), USAGE_TYPE_VERTEXINDEXBUFFERS);

        TerrainVertex* p = (TerrainVertex*)m_pVB->Lock(0, 0, 0);

        for (RenderQueue::listForceFields::iterator it = renderQueue.m_forceFields.begin(); it != renderQueue.m_forceFields.end(); ++it)
        {
            ForceFieldRenderObject* pFF = *it;

            Vec3 size(300.0f, 300.0f, 0.0f);


            const Matrix4x4& mtxRST = pFF->m_mtxTransform;
            p->pos = Vec3(-size.x, -size.y, 5.0f);
            p->pos = Vec3(Vec4(p->pos, 1.0f) * mtxRST);
            p->texcoords = Vec2(0.0f, 0.0f); ++p;
            p->pos = Vec3(-size.x, size.y, 5.0f);
            p->pos = Vec3(Vec4(p->pos, 1.0f) * mtxRST);
            p->texcoords = Vec2(0.0f, 1.0f); ++p;
            p->pos = Vec3(size.x, size.y, 5.0f);
            p->pos = Vec3(Vec4(p->pos, 1.0f) * mtxRST);
            p->texcoords = Vec2(1.0f, 1.0f); ++p;
            p->pos = Vec3(size.x, size.y, 5.0f);
            p->pos = Vec3(Vec4(p->pos, 1.0f) * mtxRST);
            p->texcoords = Vec2(1.0f, 1.0f); ++p;
            p->pos = Vec3(size.x, -size.y, 5.0f);
            p->pos = Vec3(Vec4(p->pos, 1.0f) * mtxRST);
            p->texcoords = Vec2(1.0f, 0.0f); ++p;
            p->pos = Vec3(-size.x, -size.y, 5.0f);
            p->pos = Vec3(Vec4(p->pos, 1.0f) * mtxRST);
            p->texcoords = Vec2(0.0f, 0.0f); ++p;
        }

        m_pVB->Unlock();
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldRenderer::OnLevelChanged()
    {
        ENV_DELETE(m_pVB);
    }

    //-----------------------------------------------------------------------------------
    void CForceFieldRenderer::OnLostDevice()
    {
        CBaseRenderer::OnLostDevice();

    }

    //-----------------------------------------------------------------------------------
    void CForceFieldRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        CBaseRenderer::OnResetDevice(screenWidth, screenHeight);

        m_constBufForcefieldPS.GetDataForWriting().m_invScreenSize = Vec2(1.0f / screenWidth, 1.0f / screenHeight);
    }

    //-----------------------------------------------------------------------------------
    bool CForceFieldRenderer::Initialize()
    {
        m_pShader = gShaderMgr->AddGetPtr("ForceField");

        if (!m_pShader)
            return false;

        m_pShader->LinkConstantBufferCPU("cb_ForcefieldVS", m_constBufForcefieldVS);
        m_pShader->LinkConstantBufferCPU("cb_ForcefieldPS", m_constBufForcefieldPS);

        m_pVertexDeclaration = m_pDevice->CreateVertexDeclaration(TerrainVertex::VertexElements, sizeof(TerrainVertex), m_pShader);

        if (!m_pVertexDeclaration)
            return false;

        m_pWaterSpecularityMap  = gTextureMgr->AddGetPtr("water_specular01.png");
        m_pWaterNormalMap       = gTextureMgr->AddGetPtr("water_nmap01.png");
        m_pForceField           = gTextureMgr->AddGetPtr("forcefield.png");

        if (!m_pWaterSpecularityMap ||
            !m_pWaterNormalMap ||
            !m_pForceField)
        {
            return false;
        }

        m_pWaterNormalMap->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        m_pWaterSpecularityMap->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());
        m_pForceField->SetTextureSampler(CDefaultTextureSampler::GetLinearMipWrapSampler());

        return true;
    }

    //-----------------------------------------------------------------------------------
//     void CForceFieldRenderer::AddForceField(const std::string& strName, CForceFieldEntity* pForceField)
//     {
//         m_forceFields[strName] = pForceField;
//     }
// 
//     //-----------------------------------------------------------------------------------
//     void CForceFieldRenderer::RemoveForceField(const std::string& strName)
//     {
//         mapForceFields::iterator it = m_forceFields.find(strName);
//         if (it != m_forceFields.end())
//             m_forceFields.erase(it);
//     }
} // env