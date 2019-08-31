#include <OneWatcher/Core/SteamHandler.h>


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
    if (SteamAPI_RestartAppIfNecessary(STEAM_APP_ID))
    {
        return false;
    }

    if (!SteamAPI_Init())
    {
        return false;
    }
    return true;
}

void SteamHandler::Update()
{
    SteamAPI_RunCallbacks();
    bool result = SteamUserStats()->RequestCurrentStats();
}

void SteamHandler::OnUserStatsReceived(UserStatsReceived_t* callback)
{
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
}

void SteamHandler::OnUserStatsStored(UserStatsStored_t* callback)
{
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
}

void SteamHandler::Shutdown()
{
    SteamAPI_Shutdown();
}

void SteamHandler::IncreaseStat(const char* const key)
{
    int currentValue = 0;
    CSteamID userID = SteamUser()->GetSteamID();

    bool result = SteamUserStats()->GetUserStat(userID, key, &currentValue);

    if (result == false)
    {
        return;
    }

    SetStat(key, ++currentValue);
}

void SteamHandler::SetAchievement(const char* id)
{
    SteamUserStats()->SetAchievement(id);
    StoreStats();
}

void SteamHandler::SetStat(const char* const key, const int value)
{
    SteamUserStats()->SetStat(key, value);
    StoreStats();
}

void SteamHandler::StoreStats()
{
    SteamUserStats()->StoreStats();
}

void SteamHandler::IncreaseStat_DeathCounter()
{
    IncreaseStat(STAT_DEATH_COUNTER);
}

void SteamHandler::SetStat_ChapterProgress(const int chapter, const int levelBested, const bool bossFinished)
{
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
}