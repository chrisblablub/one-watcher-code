#include <Graphic/Renderer/RainRenderer.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/Texture.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CRainRenderer::CRainRenderer(CRenderDevice* pDevice)
        : m_pDevice(pDevice)
        , m_pShader(nullptr)
        , m_pRainTexture(nullptr) {}

    //-----------------------------------------------------------------------------------
    CRainRenderer::~CRainRenderer()
    {
        gShaderMgr->Release(m_pShader);

        gTextureMgr->Release(m_pRainTexture);
    }

    //-----------------------------------------------------------------------------------
    bool CRainRenderer::Initialize()
    {
        m_renderQuad.Initialize(m_pDevice);

        m_pShader = gShaderMgr->AddGetPtr("Rain");
//         m_pShader->SetUniform("gCamInvPMatrix", &m_uniformCamInvPMatrix);
//         m_pShader->SetUniform("gCamInvVMatrix", &m_uniformCamInvVMatrix);
//         m_pShader->SetUniform("gCamVPMatrix", &m_uniformCamVPMatrix);
//         m_pShader->SetUniform("gCamPos", &m_uniformCamPos);
//         m_pShader->SetUniform("gRefractionStrength", &m_uniformStrength);
//         m_pShader->SetUniform("gInvScreenSize", &m_uniformInvScreenSize);
//         m_pShader->SetUniform("gRainTexCoords", &m_uniformRainTexCoords);

        m_pRainTexture = gTextureMgr->AddGetPtr("Sprites/rain.dds");

        return m_pShader != NULL;
    }

    //-----------------------------------------------------------------------------------
    void CRainRenderer::Render(CRenderTarget* pSource, CRenderTarget* pNormals, CRenderTarget* pDepth,
                               const Matrix4x4& mtxCamPMatrix, const Matrix4x4& mtxCamVMatrix, const Vec3& camPos, float refractionStrength,
                               const Vec2& invScreenSize,
                               float dt)
    {
//         Matrix4x4 mtxInvPMatrix = glm::inverse(mtxCamPMatrix);
//         Matrix4x4 mtxInvVMatrix = glm::inverse(mtxCamVMatrix);
// 
//         static Vec2 rainTexCoords(0.0f, 0.0f);
//         rainTexCoords.x += dt * 0.3f;
//         rainTexCoords.y -= dt * 4.0f;
// 
//         Vec3 viewCamPos;
//         viewCamPos = Vec3(Vec4(camPos, 1.0f) * mtxCamVMatrix);
// 
//         m_uniformCamVPMatrix.SetData(mtxCamPMatrix);
//         m_uniformCamInvPMatrix.SetData(mtxInvPMatrix);
//         m_uniformCamInvVMatrix.SetData(mtxInvVMatrix);
//         m_uniformCamPos.SetData(viewCamPos);
//         m_uniformStrength.SetData(refractionStrength);
//         m_uniformInvScreenSize.SetData(invScreenSize);
//         m_uniformRainTexCoords.SetData(rainTexCoords);
// 
//         m_pDevice->SetTextureSampler(0, pSource->GetTextureSampler());
//         m_pDevice->SetTexture(0, pSource->GetTexture());
//         m_pDevice->SetTextureSampler(1, pNormals->GetTextureSampler());
//         m_pDevice->SetTexture(1, pNormals->GetTexture());
//         m_pDevice->SetTextureSampler(3, pDepth->GetTextureSampler());
//         m_pDevice->SetTexture(3, pDepth->GetTexture());
// 
//         m_pDevice->SetTexture(4, m_pRainTexture);
// 
//         //CSingletonScreenQuad quad;
// 
//         m_pShader->Begin();
//         m_pShader->BeginPass(0);
//         m_renderQuad.RenderQuad();
//         m_pShader->EndPass();
//         m_pShader->End();
    }

    //-----------------------------------------------------------------------------------
    void CRainRenderer::OnLostDevice()
    {
        m_pShader->OnLostDevice();

    }

    //-----------------------------------------------------------------------------------
    void CRainRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_pShader->OnResetDevice();

        Vec2 pixelsToTexels = CGPUQuad::GetPixelsToTexelsOffset(screenWidth, screenHeight);

        RenderCoordRect renderCoords;
        renderCoords.m_left += pixelsToTexels.x;
        renderCoords.m_right += pixelsToTexels.x;
        renderCoords.m_top += pixelsToTexels.y;
        renderCoords.m_bottom += pixelsToTexels.y;

        m_renderQuad.SetRenderRect(renderCoords);
        m_renderQuad.UpdateVertexBuffer();
    }
} // env