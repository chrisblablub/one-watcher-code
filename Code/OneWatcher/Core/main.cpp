#include <Engine/Core/Engine.h>
#include <OneWatcher/Core/SteamHandler.h>

const unsigned int STEAM_APP_ID = 727150;
const int STEAM_RETURN_CODE = 100;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef ENV_FINAL
    if (SteamHandler::Init() == false)
    {
        return STEAM_RETURN_CODE;
    }
#endif

    {
        //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

        env::CEngine engine;

        env::EngineSetup setup;
        setup.hInstance = hInstance;

        if (!engine.Init(setup))
        {
            MessageBox(NULL, "One Watcher failed to initialize. Please check your system specs. If the problem persists, contact us for help.", "One Watcher has crashed", MB_OK);
            return 0;
        }

#ifdef ENV_FINAL
        SteamHandler steamHandler; // required to activate callbacks
        engine.SetOnDeathCallback(SteamHandler::IncreaseStat_DeathCounter);
        engine.SetOnChapterProgressCallback(SteamHandler::SetStat_ChapterProgress);
        engine.SetEndOfFrameCallback(SteamHandler::Update);
#endif
        engine.SetPaused(false);
        engine.Run();
    }

    //_CrtDumpMemoryLeaks();

    //#ifdef ENV_FINAL
    SteamHandler::Shutdown();
    //#endif

    return 0;
}