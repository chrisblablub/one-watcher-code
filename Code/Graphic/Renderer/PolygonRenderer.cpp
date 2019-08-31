#include <Graphic/Renderer/PolygonRenderer.h>
#include <Graphic/Renderer/DataCacheRenderer.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/VertexDeclarations.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CPolygonRenderer::CPolygonRenderer()
        : m_pShader(nullptr) {}

    //-----------------------------------------------------------------------------------
    CPolygonRenderer::~CPolygonRenderer()
    {
        if (m_pShader)
            gShaderMgr->Release(m_pShader->GetName());
    }

    //-----------------------------------------------------------------------------------
    bool CPolygonRenderer::Initialize()
    {
        m_pShader = gShaderMgr->AddGetPtr("Polygons");
        m_pShader->LinkConstantBufferCPU("cb_PolygonVS", m_constBufPolygonVS);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CPolygonRenderer::Render(CDataCacheRenderer* pRenderer, const Matrix4x4& mtxViewProj)
    {
        gRenderSystem->GetRenderDevice()->SetTexture(0, nullptr);
        gRenderSystem->GetRenderDevice()->SetTexture(1, nullptr);
        gRenderSystem->GetRenderDevice()->SetTexture(2, nullptr);
        gRenderSystem->GetRenderDevice()->SetTexture(3, nullptr);

        m_constBufPolygonVS.GetDataForWriting() = mtxViewProj;

        pRenderer->Render();
    }

    //-----------------------------------------------------------------------------------
    void CPolygonRenderer::OnLostDevice()
    {
    }

    //-----------------------------------------------------------------------------------
    void CPolygonRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
    }
} // env