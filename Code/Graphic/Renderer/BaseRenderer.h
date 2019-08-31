/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BASERENDERER_H
#define BASERENDERER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/RenderSet.h>

#ifndef ENV_RENDERPASS_NAME
#define ENV_RENDERPASS_NAME(x, F) x = #F;
#endif // ENV_RENDERPASS_NAME

namespace env
{
    struct RenderQueue;
    struct RenderSet;

    class CBaseRenderer
    {
    public:
        CBaseRenderer() {}

        virtual ~CBaseRenderer() {}
        virtual bool Initialize() { return true; }
        virtual void OnLostDevice() {}
        virtual void OnResetDevice(int screenWidth, int screenHeight) {}
        virtual void OnLevelChanged() {}

    };

    struct RenderPassBaseFunctor
    {
        virtual ~RenderPassBaseFunctor() {}
        virtual void operator()(RenderQueue& renderQueue, RenderSet& renderSet) = 0;
    };

    template< class Type, class Func >
    struct RenderPassFunctor : RenderPassBaseFunctor
    {

        RenderPassFunctor(Type* p, const Func& f)
            : m_p(p)
            , m_func(f) {}

        virtual void operator()(RenderQueue& renderQueue, RenderSet& renderSet)
        {
            (m_p->*m_func)(renderQueue, renderSet);
        }

        Type* m_p;

        Func m_func;
    };


    class CRenderPassBase
    {
    public:
        CRenderPassBase() {}

        CRenderPassBase(const std::string& strName)
            : m_strName(strName) {}

        virtual ~CRenderPassBase() {}

        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet) = 0;

        const std::string& GetName() const { return m_strName; }

    protected:
        std::string m_strName;
    };

    class CImmediateRenderPass : public CRenderPassBase
    {
    public:
        CImmediateRenderPass()
            : m_pFunctor(nullptr) {}

        CImmediateRenderPass(const std::string& strName)
            : CRenderPassBase(strName)
            , m_pFunctor(nullptr) {}

        virtual ~CImmediateRenderPass()
        {
            ENV_DELETE(m_pFunctor);
        }

        template< class T, class F >
        CImmediateRenderPass* Init(T* p, F f)
        {
            ENV_ASSERT_DBG(m_pFunctor == nullptr);

            m_pFunctor = new RenderPassFunctor< T, F >(p, f);

            return this;
        }

        template< class T, class F >
        CImmediateRenderPass* Init(const std::string& strName, T* p, F f)
        {
            ENV_ASSERT_DBG(m_pFunctor == nullptr);

            m_strName = strName;

            m_pFunctor = new RenderPassFunctor< T, F >(p, f);

            return this;
        }

        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet)
        {
            ENV_ASSERT_DBG(m_pFunctor);

            (*m_pFunctor)(renderQueue, renderSet);
        }

    protected:

        RenderPassBaseFunctor* m_pFunctor;
    };

    template< class Type >
    class CDeferredRendererRenderPass : public CRenderPassBase
    {
    public:
        CDeferredRendererRenderPass()
            : CRenderPassBase()
            , m_pRenderer(nullptr) {}

        virtual void Init(const std::string& strName, Type* pRenderer)
        {
            m_strName = strName;
            m_pRenderer = pRenderer;
        }

        virtual void Render(RenderQueue& renderQueue, RenderSet& renderSet)
        {
            m_pRenderer->Render(renderQueue, renderSet);
        }

    protected:
        Type* m_pRenderer;
    };


} // env 
#endif // BASERENDERER_H 