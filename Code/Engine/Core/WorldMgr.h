/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef WORLDMGR_H
#define WORLDMGR_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/Camera.h>
#include <Engine/Entity/EntitySlider.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Scene/TerrainMgr.h>

namespace env
{
    struct RenderQueue;

    class CWorldMgr
    {

    public:
        CWorldMgr();
        CWorldMgr(const CWorldMgr&) = delete;
        CWorldMgr& operator=(const CWorldMgr&) = delete;
        ~CWorldMgr();

        void Update(float time, float dt, RenderQueue& renderQueue);
        void Load(const std::string& strLevelFilename, bool bGameMode);
        void InitializeAtLoad();

        void Load(int numGridsX, int numGridsY, bool bGameMode);
        void InitializeAtCreation();

        void Unload();

        void OnLostDevice();
        void OnResetDevice();

        void SetGameMode(bool bGameMode);


        bool GetGameMode() const { return m_bGameMode; }


        CEntitySlider& GetEntitySlider() { return m_entitySlider; }
        
        CScenario* GetScenario() const { return m_pScenario; }
        
        void SetActiveCamera(CCamera* pCam);

        CCamera* GetDefaultCamera() const { return m_pDefaultCamera; }
        CCamera* GetActiveCamera() const { return m_pActiveCamera; }
        
        CTerrainMgr* GetLevelMgr() const { return m_pTerrainMgr; }

        bool GetLevelFirstRun() const { return m_bLevelFirstRun; }

        void SetTimeMultiplier(float timeMultiplier) { m_timeMultiplier = timeMultiplier; }
        float GetLevelTime() const { return m_levelTime; }

        bool GetLevelJustStarted() const { return m_bLevelJustStarted; }

        void SaveMap(const std::string& strMapFilepath);

        void SetPaused(bool bPause = true);
        bool GetPaused() const { return m_bPaused; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & m_pScenario;
            ar & m_entitySlider;
            ar & m_pTerrainMgr;
            ar & m_version;
        }

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_pScenario;
            ar & m_entitySlider;
            ar & m_pTerrainMgr;
            ar & m_version;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

        
        void LoadMap(const std::string& strLevelFilename);
        void RegisterEntityTypes();
        void RegisterParticleSystemTypes();

    private:
        bool m_bRunning;
        bool m_bPaused;
        bool m_bGameMode;
        
        bool m_bLevelJustStarted;
        bool m_bLevelFirstRun;
        bool m_bGameModeChanged;

        float m_timeMultiplier;

        unsigned int m_version;

        
        float m_levelTime;

        CEntitySlider m_entitySlider;

        CScenario* m_pScenario;

        CTerrainMgr* m_pTerrainMgr;

        CCamera* m_pDefaultCamera;
        CCamera* m_pActiveCamera;

        std::list< std::string > m_registeredMeshes;

    };

} // env

#endif // WORLDMGR_H
