#include <Graphic/RenderSystems/DeferredRenderSystem.h>
#include <Graphic/Core/RenderTarget.h>
#include <Graphic/Renderer/GeometryRenderer.h>
#include <Graphic/Renderer/FullscreenTextureRenderer.h>
#include <Graphic/Renderer/EnvironmentRenderer.h>
#include <Graphic/Renderer/LightingRenderer.h>
#include <Graphic/Renderer/SpriteRenderer.h>
#include <Graphic/Renderer/ParticleSystemRenderer.h>
#include <Graphic/Renderer/CollidableRenderer.h>
#include <Graphic/Renderer/GaussBlurRenderer.h>
#include <Graphic/Renderer/PostProcessRenderer.h>
#include <Graphic/Renderer/PolygonRenderer.h>
#include <Graphic/Renderer/BloomRenderer.h>
#include <Graphic/Renderer/RainRenderer.h>
#include <Graphic/Renderer/FadeRenderer.h>
#include <Graphic/Renderer/ForceFieldRenderer.h>
#include <Graphic/Renderer/CharacterRenderer.h>
#include <Graphic/Renderer/TextRenderer.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Renderer/DataCacheRenderer.h>
#include <Graphic/Renderer/FullscreenTextureRenderer.h>
#include <Graphic/Renderer/SpriteRenderer.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    struct CRenderSystem::RenderSystemImpl
    {
        CRenderSystem::RenderSystemImpl()
            : m_spritePassBeforePostProcessing(SPRITE_PIPELINE_BEFORE_POSTPROCESSING)
            , m_spritePassAfterPostProcessing(SPRITE_PIPELINE_AFTER_POSTPROCESSING)
            , m_spritePassAfterFadeScreen(SPRITE_PIPELINE_AFTER_FADE_SCREEN)
            , m_spritePassAfterFade(SPRITE_PIPELINE_AFTER_FADE) {}

        std::unique_ptr<CFullscreenTextureRenderer>     m_pFullscreenTextureRenderer;
        std::unique_ptr<CGaussBlurRenderer>             m_pGaussBlurRenderer;
        std::unique_ptr<CEnvironmentRenderer>           m_pEnvironmentRenderer;
        std::unique_ptr<CGeometryRenderer>              m_pGeometryRenderer;
        std::unique_ptr<CParticleSystemColorRenderer>   m_pParticleSystemColorRenderer;
        std::unique_ptr<CParticleSystemDistortRenderer> m_pParticleSystemDistortRenderer;
        std::unique_ptr<CLightingRenderer>              m_pLightingRenderer;
        std::unique_ptr<CSpriteRenderer>                m_pSpriteRenderer;
        std::unique_ptr<CPostProcessRenderer>           m_pPostProcessRenderer;
        std::unique_ptr<CBloomRenderer>                 m_pBloomRenderer;
        std::unique_ptr<CForceFieldRenderer>            m_pForceFieldRenderer;
        std::unique_ptr<CFadeRenderer>                  m_pFadeRenderer;
        std::unique_ptr<CCharacterRenderer>             m_pCharacterRenderer;
        std::unique_ptr<CTextRenderer>                  m_pTextRenderer;
        std::unique_ptr<CDataCacheRenderer>             m_pDataCacheRenderer;
        std::unique_ptr<CPolygonRenderer>               m_pPolygonRenderer;

        CImmediateRenderPass m_clearBackBufferPass;
        CImmediateRenderPass m_clearPass;
        CImmediateRenderPass m_renderDebugDataCaches;
        CImmediateRenderPass m_setupDeferredPass;
        CImmediateRenderPass m_geometryDeferredPass;
        CImmediateRenderPass m_geometryDeferredLayer0Pass;
        CImmediateRenderPass m_geometryTransmissivePass;
        CImmediateRenderPass m_geometryOrthoOutline;
        CImmediateRenderPass m_terrainDeferredPass;
        CImmediateRenderPass m_terrainDeferredLayer0Pass;
        CImmediateRenderPass m_terrainBorderDeferredPass;
        CImmediateRenderPass m_terrainPreviewDeferredPass;
        CImmediateRenderPass m_terrainSelectionDeferredPass;
        CImmediateRenderPass m_cleanupDeferredPass;
        CImmediateRenderPass m_geometryForwardPass;
        CImmediateRenderPass m_geometryMotionBlurPass;
        CImmediateRenderPass m_copyNoMotionBlurPass;
        CImmediateRenderPass m_envSetupPass;
        CImmediateRenderPass m_envCleanupPass;
        CImmediateRenderPass m_envEnergyFieldPass;
        CImmediateRenderPass m_envHaloPass;
        CImmediateRenderPass m_envSkyboxPass;
        CImmediateRenderPass m_envSkydomePass;
        CImmediateRenderPass m_envSkyPlanePass;
        CImmediateRenderPass m_envPlanetPass;
        //CImmediateRenderPass m_envWatcherVideoPass;
        CImmediateRenderPass m_geometryCharacterDistortPass;
        CImmediateRenderPass m_occlusionMaskPass;
        CImmediateRenderPass m_lightingPass;
        CImmediateRenderPass m_cartoonPass;
        CImmediateRenderPass m_postProcessPass;
        CImmediateRenderPass m_forceFieldPass;
        CImmediateRenderPass m_particleDefaultPass;
        CImmediateRenderPass m_particleCharacterPass;
        CImmediateRenderPass m_characterPass;
        CImmediateRenderPass m_characterCellsPass;
        CImmediateRenderPass m_textRenderPass;
        CImmediateRenderPass m_bloomPass;
        CImmediateRenderPass m_fadePass;
        CImmediateRenderPass m_fadeScreenPass;
        CImmediateRenderPass m_stripesPass;
        CImmediateRenderPass m_particleDistortPass;
        CImmediateRenderPass m_swapPass;
        CImmediateRenderPass m_fSPass;
        CImmediateRenderPass m_renderCurrentColorTarget;
        CImmediateRenderPass m_characterBackgroundPass;
        CImmediateRenderPass m_geometryCharacterPass;
        
        CSpriteRenderPass m_spritePassBeforePostProcessing;
        CSpriteRenderPass m_spritePassAfterPostProcessing;
        CSpriteRenderPass m_spritePassAfterFadeScreen;
        CSpriteRenderPass m_spritePassAfterFade;

        CFullScreenTextureRenderPass m_copyNoMotionBlurRenderPass;
    };

    //-----------------------------------------------------------------------------------
    CRenderSystem* gRenderSystem = nullptr;

    //-----------------------------------------------------------------------------------
    void CRenderSystem::Clear(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        const RenderQualityParams& videoParams = GetRenderQualityParams();

        Vec4 clearColor(0, 0, 0, 0);

        if (renderQueue.m_pEnvironmentRenderObject)
            clearColor = Vec4(renderQueue.m_pEnvironmentRenderObject->m_fogColor, 0.0f);

        m_pRenderDevice->SetViewport(Vec2(0.0f, 0.0f), videoParams.screenWidth, videoParams.screenHeight);
        m_pRenderDevice->SetRenderTarget(3, renderSet.m_pRenderTargetPostProcess01);
        m_pRenderDevice->Clear(clearColor, CLEAR_TYPE_TARGET, 1.0f, 0);
        m_pRenderDevice->SetRenderTarget(3, 0);
        m_pRenderDevice->SetDepthStencilTarget(renderSet.m_pBackBuffer);
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::ClearBackBuffer(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_pRenderDevice->SetRenderTarget(0, renderSet.m_pBackBuffer);
        m_pRenderDevice->Clear(Vec4(0, 0, 0, 1), CLEAR_TYPE_TARGET);
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::SwapRenderTargets(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        RenderSet::SwapRendertargets(&renderSet.m_pCurrentColorSource, &renderSet.m_pCurrentColorTarget);
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::RenderDebugDataCaches(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        GetImpl().m_pDataCacheRenderer->SetData<CVector3Cache::DataType>(renderQueue.m_vector3Caches, GetImpl().m_pPolygonRenderer->GetShader());
        GetImpl().m_pPolygonRenderer->Render(GetImpl().m_pDataCacheRenderer.get(), renderQueue.m_pCameraRenderObject->m_mtxViewProjection);
    }

    //-----------------------------------------------------------------------------------
    CRenderSystem::CRenderSystem()
        : CBaseRenderSystem()
        , m_impl(std::make_unique<CRenderSystem::RenderSystemImpl>())
    {
        gRenderSystem = this;
    }

    //-----------------------------------------------------------------------------------
    CRenderSystem::~CRenderSystem()
    {
        ReleaseVertexDeclarations();
    }

    //-----------------------------------------------------------------------------------
    bool CRenderSystem::Initialize(const RenderQualityParams& videoParams, const RenderSystemParams& renderSysParams)
    {
        if (!CBaseRenderSystem::Initialize(videoParams, renderSysParams))
            return false;

        ENV_LOG_INFO("INITIALIZING RENDERSYSTEM");

        InitVertexDeclarations(m_pRenderDevice);

        if (!InitializeRenderTargets())
            return false;

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CRenderSystem::InitializeRenderers()
    {
        if (!CreateRenderers())
            return false;

        return CBaseRenderSystem::InitializeRenderers();
    }

    typedef void (CFadeRenderer::*myFunc)(RenderQueue&, RenderSet&);

    //-----------------------------------------------------------------------------------
    bool CRenderSystem::CreateRenderers()
    {
        auto& impl = GetImpl();

        impl.m_pFullscreenTextureRenderer = std::make_unique<CFullscreenTextureRenderer>(m_pRenderDevice);
        impl.m_pGaussBlurRenderer = std::make_unique<CGaussBlurRenderer>(m_pRenderDevice);
        impl.m_pEnvironmentRenderer = std::make_unique<CEnvironmentRenderer>(m_pRenderDevice);
        impl.m_pGeometryRenderer = std::make_unique<CGeometryRenderer>(m_pRenderDevice, impl.m_pFullscreenTextureRenderer.get());
        impl.m_pParticleSystemColorRenderer = std::make_unique<CParticleSystemColorRenderer>(m_pRenderDevice);
        impl.m_pParticleSystemDistortRenderer = std::make_unique<CParticleSystemDistortRenderer>(m_pRenderDevice);
        impl.m_pLightingRenderer = std::make_unique<CLightingRenderer>(m_pRenderDevice);
        impl.m_pSpriteRenderer = std::make_unique<CSpriteRenderer>(m_pRenderDevice);
        impl.m_pPostProcessRenderer = std::make_unique<CPostProcessRenderer>(m_pRenderDevice, impl.m_pFullscreenTextureRenderer.get());
        impl.m_pBloomRenderer = std::make_unique<CBloomRenderer>(m_pRenderDevice, impl.m_pGaussBlurRenderer.get());
        impl.m_pForceFieldRenderer = std::make_unique<CForceFieldRenderer>(m_pRenderDevice);
        impl.m_pFadeRenderer = std::make_unique<CFadeRenderer>(m_pRenderDevice);
        impl.m_pCharacterRenderer = std::make_unique<CCharacterRenderer>(m_pRenderDevice, impl.m_pSpriteRenderer.get());
        impl.m_pTextRenderer = std::make_unique<CTextRenderer>(impl.m_pSpriteRenderer.get());
        impl.m_pDataCacheRenderer = std::make_unique<CDataCacheRenderer>(m_pRenderDevice);
        impl.m_pPolygonRenderer = std::make_unique<CPolygonRenderer>();

        AddRenderer("FullscreenRenderer", impl.m_pFullscreenTextureRenderer.get());
        AddRenderer("GaussBlurRenderer", impl.m_pGaussBlurRenderer.get());
        AddRenderer("EnvironmentRenderer", impl.m_pEnvironmentRenderer.get());
        AddRenderer("GeometryRenderer", impl.m_pGeometryRenderer.get());
        AddRenderer("ParticleColorRenderer", impl.m_pParticleSystemColorRenderer.get());
        AddRenderer("ParticleDistortRenderer", impl.m_pParticleSystemDistortRenderer.get());
        AddRenderer("LightingRenderer", impl.m_pLightingRenderer.get());
        AddRenderer("SpriteRenderer", impl.m_pSpriteRenderer.get());
        AddRenderer("PostProcess", impl.m_pPostProcessRenderer.get());
        AddRenderer("BloomRenderer", impl.m_pBloomRenderer.get());
        AddRenderer("ForceFieldRenderer", impl.m_pForceFieldRenderer.get());
        AddRenderer("FadeRenderer", impl.m_pFadeRenderer.get());
        AddRenderer("CharacterRenderer", impl.m_pCharacterRenderer.get());
        AddRenderer("TextRenderer", impl.m_pTextRenderer.get());
        AddRenderer("pDataCacheRenderer", impl.m_pDataCacheRenderer.get());
        AddRenderer("PolygonRenderer", impl.m_pPolygonRenderer.get());

        impl.m_copyNoMotionBlurRenderPass.Initialize(impl.m_pFullscreenTextureRenderer.get());
        impl.m_copyNoMotionBlurRenderPass.GetRenderStates().SetNumStates(m_pRenderDevice, 4, 0, 0);
        impl.m_copyNoMotionBlurRenderPass.GetRenderStates().AddRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        impl.m_copyNoMotionBlurRenderPass.GetRenderStates().AddRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        impl.m_copyNoMotionBlurRenderPass.GetRenderStates().AddRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
        impl.m_copyNoMotionBlurRenderPass.GetRenderStates().AddRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_GREATER);
        impl.m_copyNoMotionBlurRenderPass.GetRenderStates().AddRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_KEEP);
        impl.m_copyNoMotionBlurRenderPass.GetRenderStates().AddRenderState(RENDERSTATE_TYPE_STENCILREF, STENCILREF_DEFERRED_GEOMETRY_MOTIONBLUR);

        impl.m_clearBackBufferPass.Init("ClearBackBuffer", this, &CRenderSystem::ClearBackBuffer);
        impl.m_clearPass.Init("Clear", this, &CRenderSystem::Clear);
        impl.m_renderDebugDataCaches.Init("DebugDataCaches", this, &CRenderSystem::RenderDebugDataCaches);
        impl.m_setupDeferredPass.Init("SetupDeferred", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::SetupDeferredRendering);
        impl.m_geometryDeferredPass.Init("GeometryDeferred", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryDeferred);
        impl.m_geometryDeferredLayer0Pass.Init("GeometryDeferredLayer0", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryDeferredLayer0);
        impl.m_geometryTransmissivePass.Init("GeometryTransmissive", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryTransmissive);
        impl.m_geometryCharacterPass.Init("GeometryCharacter", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryCharacter);
        impl.m_geometryCharacterDistortPass.Init("GeometryCharacterDistort", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryCharacterDistort);
        impl.m_geometryOrthoOutline.Init("GeometryOutline", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryOutline);
        impl.m_terrainDeferredPass.Init("Terrain", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderLevelTerrain);
        impl.m_terrainDeferredLayer0Pass.Init("Terrain", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderLevelTerrainLayer0);
        impl.m_terrainBorderDeferredPass.Init("TerrainBorder", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderLevelTerrainBorder);
        impl.m_terrainPreviewDeferredPass.Init("TerrainPreview", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderLevelTerrainPreview);
        impl.m_terrainSelectionDeferredPass.Init("TerrainSelection", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderLevelTerrainSelection);
        impl.m_cleanupDeferredPass.Init("CleanupDeferred", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::CleanupDeferredRendering);
        impl.m_geometryForwardPass.Init("GeometryForward", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryForward);
        impl.m_geometryMotionBlurPass.Init("GeometryMotionBlur", impl.m_pGeometryRenderer.get(), &CGeometryRenderer::RenderGeometryMotionBlur);
        impl.m_copyNoMotionBlurPass.Init("CopyNoMotionBlur", &impl.m_copyNoMotionBlurRenderPass, &CFullScreenTextureRenderPass::Render);
        impl.m_envSetupPass.Init("EnvSetup", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::SetupEnvRendering);
        impl.m_envCleanupPass.Init("EnvCleanup", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::CleanupEnvRendering);
        impl.m_envEnergyFieldPass.Init("EnvEnergyField", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::RenderEnergyField);
        impl.m_envHaloPass.Init("EnvHalo", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::RenderHalo);
        impl.m_envSkyboxPass.Init("EnvSkybox", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::RenderSkybox);
        impl.m_envSkydomePass.Init("EnvSkydome", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::RenderSkyDome);
        impl.m_envSkyPlanePass.Init("EnvSkyplane", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::RenderSkyPlane);
        impl.m_envPlanetPass.Init("EnvPlanet", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::RenderPlanet);
        //impl.m_envWatcherVideoPass.Init("EnvWatcherVideo", impl.m_pEnvironmentRenderer.get(), &CEnvironmentRenderer::RenderWatcherVideo);
        impl.m_occlusionMaskPass.Init("OcclusionMask", this, &CRenderSystem::RenderLightraysMask);
        impl.m_lightingPass.Init("Lighting", impl.m_pLightingRenderer.get(), &CLightingRenderer::Render);
        impl.m_spritePassBeforePostProcessing.Init("SpritesBeforePostProcessing", impl.m_pSpriteRenderer.get());
        impl.m_spritePassAfterPostProcessing.Init("SpritesAfterPostProcessing", impl.m_pSpriteRenderer.get());
        impl.m_spritePassAfterFade.Init("SpritesAfterFade", impl.m_pSpriteRenderer.get());
        impl.m_spritePassAfterFadeScreen.Init("SpritesAfterFadeScreen", impl.m_pSpriteRenderer.get());
        impl.m_cartoonPass.Init("Cartoon", impl.m_pPostProcessRenderer.get(), &CPostProcessRenderer::RenderCartoon);
        impl.m_postProcessPass.Init("PostProcess", impl.m_pPostProcessRenderer.get(), &CPostProcessRenderer::RenderPostProcessing);
        impl.m_forceFieldPass.Init("ForceFields", impl.m_pForceFieldRenderer.get(), &CForceFieldRenderer::Render);
        impl.m_particleDefaultPass.Init("ParticleDefault", impl.m_pParticleSystemColorRenderer.get(), &CParticleSystemColorRenderer::RenderDefault);
        impl.m_particleDistortPass.Init("ParticleDistort", impl.m_pParticleSystemDistortRenderer.get(), &CParticleSystemDistortRenderer::RenderDefault);
        impl.m_characterPass.Init("Character", impl.m_pCharacterRenderer.get(), &CCharacterRenderer::Render);
        impl.m_characterCellsPass.Init("CharacterCells", impl.m_pCharacterRenderer.get(), &CCharacterRenderer::RenderCells);
        impl.m_textRenderPass.Init("Text", impl.m_pTextRenderer.get(), &CTextRenderer::Render);
        impl.m_bloomPass.Init("Bloom", impl.m_pBloomRenderer.get(), &CBloomRenderer::Render);
        impl.m_fadePass.Init("Fade", impl.m_pFadeRenderer.get(), &CFadeRenderer::Render);
        impl.m_fadeScreenPass.Init("FadeScreen", impl.m_pFadeRenderer.get(), &CFadeRenderer::RenderFadeScreen);
        impl.m_stripesPass.Init("Stripes", impl.m_pFadeRenderer.get(), &CFadeRenderer::RenderStripes);
        impl.m_swapPass.Init("SwapTargets", this, &CRenderSystem::SwapRenderTargets);
        impl.m_renderCurrentColorTarget.Init("CurrentColorTarget", this, &CRenderSystem::RenderCurrentColorTarget);
        impl.m_characterBackgroundPass.Init("CharacterBackground", impl.m_pCharacterRenderer.get(), &CCharacterRenderer::RenderBackground);

        ENV_LOG_INFO("INITIALIZING RENDERSYSTEM SUCCEEDED");

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::RenderLoadingScreen(RenderQueue& renderQueue)
    {
//         m_pRenderDevice->BeginScene();
// 
//         m_pRenderDevice->SetRenderTarget(0, m_pRenderDevice->GetBackBuffer());
//         m_pRenderDevice->Clear(Vec4(0.2f, 0.2f, 0.2f, 1), CLEAR_TYPE_TARGET);
// 
//         m_pRenderDevice->EndScene();
        auto& impl = GetImpl();

        m_renderSet.m_pCurrentColorTarget = m_renderSet.m_pRenderTargetPostProcess02;
        m_renderSet.m_pCurrentColorSource = m_renderSet.m_pRenderTargetPostProcess01;
        m_renderSet.m_pBackBuffer = m_pRenderDevice->GetBackBuffer();

        ResetRenderPasses();

//         AddRenderPass(&impl.m_clearPass);
// 
//         AddRenderPass(&impl.m_fadePass);
//         AddRenderPass(&impl.m_stripesPass);
// 
//         Render(0.0f, 0.0f);
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::AddRenderPass(CRenderPassBase* pRenderPass)
    {
        m_currentRenderPasses.push_back(pRenderPass);
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::ResetRenderPasses()
    {
        m_currentRenderPasses.reserve(50);
        m_currentRenderPasses.clear();
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::Render(float time, float dt)
    {
        ENV_SCOPED_GPU_PROFILING(Rendering, Render);

        RenderQueue& renderQueue = *m_pRenderQueue;

        for (unsigned int i = 0; i < m_currentRenderPasses.size(); ++i)
        {
            CRenderPassBase* pRenderPass = m_currentRenderPasses[i];

            ENV_SCOPED_NAMED_GPU_PROFILING(RenderPasses, pRenderPass->GetName());

            ENV_NOT_FINAL_CODE(m_pRenderDevice->BeginPerformanceMarker(pRenderPass->GetName()));

            pRenderPass->Render(renderQueue, m_renderSet);

            ENV_NOT_FINAL_CODE(m_pRenderDevice->EndPerformanceMarker());
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::RenderLightraysMask(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        if (renderQueue.m_pLightRaysRenderObject &&
            renderQueue.m_pLightRaysRenderObject->m_bLightRaysOcclusionMask)
        {
            m_pRenderDevice->SetRenderTarget(0, m_renderSet.m_pRenderTargetOcclusionMask);
            m_pRenderDevice->Clear(Vec4(1, 1, 1, 1), CLEAR_TYPE_TARGET, 1.0f, 0);
            m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, true);
            m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, 0);
            m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_NOTEQUAL);
            m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_STENCILPASS, STENCILOP_TYPE_KEEP);
            m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFAIL, STENCILOP_TYPE_KEEP);
            GetImpl().m_pFullscreenTextureRenderer->Render(renderQueue);
            m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::PreRender()
    {
        auto& impl = GetImpl();

        if (!m_pRenderQueue ||
            !m_pRenderQueue->IsValid())
        {
            ResetRenderPasses();

            AddRenderPass(&impl.m_clearBackBufferPass);
        }
        else
        {
            m_renderSet.m_pCurrentColorTarget = m_renderSet.m_pRenderTargetPostProcess02;
            m_renderSet.m_pCurrentColorSource = m_renderSet.m_pRenderTargetPostProcess01;
            m_renderSet.m_pBackBuffer = m_pRenderDevice->GetBackBuffer();

            ResetRenderPasses();

            AddRenderPass(&impl.m_clearPass);
            AddRenderPass(&impl.m_setupDeferredPass);
            AddRenderPass(&impl.m_terrainBorderDeferredPass);
            AddRenderPass(&impl.m_terrainDeferredPass);
            AddRenderPass(&impl.m_geometryDeferredLayer0Pass);
            AddRenderPass(&impl.m_terrainDeferredLayer0Pass);
            AddRenderPass(&impl.m_geometryDeferredPass);

            ENV_NOT_FINAL_CODE(AddRenderPass(&impl.m_terrainSelectionDeferredPass));
            ENV_NOT_FINAL_CODE(AddRenderPass(&impl.m_terrainPreviewDeferredPass));

            AddRenderPass(&impl.m_cleanupDeferredPass);

            AddRenderPass(&impl.m_envSetupPass);
            AddRenderPass(&impl.m_envSkydomePass);
            AddRenderPass(&impl.m_envSkyboxPass);
            AddRenderPass(&impl.m_envEnergyFieldPass);
            AddRenderPass(&impl.m_envPlanetPass);
            AddRenderPass(&impl.m_envSkyPlanePass);
            AddRenderPass(&impl.m_envCleanupPass);

            AddRenderPass(&impl.m_lightingPass);

            AddRenderPass(&impl.m_envHaloPass);

            AddRenderPass(&impl.m_cartoonPass);
            AddRenderPass(&impl.m_characterCellsPass);


            AddRenderPass(&impl.m_spritePassBeforePostProcessing);
            AddRenderPass(&impl.m_forceFieldPass);

            if (m_pRenderQueue->bRenderParticles)
                AddRenderPass(&impl.m_particleDefaultPass);

            //AddRenderPass(&m_pGeometryCharacterPass);

            //AddRenderPass(&impl.m_swapPass);
            //AddRenderPass(&impl.m_renderCurrentColorTarget);
            AddRenderPass(&impl.m_geometryCharacterPass);
          
            AddRenderPass(&impl.m_swapPass);

            AddRenderPass(&impl.m_bloomPass);

            ENV_NOT_FINAL_CODE(AddRenderPass(&impl.m_geometryOrthoOutline));
            
            //AddRenderPass(&impl.m_swapPass);
            //AddRenderPass(&impl.m_geometryMotionBlurPass);
            //AddRenderPass(&impl.m_copyNoMotionBlurPass);

            ENV_NOT_FINAL_CODE(AddRenderPass(&impl.m_geometryForwardPass));

            AddRenderPass(&impl.m_swapPass);

             AddRenderPass(&impl.m_renderCurrentColorTarget);

            if (m_pRenderQueue->bRenderParticles)
                AddRenderPass(&impl.m_particleDistortPass);

            AddRenderPass(&impl.m_geometryTransmissivePass);

            AddRenderPass(&impl.m_swapPass);

            AddRenderPass(&impl.m_postProcessPass);

            AddRenderPass(&impl.m_spritePassAfterPostProcessing);

            // Fade screen
            AddRenderPass(&impl.m_fadeScreenPass);
            AddRenderPass(&impl.m_spritePassAfterFadeScreen);
            AddRenderPass(&impl.m_swapPass);
            
            // Fade & stripes
            AddRenderPass(&impl.m_fadePass);
            AddRenderPass(&impl.m_stripesPass);
            AddRenderPass(&impl.m_spritePassAfterFade);
            AddRenderPass(&impl.m_textRenderPass);

            ENV_NOT_FINAL_CODE(AddRenderPass(&impl.m_renderDebugDataCaches));
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::RenderCurrentColorTarget(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_pRenderDevice->ResetToDefaultRenderStates();
        m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, true);
        m_pRenderDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);

        GetImpl().m_pFullscreenTextureRenderer->Render(renderSet.m_pCurrentColorSource, renderQueue, renderSet, renderSet.m_pCurrentColorTarget);
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::OnLostDevice()
    {
        CBaseRenderSystem::OnLostDevice();

        for (const auto& pRenderTarget : m_renderTargets)
        {
            pRenderTarget->OnLostDevice();
        }
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::OnResetDevice()
    {
        ENV_LOG_INFO("CD3D11DeferredRenderSystem::OnResetDevice");

        CBaseRenderSystem::OnResetDevice();

        for (const auto& pRenderTarget : m_renderTargets)
        {
            pRenderTarget->SetSize(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight);
            pRenderTarget->OnResetDevice();
        }

        ENV_LOG_INFO("CD3D11DeferredRenderSystem::OnResetDevice SUCCEEDED");
    }

    //-----------------------------------------------------------------------------------
    void CRenderSystem::OnLevelChanged()
    {
        CBaseRenderSystem::OnLevelChanged();
    }

    //-----------------------------------------------------------------------------------
    bool CRenderSystem::InitializeRenderTargets()
    {
        ENV_LOG_INFO("CREATING RENDER TARGETS");

        m_renderTargets.reserve(10);
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_A16B16G16R16F));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_A16B16G16R16F));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_R32F));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_R11G11B10));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_A16B16G16R16F));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_A16B16G16R16F));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_A8R8G8B8));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_A16B16G16R16F, true));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_A16B16G16R16F));
        m_renderTargets.push_back(m_pRenderDevice->CreateRenderTarget(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight, TEXTUREFORMAT_TYPE_D24S8, true, false));

        m_renderSet.m_pRenderTargetColors = m_renderTargets[0].get();
        m_renderSet.m_pRenderTargetNormals = m_renderTargets[1].get();
        m_renderSet.m_pRenderTargetDepths = m_renderTargets[2].get();
        m_renderSet.m_pRenderTargetEmissive = m_renderTargets[3].get();
        m_renderSet.m_pRenderTargetPostProcess01 = m_renderTargets[4].get();
        m_renderSet.m_pRenderTargetPostProcess02 = m_renderTargets[5].get();
        m_renderSet.m_pRenderTargetOcclusionMask = m_renderTargets[6].get();
        m_renderSet.m_pRenderTargetDepthMapOrtho = m_renderTargets[7].get();
        m_renderSet.m_pRenderTargetBlurBilateral = m_renderTargets[8].get();
        m_renderSet.m_pForwardBackBuffer = m_renderTargets[9].get();

        for (const auto& pRenderTarget : m_renderTargets)
            pRenderTarget->SetTextureSampler(CDefaultTextureSampler::GetPointClampSampler());

        m_renderSet.m_pRenderTargetBlurBilateral->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());
        m_renderSet.m_pRenderTargetBlurBilateral->SetTextureSampler(CDefaultTextureSampler::GetLinearClampSampler());

        ENV_LOG_INFO("CREATING RENDER TARGETS SUCCEEDED");

        return true;
    }


} // env
