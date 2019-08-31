/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef DATA_CACHE_RENDERER_H
#define DATA_CACHE_RENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Renderer/BaseRenderer.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/DataCache.h>


namespace env
{
    class ENV_DECL_ALIGN_16 CDataCacheRenderer : public CBaseRenderer
    {
    public:
        ENV_DEFINE_ALLOC_ALIGNED_16

        CDataCacheRenderer(CRenderDevice* pDevice)
            : m_pDevice(pDevice)
            , m_pVB(nullptr)
            , m_pShader(nullptr)
            , m_numElements(0)
            , m_primType(PRIMITIVE_TYPE_POINTLIST) {}

        ~CDataCacheRenderer()
        {
            ENV_DELETE(m_pVB);
        }

        template< typename DataElementType > void SetData(const std::list<CDataCache<DataElementType>*>& caches, CShader* pShader)
        {
            ENV_DELETE(m_pVB);

            m_numElements = 0;

            for (typename std::list<CDataCache<DataElementType>*>::const_iterator it = caches.begin(); it != caches.end(); ++it)
            {
                const CDataCache<DataElementType>* pCache = *it;
                m_numElements += pCache->GetNumElements();
            }

            if (m_numElements == 0 || caches.empty())
                return;

            m_primType  = (*caches.front()).GetPrimitiveType();
            m_pDecl     = m_pDevice->CreateVertexDeclaration(DataElementType::VertexElements, sizeof(DataElementType), pShader);
            m_pVB       = m_pDevice->CreateVertexBuffer(m_numElements * m_pDecl->GetStride(), USAGE_TYPE_VERTEXINDEXBUFFERS);
            m_pShader   = pShader;

            char* p = static_cast<char*>(m_pVB->Lock(0, 0, 0));

            for (typename std::list<CDataCache<DataElementType>*>::const_iterator it = caches.begin(); it != caches.end(); ++it)
            {
                const CDataCache<DataElementType>* pCache = *it;

                if (pCache->GetNumElements() == 0)
                    continue;

                memcpy(p, pCache->GetData(), pCache->GetNumElements() * m_pDecl->GetStride());
                p += pCache->GetNumElements() * m_pDecl->GetStride();
            }

            m_pVB->Unlock();
        }

        void Render()
        {
            if (!m_numElements ||
                !m_pDecl ||
                !m_pVB || 
                !m_pShader)
            {
                return;
            }

            int numPrimitives = m_numElements;
            if (m_primType == PRIMITIVE_TYPE_LINELIST)
                numPrimitives /= 2;
            else if (m_primType == PRIMITIVE_TYPE_TRIANGLELIST)
                numPrimitives /= 3;

            m_pDevice->ResetToDefaultRenderStates();
            m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);
            m_pDevice->SetVertexDeclaration(m_pDecl.get());
            m_pDevice->SetVertexBuffer(0, 0, m_pVB);
            m_pDevice->SetShader(m_pShader);
            m_pDevice->DrawPrimitive(m_primType, 0, numPrimitives);
        }

    private:

        Matrix4x4 m_mtxTransform;

        CVertexBuffer* m_pVB;
        CRenderDevice* m_pDevice;
        CShader* m_pShader;

        unsigned int m_numElements;

        std::unique_ptr<CVertexDeclaration> m_pDecl;
        PRIMITIVE_TYPE m_primType;
    };

} // env
#endif // DATA_CACHE_RENDERER_H 