/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PLAYER_MGR_H
#define PLAYER_MGR_H

#include <Engine/EngineCommon.h>
#include <time.h>

namespace env
{
    struct LevelInfo
    {
        LevelInfo()
            :
#ifdef ENV_FINAL
            bAccessible(false)
#else
            bAccessible(false)
#endif // ENV_FINAL
            , bFinished(false)
            , bCutScene(false)
            , bSelectable(true)
            , bCompletesChapter(false)
            , bBoss(false)
            , numKills(0)
            , bestTime(-1.0f)
            , medal(10000) {}

        std::string strFilename;
        std::string strName;
        bool bAccessible;
        bool bFinished;
        bool bCutScene;
        bool bSelectable;
        bool bCompletesChapter;
        bool bBoss;
        float bestTime;
        unsigned int numKills;
        int medal;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & strFilename;
            ar & strName;
            ar & bAccessible;
            ar & bFinished;
            ar & numKills;
            ar & bestTime;
            ar & medal;
            ar & bCutScene;
            ar & bSelectable;
            ar & bCompletesChapter;
        }
    };

    struct Chapter
    {
        Chapter()
            : bFinished(false)
            , bAccessible(true)
            , firstLevelIndex(0)
            , numLevelsUnlockBoss(18) {}

        bool bFinished;
        bool bAccessible;

        unsigned int firstLevelIndex;
        unsigned int numLevelsUnlockBoss;

        std::string strName;
        std::vector< LevelInfo > levels;

        unsigned int GetNumLevelsInMenue() const
        {
            unsigned int numLevels = 0;

            for (unsigned int i = 0; i < levels.size(); ++i)
            {
                if (levels[i].bSelectable)
                    ++numLevels;
            }

            return numLevels;
        }

        unsigned int GetNextHigherSelectableLevelIndex(unsigned int i) const
        {
            unsigned int k = i;

            while (++k != i)
            {
                if (k >= levels.size())
                    k = 0;

                if (levels[k].bSelectable)
                    break;
            }

            return k;
        }

        unsigned int GetNextHigherLevelIndex(unsigned int i) const
        {
            return (i + 1) % levels.size();
        }

        unsigned int GetNextLowerSelectableLevelIndex(unsigned int i) const
        {
            int k = i;

            while (--k != i)
            {
                if (k < 0)
                    k = levels.size() - 1;

                if (levels[k].bSelectable)
                    break;
            }

            return k;
        }


        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & bFinished;
            ar & strName;
            ar & levels;
            ar & bAccessible;
            ar & firstLevelIndex;
        }
    };

    const std::string PLAYER_SAVE_FILE = "save";

    class CPlayerMgr
    {
        friend class CSingletonStatic< CPlayerMgr >;
    private:
        CPlayerMgr();
    public:
        ~CPlayerMgr();

        void Save();
        void Load();

        LevelInfo& GetLevelinfo(unsigned int chapter, unsigned int level);
        LevelInfo& GetCurrentLevelInfo();

        unsigned int GetNumChapters() const { return m_chapters.size(); }
        unsigned int GetNumLevels(int chapter) const { return m_chapters.size() > chapter ? m_chapters[chapter].levels.size() : 0; }

        Chapter& GetChapter(unsigned int chapter) { return m_chapters[chapter]; }
        Chapter& GetCurrentChapter() { return m_chapters[GetCurrentChapterIndex()]; }

        void SetCurrentChapterFinished(bool bFinished) { m_chapters[m_currentChapter].bFinished = bFinished; }
        void SetCurrentLevelFinished(bool bFinished) { m_chapters[m_currentChapter].levels[m_currentLevel].bFinished = bFinished; }

        unsigned int GetCurrentChapterIndex() const { return m_currentChapter; }
        void SetCurrentChapterByIndex(unsigned int chapter);
        void SetCurrentLevelByIndex(unsigned int level) { m_currentLevel = level; }
        unsigned int GetCurrentLevelIndex() const { return m_currentLevel; }

        void IncrementNumKills()
        {
            m_numKills++;

            GetCurrentLevelInfo().numKills++;

            Save();
        }
        unsigned int GetNumKills() const { return m_numKills; }

        const std::string& GetChapterFolder() const { return m_strChapterFolder; }

//         void SetLevelFinishedAndLoadNext(std::string strLevelFilename);
        void SetLevelFinished(std::queue<unsigned int>& unlockedLevels = std::queue<unsigned int>());
        void LoadLevel(std::string strLevelFilename);
    private:

        void SetLevelAccessible(unsigned int chapterIndex, unsigned int levelIndex, std::queue<unsigned int>& unlockedLevels)
        {
            Chapter& chapter = GetChapter(chapterIndex);

            if (levelIndex < chapter.GetNumLevelsInMenue() &&
                !chapter.levels[levelIndex].bAccessible)
            {
                if (GetLevelinfo(chapterIndex, levelIndex).bBoss)
                {
                    int numLevelsFinished = 0;
                    for (unsigned int i = chapter.firstLevelIndex; i < chapter.GetNumLevelsInMenue(); ++i)
                    {
                        if (GetLevelinfo(chapterIndex, i).bFinished)
                            ++numLevelsFinished;
                    }

                    if (numLevelsFinished >= chapter.numLevelsUnlockBoss)
                    {
                        unlockedLevels.push(levelIndex);

                        chapter.levels[levelIndex].bAccessible = true;
                    }
                }
                else
                {
                    unlockedLevels.push(levelIndex);

                    chapter.levels[levelIndex].bAccessible = true;
                }
            }
        }

        void SetLevelFinishedAndIncrement(unsigned int chapterIndex, unsigned int levelIndex, std::queue<unsigned int>& unlockedLevels)
        {
            Chapter& chapter = GetChapter(chapterIndex);

            bool bossFinished = false;

            if (chapter.levels[levelIndex].bCompletesChapter &&
                chapter.levels[levelIndex].bFinished == false)
            {
                bossFinished = true;
            }

            chapter.levels[levelIndex].bFinished = true;

            if (chapter.GetNextHigherLevelIndex(levelIndex) > levelIndex)
                m_currentLevel++;

            SetLevelAccessible(chapterIndex, ++levelIndex, unlockedLevels);
            SetLevelAccessible(chapterIndex, ++levelIndex, unlockedLevels);
            SetLevelAccessible(chapterIndex, chapter.GetNumLevelsInMenue() - 1, unlockedLevels);

            int numberOfLevelBeaten = 0;

            for (int i = 0; i < chapter.levels.size(); ++i)
            {
                if (chapter.levels[i].bCutScene)
                {
                    continue;
                }

                if (chapter.levels[i].bFinished == false)
                {
                    continue;
                }

                ++numberOfLevelBeaten;
            }

            gEngine->TriggerChapterProgressCallback(chapterIndex, numberOfLevelBeaten, bossFinished);
        }


        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_chapters;
            ar & m_currentChapter;

            if (version > 1)
                ar & m_numKills;
        }

    private:
        std::vector< Chapter > m_chapters;

        unsigned int m_currentChapter;
        unsigned int m_currentLevel;
        unsigned int m_numKills;

        std::string m_strChapterFolder;
    };
    typedef CSingletonStatic< CPlayerMgr > CSingletonPlayerMgr;

} // env

#endif // PLAYER_MGR_H