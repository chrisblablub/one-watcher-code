/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SCREENQUAD_H
#define SCREENQUAD_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/VertexDeclarations.h>

namespace env
{
    struct TextureCoordRect
    {
        TextureCoordRect()
            : m_left(0.0f)
            , m_top(0.0f)
            , m_right(1.0f)
            , m_bottom(1.0f) {}

        TextureCoordRect(float l, float t, float r, float b)
            : m_left(l)
            , m_top(t)
            , m_right(r)
            , m_bottom(b) {}


        float m_left;
        float m_top;
        float m_right;
        float m_bottom;
    };

    struct RenderCoordRect
    {
        RenderCoordRect()
            : m_left(-1.0f),
            m_top(1.0f),
            m_right(1.0f),
            m_bottom(-1.0f) {}

        float m_left;
        float m_top;
        float m_right;
        float m_bottom;
    };

    class CRenderDevice;
    class CVertexBuffer;

    template<typename VertexGPU>
    class CGenericGPUQuad
    {
    public:
        CGenericGPUQuad()
            : m_pD3DDevice(nullptr)
            , m_pVB(nullptr) {}

        ~CGenericGPUQuad()
        {
            ENV_DELETE(m_pVB);
        }

        bool Initialize(CRenderDevice* pD3DDevice) 
        {
            m_pD3DDevice = pD3DDevice;

            ENV_DELETE(m_pVB);

            m_pVB = m_pD3DDevice->CreateVertexBuffer(6 * sizeof(VertexGPU), USAGE_TYPE_VERTEXINDEXBUFFERS);

            return m_pVB != nullptr;
        }

        inline VertexGPU* Lock()
        {
            return static_cast<VertexGPU*>(m_pVB->Lock(0, 0, 0));
        }

        inline void Unlock()
        {
            m_pVB->Unlock();
        }

        inline void BindQuad()
        {
            m_pD3DDevice->SetVertexBuffer(0, 0, m_pVB);
        }

        inline void DrawQuad()
        {
            m_pD3DDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 2);
        }

        inline void RenderQuad()
        {
            m_pD3DDevice->SetVertexBuffer(0, 0, m_pVB);
            m_pD3DDevice->DrawPrimitive(PRIMITIVE_TYPE_TRIANGLELIST, 0, 2);
        }

    protected:
        CRenderDevice* m_pD3DDevice;
        CVertexBuffer* m_pVB;
    };

    class CGPUQuad : public CGenericGPUQuad<PosTexVertex>
    {
    public:
        bool Initialize(CRenderDevice* pD3DDevice,
                        const TextureCoordRect& textureCoords = TextureCoordRect(),
                        const RenderCoordRect& renderCoords = RenderCoordRect());

        void SetTextureRect(const TextureCoordRect& coords);
        void SetRenderRect(const RenderCoordRect& renderRect);

        void UpdateVertexBuffer();

        static Vec2 GetPixelsToTexelsOffset(int targetWidth, int targetHeight);

        const TextureCoordRect& GetTextureCoords() { return m_coords; }

    private:

        TextureCoordRect m_coords;
        RenderCoordRect m_renderRect;
    };


} /* env */
#endif /* SCREENQUAD_H */