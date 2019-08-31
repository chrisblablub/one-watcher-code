#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/Engine.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    CPlayerMgr::CPlayerMgr()
        : m_currentChapter(0)
        , m_currentLevel(0)
        , m_numKills(0)
        , m_strChapterFolder("Chapter1/") {}

    //-----------------------------------------------------------------------------------
    CPlayerMgr::~CPlayerMgr() {}

    //-----------------------------------------------------------------------------------
    void CPlayerMgr::SetCurrentChapterByIndex(unsigned int chapter)
    {
        m_currentChapter = chapter;

        std::stringstream str;
        for (int i = 1; i <= GetNumChapters(); ++i)
        {
            if (m_currentChapter == (i - 1))
                str << "Chapter" << i << "/";
        }

        m_strChapterFolder = str.str();
    }

    //-----------------------------------------------------------------------------------
//     void CPlayerMgr::SetLevelFinishedAndLoadNext(std::string strLevelFilename)
//     {
//         LevelInfo& levelInfo = GetCurrentLevelInfo();
// 
//         if (!gEngine->GetEditor())
//         {
//             if (levelInfo.bCompletesChapter)
//             {
//                 SetCurrentChapterFinished(true);
// 
//                 CLevelSelectionState* pLevelSelectionState = static_cast<CLevelSelectionState*>(gEngine->GetState("LevelSelectionState"));
//                 pLevelSelectionState->SetPlayChapterComplete(true);
//             }
// 
//             unsigned int curLevelIndex = GetCurrentLevelIndex();
// 
//             SetLevelFinishedAndIncrement(GetCurrentChapterIndex(), GetCurrentLevelIndex());
// 
//             Save();
// 
// 
//             if (curLevelIndex == GetCurrentChapter().levels.size() - 1)
//             {
//                 // Last level -> go back to menue state
//                 gEngine->GetLevelLoader().LoadLevel("../Resources/Levels/" + GetChapterFolder() + "menue.env", "LevelSelectionState");
//             }
//             else
//             {
//                 // Load next level
//                 strLevelFilename.erase(remove_if(strLevelFilename.begin(), strLevelFilename.end(), isspace), strLevelFilename.end());
//                 strLevelFilename = "../Resources/Levels/" + strLevelFilename;
// 
//                 gEngine->GetLevelLoader().LoadLevel(strLevelFilename, "GameState");
//             }
//         }
//     }

    //-----------------------------------------------------------------------------------
    void CPlayerMgr::LoadLevel(std::string strLevelFilename)
    {
        unsigned int curLevelIndex = GetCurrentLevelIndex();

        if ((curLevelIndex == GetCurrentChapter().levels.size()) ||
            !GetCurrentLevelInfo().bAccessible ||
            strLevelFilename.empty())
        {
            // Last level -> go back to menue state
            gEngine->GetLevelLoader().LoadLevel("../Resources/Levels/" + GetChapterFolder() + "menue.env", "LevelSelectionState");
        }
        else
        {
            // Load next level
            strLevelFilename.erase(remove_if(strLevelFilename.begin(), strLevelFilename.end(), isspace), strLevelFilename.end());
            strLevelFilename = strLevelFilename;

            gEngine->GetLevelLoader().LoadLevel(strLevelFilename, "GameState");
        }
    }

    //-----------------------------------------------------------------------------------
    void CPlayerMgr::SetLevelFinished(std::queue<unsigned int>& unlockedLevels)
    {
        LevelInfo& levelInfo = GetCurrentLevelInfo();

        if (!gEngine->GetEditor())
        {
            if (levelInfo.bCompletesChapter)
            {
                SetCurrentChapterFinished(true);

                CLevelSelectionState* pLevelSelectionState = static_cast<CLevelSelectionState*>(gEngine->GetState("LevelSelectionState"));
                pLevelSelectionState->SetPlayChapterComplete(true);
            }

            unsigned int curLevelIndex = GetCurrentLevelIndex();

            SetLevelFinishedAndIncrement(GetCurrentChapterIndex(), GetCurrentLevelIndex(), unlockedLevels);

            Save();
        }
    }

    //-----------------------------------------------------------------------------------
    LevelInfo& CPlayerMgr::GetLevelinfo(unsigned int chapter, unsigned int level)
    {
        return m_chapters[chapter].levels[level];
    }

    //-----------------------------------------------------------------------------------
    LevelInfo& CPlayerMgr::GetCurrentLevelInfo()
    {
        return m_chapters[GetCurrentChapterIndex()].levels[GetCurrentLevelIndex()];
    }

    //-----------------------------------------------------------------------------------
    void CPlayerMgr::Save()
    {
#ifdef ENV_PLATFORM_WINDOWS
        std::ofstream ofs(PLAYER_SAVE_FILE, std::ios::binary);
        boost::archive::binary_oarchive oa(ofs);
        oa & *this;
#endif // ENV_PLATFORM_WINDOWS
    }

    //-----------------------------------------------------------------------------------
    void CPlayerMgr::Load()
    {
#ifdef ENV_PLATFORM_WINDOWS
        ENV_LOG_INFO("LOADING PLAYER SAVEGAME");

        std::ifstream ifs(PLAYER_SAVE_FILE, std::ios::binary);
        if (ifs.is_open() && ifs.peek() != std::ifstream::traits_type::eof())
        {
            boost::archive::binary_iarchive ia(ifs);
            ia & *this;

            ifs.close();

            SetCurrentChapterByIndex(m_currentChapter);

            m_chapters[1].levels[0].strFilename = "../Resources/Levels/Chapter2/intro.env";
            m_chapters[2].levels[0].strFilename = "../Resources/Levels/Chapter3/intro.env";
            m_chapters[3].levels[0].strFilename = "../Resources/Levels/Chapter4/intro.env";

            for (unsigned int i = 1; i < 4; ++i)
            {
                m_chapters[i].firstLevelIndex = 0;
                m_chapters[i].levels[0].bAccessible = true;
                m_chapters[i].levels[0].bCutScene = true;
                m_chapters[i].levels[0].bSelectable = true;
            }
        }
        else
        {
            // Create
            Chapter chapterOne;
            chapterOne.strName = "Chapter 1";
            chapterOne.levels.resize(22);
            chapterOne.bAccessible = true;
            chapterOne.firstLevelIndex = 0;

            for (int i = 1; i < chapterOne.levels.size(); ++i)
            {
                std::stringstream str;
                str << "../Resources/Levels/Chapter1/chapter1_level" << std::setfill('0') << std::setw(2) << i << ".env";
                chapterOne.levels[i].strFilename = str.str();
            }
            chapterOne.levels[0].strName = "Prolog";
            chapterOne.levels[0].strFilename = "../Resources/Levels/Chapter1/intro.env";
            chapterOne.levels[0].bAccessible = true;
            chapterOne.levels[0].bCutScene = true;
            chapterOne.levels[1].strName = "1 - JUMP AND RUN";
            chapterOne.levels[1].bAccessible = false;
            chapterOne.levels[2].strName = "2 - YAWNING";
            chapterOne.levels[3].strName = "3 - THE TOWER";
            chapterOne.levels[4].strName = "4 - CLIMBING";
            chapterOne.levels[5].strName = "5 - THE WALL";
            chapterOne.levels[6].strName = "6 - THE GAP";
            chapterOne.levels[7].strName = "7 - LIFTERS";
            chapterOne.levels[8].strName = "8 - SLIDER-MAN";
            chapterOne.levels[9].strName = "9 - MORGENSTERN";
            chapterOne.levels[10].strName = "10 - TRIPPLE SWING";
            chapterOne.levels[11].strName = "11 - SUPER BOWLING";
            chapterOne.levels[12].strName = "12 - CLIFFHANGER";
            chapterOne.levels[13].strName = "13 - DEADLINE";
            chapterOne.levels[14].strName = "14 - LASERLINE";
            chapterOne.levels[15].strName = "15 - YOU BETTER RUN";
            chapterOne.levels[16].strName = "16 - STOP AND GO";
            chapterOne.levels[17].strName = "17 - BLOODY SWING";
            chapterOne.levels[18].strName = "18 - RUN FOR COVER";
            chapterOne.levels[19].strName = "19 PARCOUR";
            chapterOne.levels[20].strName = "20 - BOSS I";
            chapterOne.levels[20].bCompletesChapter = true;
            chapterOne.levels[21].strName = "OUTRO";
            chapterOne.levels[21].bSelectable = false;
            chapterOne.levels[21].bCutScene = true;

            Chapter chapterTwo;
            chapterTwo.strName = "Chapter 2";
            chapterTwo.levels.resize(22);
            chapterTwo.firstLevelIndex = 0;

            for (int i = 0; i < chapterTwo.levels.size(); ++i)
            {
                std::stringstream str;
                str << "../Resources/Levels/Chapter2/chapter2_level" << std::setfill('0') << std::setw(2) << i << ".env";
                chapterTwo.levels[i].strFilename = str.str();
            }

            chapterTwo.levels[0].strName = "Prolog";
            chapterTwo.levels[0].bAccessible = true;
            chapterTwo.levels[0].strFilename = "../Resources/Levels/Chapter2/intro.env";
            chapterTwo.levels[0].bCutScene = true;
            chapterTwo.levels[0].bSelectable = true;
            chapterTwo.levels[1].strName = "21 - HELLO WORLD";
            chapterTwo.levels[2].strName = "22 - SHOOTING STAR";
            chapterTwo.levels[3].strName = "23 - BURNING SKY";
            chapterTwo.levels[4].strName = "24 - FLYING HIGH";
            chapterTwo.levels[5].strName = "25 - TOWER DEFENSE";
            chapterTwo.levels[6].strName = "26 - TRIPPLE X";
            chapterTwo.levels[7].strName = "27 - ACROPHOBIA";
            chapterTwo.levels[8].strName = "28 - HEAT PIPE";
            chapterTwo.levels[9].strName = "29 - THE HUNTER";
            chapterTwo.levels[10].strName = "30 - UPGRADE";
            chapterTwo.levels[11].strName = "31 - GRAVITY";
            chapterTwo.levels[12].strName = "32 - LAUNCHER";
            chapterTwo.levels[13].strName = "33 - UPS AND DOWNS";
            chapterTwo.levels[14].strName = "34 - FIREBALLS";
            chapterTwo.levels[15].strName = "35 - SKYFALL";
            chapterTwo.levels[16].strName = "36- FIRE IN THE HOLE";
            chapterTwo.levels[17].strName = "37 - ELEVATOR";
            chapterTwo.levels[18].strName = "38 - GUN FIRE";
            chapterTwo.levels[19].strName = "39 - DUNGEON";
            chapterTwo.levels[20].strName = "40 - BOSS II";
            chapterTwo.levels[20].bCompletesChapter = true;
            chapterTwo.levels[21].strName = "Outro";
            chapterTwo.levels[21].bSelectable = false;
            chapterTwo.levels[21].bCutScene = false;


            Chapter chapterThree;
            chapterThree.strName = "Chapter 3";
            chapterThree.levels.resize(22);
            chapterThree.firstLevelIndex = 0;

            for (int i = 0; i < chapterThree.levels.size(); ++i)
            {
                std::stringstream str;
                str << "../Resources/Levels/Chapter3/chapter3_level" << std::setfill('0') << std::setw(2) << i << ".env";
                chapterThree.levels[i].strFilename = str.str();
            }

            chapterThree.levels[0].strName = "Prolog";
            chapterThree.levels[0].bAccessible = true;
            chapterThree.levels[0].strFilename = "../Resources/Levels/Chapter3/intro.env";
            chapterThree.levels[0].bCutScene = true;
            chapterThree.levels[0].bSelectable = true;
            chapterThree.levels[1].strName = "41 - BEYOND THE SUN";
            chapterThree.levels[2].strName = "42 - XTRA LARGE";
            chapterThree.levels[3].strName = "43 - BANGBOOMBANG";
            chapterThree.levels[4].strName = "44 - FIRE IN THE HOLE II";
            chapterThree.levels[5].strName = "45 - THE COLD FIST";
            chapterThree.levels[6].strName = "46 - EXPLOSIVES";
            chapterThree.levels[7].strName = "47 - BUMPERS";
            chapterThree.levels[8].strName = "48 - UPSIDE DOWN";
            chapterThree.levels[9].strName = "49 - THE HUNTED";
            chapterThree.levels[10].strName = "50 - BATTLEFIELD";
            chapterThree.levels[11].strName = "51 - FIRING RANGE";
            chapterThree.levels[12].strName = "52 - FIREWIRE";
            chapterThree.levels[13].strName = "53 - ASCENSION";
            chapterThree.levels[14].strName = "54 - BUMPY FLIGHT";
            chapterThree.levels[15].strName = "55 - DOUBLE";
            chapterThree.levels[16].strName = "56 - CASTLE";
            chapterThree.levels[17].strName = "57 - SLOW MOTION";
            chapterThree.levels[18].strName = "58 - A.INTELLIGENCE I";
            chapterThree.levels[19].strName = "59 - A.INTELLIGENCE II";
            chapterThree.levels[20].strName = "60 - BOSS III";
            chapterThree.levels[20].bCompletesChapter = true;
            chapterThree.levels[21].strName = "Outro";
            chapterThree.levels[21].bSelectable = false;
            chapterThree.levels[21].bCutScene = true;


            Chapter chapterFour;
            chapterFour.strName = "Chapter 4";
            chapterFour.levels.resize(8);
            chapterFour.firstLevelIndex = 0;

            for (int i = 0; i < chapterFour.levels.size(); ++i)
            {
                std::stringstream str;
                str << "../Resources/Levels/Chapter4/chapter4_level" << std::setfill('0') << std::setw(2) << i << ".env";
                chapterFour.levels[i].strFilename = str.str();
            }

            chapterFour.levels[0].strName = "Prolog";
            chapterFour.levels[0].bAccessible = true;
            chapterFour.levels[0].strFilename = "../Resources/Levels/Chapter4/intro.env";
            chapterFour.levels[0].bCutScene = true;
            chapterFour.levels[0].bSelectable = true;
            chapterFour.levels[1].strName = "61 - GALAXY";
            chapterFour.levels[2].strName = "62 - PROSPECT";
            chapterFour.levels[3].strName = "63 - UPSIDE DOWN";
            chapterFour.levels[4].strName = "64 - GRAVITATION";
            chapterFour.levels[5].strName = "65 - FINAL";
            chapterFour.levels[6].strName = "66 - THE WATCHER";
            chapterFour.levels[6].bCompletesChapter = true;
            chapterFour.levels[7].strName = "Outro";
            chapterFour.levels[7].bSelectable = false;
            chapterFour.levels[7].bCutScene = true;

            m_chapters.resize(4);
            m_chapters[0] = chapterOne;
            m_chapters[1] = chapterTwo;
            m_chapters[2] = chapterThree;
            m_chapters[3] = chapterFour;

            SetCurrentChapterByIndex(0);
        }

        m_chapters[0].numLevelsUnlockBoss = 18;
        m_chapters[1].numLevelsUnlockBoss = 18;
        m_chapters[2].numLevelsUnlockBoss = 18;
        m_chapters[3].numLevelsUnlockBoss = 5;

        m_chapters[0].levels[20].bBoss = true;
        m_chapters[1].levels[20].bBoss = true;
        m_chapters[2].levels[20].bBoss = true;
        m_chapters[3].levels[5].bBoss = true;

        for (unsigned int k = 0; k < m_chapters.size(); ++k)
            for (unsigned int l = 0; l < m_chapters[k].levels.size(); ++l)
                if (m_chapters[k].levels[l].bCutScene)
                    m_chapters[k].levels[l].bAccessible = true;

        m_numKills = 0;

        for (unsigned int i = 0; i < m_chapters.size(); ++i)
        {
            for (unsigned int j = 0; j < m_chapters[i].levels.size(); ++j)
                m_numKills += m_chapters[i].levels[j].numKills;
        }
        ENV_LOG_INFO("LOADING PLAYER SAVEGAME SUCCEEDED");

#endif // ENV_PLATFORM_WINDOWS
    }

} // env