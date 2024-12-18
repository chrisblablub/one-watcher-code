#include <OneWatcher/Core/SteamHandler.h>

#ifdef ENV_FINAL
const char* const SteamHandler::STAT_PROGRESS_CHAPTER_1 = "p_c1";
const char* const SteamHandler::STAT_PROGRESS_CHAPTER_2 = "p_c2";
const char* const SteamHandler::STAT_PROGRESS_CHAPTER_3 = "p_c3";
const char* const SteamHandler::STAT_PROGRESS_CHAPTER_4 = "p_c4";
const char* const SteamHandler::STAT_DEATH_COUNTER = "death";

const char* const SteamHandler::ACHIEVEMENT_FINISHED_CHAPTER_1 = "FINISHED_CHAPTER_1";
const char* const SteamHandler::ACHIEVEMENT_FINISHED_CHAPTER_2 = "FINISHED_CHAPTER_2";
const char* const SteamHandler::ACHIEVEMENT_FINISHED_CHAPTER_3 = "FINISHED_CHAPTER_3";
const char* const SteamHandler::ACHIEVEMENT_FINISHED_CHAPTER_4 = "FINISHED_CHAPTER_4";

bool SteamHandler::Init()
{
#ifdef ENV_FINAL
    if (SteamAPI_RestartAppIfNecessary(STEAM_APP_ID))
    {
        return false;
    }

    if (!SteamAPI_Init())
    {
        return false;
    }
#endif
    return true;
}

void SteamHandler::Update()
{
#ifdef ENV_FINAL
    SteamAPI_RunCallbacks();
    bool result = SteamUserStats()->RequestCurrentStats();
#endif
}

void SteamHandler::OnUserStatsReceived(UserStatsReceived_t* callback)
{
#ifdef ENV_FINAL
    EResult requestResult = callback->m_eResult;

    if (requestResult != EResult::k_EResultOK)
    {
        return;
    }

    int gameID = callback->m_nGameID;

    if (gameID != STEAM_APP_ID)
    {
        return;
    }
#endif
}

void SteamHandler::OnUserStatsStored(UserStatsStored_t* callback)
{
#ifdef ENV_FINAL
    EResult requestResult = callback->m_eResult;

    if (requestResult != EResult::k_EResultOK)
    {
        return;
    }

    int gameID = callback->m_nGameID;

    if (gameID != STEAM_APP_ID)
    {
        return;
    }
#endif
}

void SteamHandler::Shutdown()
{
#ifdef ENV_FINAL
    SteamAPI_Shutdown();
#endif
}

void SteamHandler::IncreaseStat(const char* const key)
{
#ifdef ENV_FINAL
    int currentValue = 0;
    CSteamID userID = SteamUser()->GetSteamID();

    bool result = SteamUserStats()->GetUserStat(userID, key, &currentValue);

    if (result == false)
    {
        return;
    }

    SetStat(key, ++currentValue);
#endif
}

void SteamHandler::SetAchievement(const char* id)
{
#ifdef ENV_FINAL
    SteamUserStats()->SetAchievement(id);
    StoreStats();
#endif
}

void SteamHandler::SetStat(const char* const key, const int value)
{
#ifdef ENV_FINAL
    SteamUserStats()->SetStat(key, value);
    StoreStats();
#endif
}

void SteamHandler::StoreStats()
{
#ifdef ENV_FINAL
    SteamUserStats()->StoreStats();
#endif
}

void SteamHandler::IncreaseStat_DeathCounter()
{
#ifdef ENV_FINAL
    IncreaseStat(STAT_DEATH_COUNTER);
#endif
}

void SteamHandler::SetStat_ChapterProgress(const int chapter, const int levelBested, const bool bossFinished)
{
#ifdef ENV_FINAL
    switch (chapter)
    {
    case 0:
        SetStat(STAT_PROGRESS_CHAPTER_1, levelBested);

        if (bossFinished)
            SetAchievement(ACHIEVEMENT_FINISHED_CHAPTER_1);

        break;
    case 1:
        SetStat(STAT_PROGRESS_CHAPTER_2, levelBested);

        if (bossFinished)
            SetAchievement(ACHIEVEMENT_FINISHED_CHAPTER_2);

        break;
    case 2:
        SetStat(STAT_PROGRESS_CHAPTER_3, levelBested);

        if (bossFinished)
            SetAchievement(ACHIEVEMENT_FINISHED_CHAPTER_3);

        break;
    case 3:
        SetStat(STAT_PROGRESS_CHAPTER_4, levelBested);

        if (bossFinished)
            SetAchievement(ACHIEVEMENT_FINISHED_CHAPTER_4);

        break;
    default:
        break;
    }
#endif
}

#endif