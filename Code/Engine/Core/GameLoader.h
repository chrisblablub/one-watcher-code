/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GAME_LOADER_H
#define GAME_LOADER_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/Sprite.h>
#include <Engine/Scene/SceneNode.h>

#include <Graphic/Core/RenderQueue.h>

namespace env
{
    enum LOADER_STATE
    {
        LOADER_STATE_INACTIVE = 0,
        LOADER_STATE_LOADING,
        LOADER_STATE_FIRSTFRAME,
        LOADER_STATE_SECONDFRAME
    };

    const float MINIMUM_LEVEL_LOADING_TIME = 0.4f;

    class CLevelLoader
    {
    public:
        static void LoadingThread(void * aArg);

        CLevelLoader();
        ~CLevelLoader();

        void Initialize();

        //void CloseLoadingThread();

        LOADER_STATE Update(float dt, RenderQueue& renderQueue);
        void PrepareLoadingScreen(float dt, RenderQueue& renderQueue);

        void LoadLevel(const std::string& strFilename, const std::string& strNewState = std::string());
        void LoadLevel(int numGridsX, int numGridsY);

        //void SetCloseThread();

        //bool GetCloseThread() const;

        //void SetThreadClosed(bool bSet);

        //bool GetThreadClosed() const;

        void SetLevelLoaded(bool bSet);

        bool GetLevelLoaded() const;

        void SetLevelFilename(const std::string& strFilename);

        const std::string& GetLevelFilename() const;

        const std::string& GetNewState() const;

        void SetState(LOADER_STATE state);
        LOADER_STATE GetState();

        void SetStripeRenderObject(StripeRenderObject obj) { m_stripeRenderObject = obj; }

    private:
        

        bool m_bCloseThread;
        bool m_bThreadClosed;
        bool m_bLoadLevel;
        bool m_bLevelLoaded;

        float m_currentLoadingTime;
        float m_loadingSpriteParam;

        std::string m_strLevelFilename;
        std::string m_strNewState;

        StripeRenderObject m_stripeRenderObject;

        ENV_PLATFORM_WINDOWS_ONLY(Handle m_hLoadingThread;);

        LOADER_STATE m_state;

//         CSprite m_loadingSprite;
//         CSceneNode m_loadingSpriteNode;

    };
} // env

#endif // GAME_LOADER_H