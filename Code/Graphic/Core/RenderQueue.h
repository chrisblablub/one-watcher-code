/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <Graphic/GraphicCommon.h>
//#include <Graphic/Core/Material.h>
#include <Graphic/RenderObjects/BloomRenderObject.h>
#include <Graphic/RenderObjects/LightRaysRenderObject.h>
#include <Graphic/RenderObjects/CharacterRenderObject.h>
#include <Graphic/RenderObjects/CameraRenderObject.h>
#include <Graphic/RenderObjects/MeshRenderObject.h>
#include <Graphic/RenderObjects/SpriteRenderObject.h>
#include <Graphic/RenderObjects/LightRenderObject.h>
#include <Graphic/RenderObjects/EnvironmentRenderObject.h>
#include <Graphic/RenderObjects/PostProcessRenderObject.h>
#include <Graphic/RenderObjects/TextRenderObject.h>
#include <Graphic/RenderObjects/FadeRenderObject.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>
#include <Graphic/RenderObjects/WatcherVideoRenderObject.h>
#include <Graphic/RenderObjects/ParticleSystemRenderObject.h>
#include <Graphic/RenderObjects/SkyPlaneRenderObject.h>
#include <Graphic/RenderObjects/SkyDomeRenderObject.h>
#include <Graphic/RenderObjects/SkyBoxRenderObject.h>
#include <Graphic/RenderObjects/PlanetRenderObject.h>
#include <Graphic/RenderObjects/SunRenderObject.h>
#include <Graphic/RenderObjects/EnergyFieldRenderObject.h>
#include <Graphic/RenderObjects/ForceFieldRenderObject.h>
#include <Graphic/RenderObjects/EnvMapRenderObject.h>
#include <Graphic/RenderObjects/StripeRenderObject.h>
#include <Graphic/RenderObjects/HaloRenderObject.h>
#include <Graphic/Core/Material.h>

namespace env
{
    struct PolygonVertex;


    template< typename T > class CDataCache;
    typedef CDataCache< PolygonVertex > CPolygonEntityCache;


    class CSkyDomeEntity;
    class CPlanetEntity;
    class CUniversePlanetEntity;
    class CSunEntity;
    class CHaloEntity;
    class CSkyBoxEntity;
    class CEnergyFieldEntity;
    class CWatcherVideoEntity;
    class CMeshEntity;



    struct RenderQueue
    {
        enum LIGHTING_MODE
        {
            // Render every light source dynamically
            LIGHTING_MODE_DYNAMIC = 0,
            LIGHTING_MODE_FINAL,
            LIGHTING_MODE_PRECOMPUTEDONLY,
            LIGHTING_MODE_DYNAMICONLY,
            LIGHTING_MODE_OFF
        };

        
        typedef std::list< CDataCache<PolygonVertex>* > listVector3Caches;


        RenderQueue()
            : bRenderParticles(true)
            , bRenderDebugData(false)
            , bRenderMeshes(true)
            , bRenderLevelTerrain(true)
            , bRenderLevelBorder(true)
            , m_pCharacterRenderObject(nullptr)
            , m_pCharacter3DRenderObject(nullptr)
            , m_pWatcherVideoRenderObject(nullptr)
            , m_pHaloRenderObject(nullptr)
            , m_pBloomRenderObject(nullptr)
            , m_pCameraRenderObject(nullptr)
            , m_pLightRaysRenderObject(nullptr)
            , m_pEnvironmentRenderObject(nullptr)
            , m_pFadeBarsRenderObject(nullptr)
            , m_pSkyPlaneRenderObject(nullptr)
            , m_pSkyDomeRenderObject(nullptr)
            , m_pSkyBoxRenderObject(nullptr)
            , m_pSunRenderObject(nullptr)
            , m_pPlanetRenderObject(nullptr)
            , m_pLevelGridRenderObject(nullptr)
            , m_pEnergyFieldRenderObject(nullptr)
            , m_pEnvMapRenderObject(nullptr)
            , timeElapsedFrame(0.0f)
            , timeElapsed(0.0f)
            , lightingMode(LIGHTING_MODE_FINAL) 
        {
            m_meshInstances.resize(MESH_RENDER_MODE_COUNT);
        }

        BloomRenderObject* m_pBloomRenderObject;
        LightRaysRenderObject* m_pLightRaysRenderObject;
        CharacterRenderObject* m_pCharacterRenderObject;
        Character3DRenderObject* m_pCharacter3DRenderObject;
        CameraRenderObject* m_pCameraRenderObject;
        EnvironmentRenderObject* m_pEnvironmentRenderObject;
        WatcherVideoRenderObject* m_pWatcherVideoRenderObject;
        HaloRenderObject* m_pHaloRenderObject;
        FadeBarsRenderObject* m_pFadeBarsRenderObject;
        SkyPlaneRenderObject* m_pSkyPlaneRenderObject;
        SkyDomeRenderObject* m_pSkyDomeRenderObject;
        SkyBoxRenderObject* m_pSkyBoxRenderObject;
        SunRenderObject* m_pSunRenderObject;
        PlanetRenderObject* m_pPlanetRenderObject;
        EnergyFieldRenderObject* m_pEnergyFieldRenderObject;
        TerrainRenderObject* m_pLevelGridRenderObject;
        EnvMapRenderObject* m_pEnvMapRenderObject;

        PostProcessRenderObject m_postProcessRenderObject;

        FadeScreenRenderObject m_fadeScreenRenderObject;
        FadeFrameRenderObject  m_fadeFrameRenderObject;

        StripeRenderObject m_stripeRenderObject;

        vecMeshInstanceRenderObjectCategories m_meshInstances;

        listSpriteRenderObjects m_spriteRenderObjects;
        listSpriteRenderObjects m_spriteTextRenderObjects;
        listLightRenderObjects m_lightRenderObjects;

        mapFontTextRenderObjects m_textRenderObjects;

        listVector3Caches m_vector3Caches;

        mapParticleSystemRenderObjects m_particleSystems;
        
        typedef std::list< ForceFieldRenderObject* > listForceFields;
        listForceFields m_forceFields;

//         CHaloEntity* pHalo;
//         CEnergyFieldEntity* pEnergyField;
                
        bool bRenderParticles;
        bool bRenderDebugData;
        bool bRenderMeshes;
        bool bRenderLevelTerrain;
        bool bRenderLevelBorder;

        LIGHTING_MODE lightingMode;

        float timeElapsedFrame;
        float timeElapsed;

        bool IsValid()
        {
            return m_pCameraRenderObject;
        }

        Matrix4x4 mtxSunShadowTransform;
    };
    
} // End namespace 
#endif // RENDER_QUEUE_H
