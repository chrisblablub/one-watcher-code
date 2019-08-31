#include <Graphic/Core/Quad.h>

#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/VertexBuffer.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    bool CGPUQuad::Initialize(CRenderDevice* pD3DDevice, const TextureCoordRect& textureCoords, const RenderCoordRect& renderCoords)
    {
        if (!CGenericGPUQuad<PosTexVertex>::Initialize(pD3DDevice))
            return false;
        
        SetTextureRect(textureCoords);
        SetRenderRect(renderCoords);

        UpdateVertexBuffer();

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CGPUQuad::SetTextureRect(const TextureCoordRect& coords)
    {
        m_coords = coords;
    }

    //-----------------------------------------------------------------------------------
    void CGPUQuad::SetRenderRect(const RenderCoordRect& renderRect)
    {
        m_renderRect = renderRect;
    }

    //-----------------------------------------------------------------------------------
    void CGPUQuad::UpdateVertexBuffer()
    {
        PosTexVertex* pVertex = static_cast<PosTexVertex*>(m_pVB->Lock(0, 0, 0));

        // TODO: consider m_renderRect
        pVertex[0].p = Vec3(m_renderRect.m_left, m_renderRect.m_bottom, 0);
        pVertex[0].t = Vec2(m_coords.m_left, m_coords.m_bottom);

        pVertex[1].p = Vec3(m_renderRect.m_left, m_renderRect.m_top, 0);
        pVertex[1].t = Vec2(m_coords.m_left, m_coords.m_top);

        pVertex[2].p = Vec3(m_renderRect.m_right, m_renderRect.m_top, 0);
        pVertex[2].t = Vec2(m_coords.m_right, m_coords.m_top);

        pVertex[3].p = Vec3(m_renderRect.m_right, m_renderRect.m_top, 0);
        pVertex[3].t = Vec2(m_coords.m_right, m_coords.m_top);

        pVertex[4].p = Vec3(m_renderRect.m_right, m_renderRect.m_bottom, 0);
        pVertex[4].t = Vec2(m_coords.m_right, m_coords.m_bottom);

        pVertex[5].p = Vec3(m_renderRect.m_left, m_renderRect.m_bottom, 0);
        pVertex[5].t = Vec2(m_coords.m_left, m_coords.m_bottom);

        m_pVB->Unlock();
    }

    //-----------------------------------------------------------------------------------
    Vec2 CGPUQuad::GetPixelsToTexelsOffset(int targetWidth, int targetHeight)
    {
        return Vec2(-1.0f / (float)targetWidth, 1.0f / (float)targetHeight);
    }
}