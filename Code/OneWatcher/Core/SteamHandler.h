#pragma once
#ifndef STEAM_HANDLER_H
#define STEAM_HANDLER_H

#ifdef ENV_FINAL

#include <steam_api.h>
#include <isteamuserstats.h>


class SteamHandler
{
public:
    static const char* const STAT_PROGRESS_CHAPTER_1;
    static const char* const STAT_PROGRESS_CHAPTER_2;
    static const char* const STAT_PROGRESS_CHAPTER_3;
    static const char* const STAT_PROGRESS_CHAPTER_4;
    static const char* const STAT_DEATH_COUNTER;
    static const char* const ACHIEVEMENT_FINISHED_CHAPTER_1;
    static const char* const ACHIEVEMENT_FINISHED_CHAPTER_2;
    static const char* const ACHIEVEMENT_FINISHED_CHAPTER_3;
    static const char* const ACHIEVEMENT_FINISHED_CHAPTER_4;

    STEAM_CALLBACK(SteamHandler, OnUserStatsReceived, UserStatsReceived_t);
    STEAM_CALLBACK(SteamHandler, OnUserStatsStored, UserStatsStored_t);

    static bool Init();
    static void Shutdown();
    static void Update();

    static void SetAchievement(const char* id);
    static void SetStat(const char* const, const int);
    static void IncreaseStat(const char* const);
    static void StoreStats();

    static void IncreaseStat_DeathCounter();
    static void SetStat_ChapterProgress(const int chapter, const int levelBested, const bool bossFinished);
};
#endif
#endif