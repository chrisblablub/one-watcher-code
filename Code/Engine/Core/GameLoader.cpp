#include <Engine/Core/GameLoader.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Scene/SceneMgr.h>


#ifdef ENV_PLATFORM_ORBIS
namespace tthread
{
    struct mutex {};
    struct thread {}; 
}
#else
#include <Engine/Thread/TinyThread/tinythread.h>
#endif // ENV_PLATFORM_ORBIS

namespace env
{
    tthread::mutex mMutex;
    tthread::thread* m_pThread = nullptr;

    //-----------------------------------------------------------------------------------
    CLevelLoader::CLevelLoader()
        : m_bCloseThread(false)
        , m_bThreadClosed(false)
        , m_bLoadLevel(false)
        , m_bLevelLoaded(false)
        , m_state(LOADER_STATE_INACTIVE)
        , m_loadingSpriteParam(0.0f)
        //, m_pThread(NULL)
    {
        ENV_LOG_INFO("CREATING LOADING THREAD");

//         ENV_PLATFORM_WINDOWS_ONLY(m_pThread = new tthread::thread(CLevelLoader::LoadingThread, this););
// 
//         ENV_PLATFORM_WINDOWS_ONLY(m_hLoadingThread = m_pThread->native_handle(););
    }

    //-----------------------------------------------------------------------------------
    CLevelLoader::~CLevelLoader()
    {
//         CloseLoadingThread();
// 
//         ENV_DELETE(m_pThread);
    }

    //-----------------------------------------------------------------------------------
    void CLevelLoader::Initialize()
    {
//         m_loadingSprite.SetMaterialFilename("GUI/loading.png");
//         m_loadingSprite.SetScreenSpace(true);
//         m_loadingSprite.AttachNode(&m_loadingSpriteNode);
    }

    //-----------------------------------------------------------------------------------
//     void CLevelLoader::CloseLoadingThread()
//     {
//         ENV_LOG_INFO("CLOSING LOADING THREAD");
//         //ENV_ASSERT_DBG(m_pThread->joinable());
// 
//         SetCloseThread();
// 
//         ENV_PLATFORM_WINDOWS_ONLY(m_pThread->join(););
// 
//         ENV_LOG_INFO("LEVELLOADER CLOSED");
//     }

    //-----------------------------------------------------------------------------------
//     void CLevelLoader::LoadingThread(void * aArg)
//     {
//         CLevelLoader* p = static_cast<CLevelLoader*>(aArg);
// 
//         while (!p->GetCloseThread())
//         {
// 
//         }
//         ENV_LOG_INFO("LOADING THRAD CLOSED");
//     }

    //-----------------------------------------------------------------------------------
    void CLevelLoader::SetState(LOADER_STATE state)
    {
//        ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););

        m_state = state;
    }

    //-----------------------------------------------------------------------------------
    LOADER_STATE CLevelLoader::GetState()
    {
 //       ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););

        return m_state;
    }

    //-----------------------------------------------------------------------------------
    void CLevelLoader::PrepareLoadingScreen(float dt, RenderQueue& renderQueue)
    {
//         float scaling = m_loadingSpriteParam / (MINIMUM_LEVEL_LOADING_TIME * 0.5f);
// 
//         CQuaternion quat = m_loadingSpriteNode.GetOrientation();
//         quat.AddRoll(dt * 5.0f);
//         m_loadingSpriteNode.SetOrientation(quat);
//         m_loadingSpriteNode.SetPosition(Vec3(0.5f, 0.9f, 0.0f));
//         m_loadingSpriteNode.SetScale(Vec3(scaling * 1.2f, scaling * 1.2f, 1.0f));
// 
//         renderQueue.m_spriteRenderObjects.push_back(m_loadingSprite.GetRenderObject());
//         renderQueue.m_stripeRenderObject = m_stripeRenderObject;
    }

    //-----------------------------------------------------------------------------------
    LOADER_STATE CLevelLoader::Update(float dt, RenderQueue& renderQueue)
    {
        LOADER_STATE returnState = LOADER_STATE_INACTIVE;

        if (m_bLoadLevel)
        {
//             std::string prevState = gEngine->GetCurrentState() == nullptr ? std::string() : gEngine->GetCurrentState()->GetStateName();
//             gEngine->ChangeState(prevState);

            gEngine->GetWorldMgr()->Unload();
            gEngine->GetSceneMgr()->Reset(false);

            gEngine->GetWorldMgr()->Load(m_strLevelFilename, true);
            gEngine->GetWorldMgr()->InitializeAtLoad();
            //gEngine->GetWorldMgr()->Update(0.0f, 0.0f, renderQueue);

            gRenderSystem->OnLevelChanged();

            gEngine->ChangeState(m_strNewState);

            SetLevelLoaded(true);
//             SetState(LOADER_STATE_LOADING);
// 
//             returnState = LOADER_STATE_LOADING;
// 
//             m_currentLoadingTime += dt;
//             m_loadingSpriteParam = std::min(m_loadingSpriteParam + dt, MINIMUM_LEVEL_LOADING_TIME * 0.5f);

            m_bLoadLevel = false;

            return LOADER_STATE_INACTIVE;
        }
        else if (GetState() == LOADER_STATE_LOADING)
        {
            returnState = LOADER_STATE_LOADING;

            PrepareLoadingScreen(dt, renderQueue);

            m_currentLoadingTime += dt;
            m_loadingSpriteParam = std::min(m_loadingSpriteParam + dt, MINIMUM_LEVEL_LOADING_TIME * 0.5f);
        }
        else if (GetState() == LOADER_STATE_FIRSTFRAME)
        {
            m_currentLoadingTime += dt;
            if (m_currentLoadingTime <= (MINIMUM_LEVEL_LOADING_TIME * 0.5f))
                m_loadingSpriteParam = std::min(m_loadingSpriteParam + dt, MINIMUM_LEVEL_LOADING_TIME * 0.5f);
            else
                m_loadingSpriteParam = std::max(m_loadingSpriteParam - dt, 0.0f);

            //SuspendThread(m_hLoadingThread);

            if (m_loadingSpriteParam > 0.0f)
            {
                PrepareLoadingScreen(dt, renderQueue);

                returnState = LOADER_STATE_LOADING;
            }
            else
            {
                returnState = LOADER_STATE_FIRSTFRAME;
                SetLevelLoaded(true);

                SetState(LOADER_STATE_SECONDFRAME);
            }
        }
        else if (GetState() == LOADER_STATE_SECONDFRAME)
        {
            returnState = LOADER_STATE_SECONDFRAME;

            SetState(LOADER_STATE_INACTIVE);
        }

        m_bLoadLevel = false;

        return returnState;
    }

    //-----------------------------------------------------------------------------------
    void CLevelLoader::LoadLevel(const std::string& strFilename, const std::string& strNewState)
    {
        m_strNewState = strNewState;
        m_strLevelFilename = strFilename;
        m_currentLoadingTime = 0.0f;
        m_loadingSpriteParam = 0.0f;

        m_bLoadLevel = true;
        m_bLevelLoaded = false;


        ENV_LOG_INFO("LOADING LEVEL");
    }

    //-----------------------------------------------------------------------------------
    void CLevelLoader::LoadLevel(int numGridsX, int numGridsY)
    {
        bool bEditor = gEngine->GetEditor();

        gEngine->GetWorldMgr()->Unload();
        gEngine->GetWorldMgr()->Load(numGridsX, numGridsY, !bEditor);

        m_strLevelFilename.clear();
        m_strNewState = "GameState";
        m_currentLoadingTime = 0.0f;
        m_loadingSpriteParam = 0.0f;

        SetState(LOADER_STATE_SECONDFRAME);

        m_bLoadLevel = false;
        m_bLevelLoaded = true;
    }

    //-----------------------------------------------------------------------------------
//     void CLevelLoader::SetCloseThread()
//     {
//         ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););
// 
//         m_bCloseThread = true;
//     }

    //-----------------------------------------------------------------------------------
//     bool CLevelLoader::GetCloseThread() const
//     {
//         ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););
// 
//         return m_bCloseThread;
//     }

    //-----------------------------------------------------------------------------------
//     void CLevelLoader::SetThreadClosed(bool bSet)
//     {
//         ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););
// 
//         m_bThreadClosed = bSet;
//     }

    //-----------------------------------------------------------------------------------
//     bool CLevelLoader::GetThreadClosed() const
//     {
//         ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););
// 
//         return m_bThreadClosed;
//     }

    //-----------------------------------------------------------------------------------
    void CLevelLoader::SetLevelLoaded(bool bSet)
    {
        m_bLevelLoaded = bSet;
    }

    //-----------------------------------------------------------------------------------
    bool CLevelLoader::GetLevelLoaded() const
    {
        return m_bLevelLoaded;
    }

    //-----------------------------------------------------------------------------------
    void CLevelLoader::SetLevelFilename(const std::string& strFilename)
    {
//        ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););
        m_strLevelFilename = strFilename;
    }

    //-----------------------------------------------------------------------------------
    const std::string& CLevelLoader::GetLevelFilename() const
    {
//        ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););
        return m_strLevelFilename;
    }

    //-----------------------------------------------------------------------------------
    const std::string& CLevelLoader::GetNewState() const
    {
 //       ENV_PLATFORM_WINDOWS_ONLY(tthread::lock_guard<tthread::mutex>(mMutex););
        return m_strNewState;
    }
} // env