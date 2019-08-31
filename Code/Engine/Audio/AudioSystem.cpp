#include "AudioSystem.h"
#include <Engine/Core/Engine.h>
#include <Common/Core/FilePackageManager.h>


#if 1

#include "fmod.hpp"
#include "fmod_errors.h"

namespace env
{

    //-----------------------------------------------------------------------------------
    CSound::CSound()
        : m_pSound(nullptr)
        , m_pSystem(nullptr)
        , m_pChannel(nullptr)
        , m_frequency(0.0f)
        , m_speed(1.0f)
        , m_loopCount(-1)
        , m_volume(1.0f)
        , m_targetVolume(1.0f)
        , m_channelGroup(AUDIO_CHANNELGROUP_MUSIC) {}

    //-----------------------------------------------------------------------------------
    CSound::~CSound()
    {
        if (m_pSound)
            m_pSound->release();
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetChannelGroup(AUDIO_CHANNELGROUP channelGroup)
    {
        m_channelGroup = channelGroup;
    }

    //-----------------------------------------------------------------------------------
    bool CSound::IsPlayingMultiChannelSound(const int id)
    {
        FMOD::Channel* currentChannel = m_ChannelMap[id];

        if (currentChannel == NULL)
        {
            return false;
        }

        bool bIsPlaying = false;
        currentChannel->isPlaying(&bIsPlaying);

        return bIsPlaying;
    }

    //-----------------------------------------------------------------------------------
    void CSound::StopMultiChannelSound(const int id)
    {
        FMOD::Channel* currentChannel = m_ChannelMap[id];

        if (currentChannel == NULL)
        {
            return;
        }

        currentChannel->stop();
    }

    //-----------------------------------------------------------------------------------
    void CSound::PlayMultiChannelSound(const int id, const bool isAutoplay)
    {
        FMOD::Channel* currentChannel = m_ChannelMap[id];

        if (currentChannel != NULL)
        {
            bool bIsPlaying = false;
            currentChannel->isPlaying(&bIsPlaying);

            if (bIsPlaying)
            {
                currentChannel->stop();
            }
        }

        m_pSound->setLoopCount(isAutoplay ? -1 : 0);
        m_pSound->setMode(FMOD_LOOP_NORMAL);
        m_pSystem->playSound(m_pSound, gEngine->GetAudioSystem()->GetChannelGroup(m_channelGroup), false, &currentChannel);

		unsigned long long parentclock;
		currentChannel->getDSPClock(NULL, &parentclock);
		currentChannel->addFadePoint(parentclock, 0.0f);
		currentChannel->addFadePoint(parentclock + 4096, m_volume);

        currentChannel->setLoopCount(isAutoplay ? -1 : 0);
        currentChannel->setMode(FMOD_LOOP_NORMAL);
        currentChannel->setPitch(1.0);

        currentChannel->getFrequency(&m_frequency);
        currentChannel->setFrequency(m_frequency * m_speed);
        currentChannel->setVolume(m_volume);

        m_ChannelMap[id] = currentChannel;
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetMultiChannelVolume(const int channelID, const float volume)
    {
        FMOD::Channel* selectedChannel = m_ChannelMap[channelID];

        if (selectedChannel != NULL)
        {
            selectedChannel->setVolume(volume);
        }
    }

    //-----------------------------------------------------------------------------------
    void CSound::PlaySound(int loopCount)
    {
        if (IsPlaying())
            m_pChannel->stop();

        if (!m_pSound)
            return;

        m_loopCount = loopCount;

        m_pSound->setLoopCount(loopCount);
        m_pSound->setMode(FMOD_LOOP_NORMAL);
        m_pSystem->playSound(m_pSound, gEngine->GetAudioSystem()->GetChannelGroup(m_channelGroup), false, &m_pChannel);

        m_pChannel->setLoopCount(loopCount);
        m_pChannel->setMode(FMOD_LOOP_NORMAL);
        m_pChannel->setPitch(1.0);
        //m_pChannel->setPan(2);

        m_pChannel->getFrequency(&m_frequency);
        m_pChannel->setFrequency(m_frequency * m_speed);
        m_pChannel->setVolume(m_volume);
    }

    //-----------------------------------------------------------------------------------
    void CSound::PlaySound(AUDIO_CHANNELGROUP channelGroup, int loopCount)
    {
        if (IsPlaying())
            m_pChannel->stop();

        if (!m_pSound)
            return;

        m_loopCount = loopCount;

        m_pSound->setLoopCount(loopCount);
        m_pSystem->playSound(m_pSound, gEngine->GetAudioSystem()->GetChannelGroup(channelGroup), false, &m_pChannel);

        m_pChannel->setLoopCount(loopCount);
        m_pChannel->setMode(FMOD_LOOP_NORMAL);
        m_pChannel->getFrequency(&m_frequency);
        m_pChannel->setFrequency(m_frequency * m_speed);
        m_pChannel->setVolume(m_volume);
    }

    //-----------------------------------------------------------------------------------
    void CSound::StopSound()
    {
        m_pChannel->stop();
    }

    //-----------------------------------------------------------------------------------
    bool CSound::IsPlaying()
    {
        bool bIsPlaying = false;
        m_pChannel->isPlaying(&bIsPlaying);

        return bIsPlaying;
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetVolume(float vol)
    {
        if (m_pChannel)
            m_pChannel->setVolume(vol);

        m_volume = vol;
        m_targetVolume = vol;
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetSpeed(float speed)
    {
        m_speed = speed;

        if (m_pChannel)
            m_pChannel->setFrequency(m_frequency * m_speed);
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetFadeDuration(float duration, float targetVolume)
    {
        m_targetVolume = targetVolume;

        m_pChannel->getFrequency(&m_frequency);

        unsigned long long parentclock;
        m_pChannel->getDSPClock(NULL, &parentclock);

        float currentVolume = 0.0f;
        m_pChannel->getVolume(&currentVolume);
        m_pChannel->addFadePoint(parentclock, currentVolume);
        m_pChannel->addFadePoint(parentclock + m_frequency * duration, targetVolume);

        if (Equals(targetVolume, 0.0f))
            m_pChannel->setDelay(0, parentclock + m_frequency * duration, true);
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetFadeDuration(float duration, float startVolume, float targetVolume)
    {
        m_targetVolume = targetVolume;

        m_pChannel->getFrequency(&m_frequency);

        unsigned long long parentclock;
        m_pChannel->getDSPClock(NULL, &parentclock);
        m_pChannel->addFadePoint(parentclock, startVolume);
        m_pChannel->addFadePoint(parentclock + m_frequency * duration, targetVolume);

        if (Equals(targetVolume, 0.0f))
            m_pChannel->setDelay(0, parentclock + m_frequency * duration, true);
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetFade(float timeStart, float timeEnd, float targetVolume)
    {
        m_targetVolume = targetVolume;

        m_pChannel->getFrequency(&m_frequency);

        float currentVolume = 0.0f;
        m_pChannel->getVolume(&currentVolume);

        m_pChannel->addFadePoint(m_frequency * timeStart, currentVolume);
        m_pChannel->addFadePoint(m_frequency * timeEnd, targetVolume);

        if (Equals(targetVolume, 0.0f))
            m_pChannel->setDelay(0, m_frequency * timeEnd, true);
    }

    //-----------------------------------------------------------------------------------
    void CSound::SetFade(float timeStart, float timeEnd, float startVolume, float targetVolume)
    {
        m_targetVolume = targetVolume;

        m_pChannel->getFrequency(&m_frequency);

        m_pChannel->addFadePoint(m_frequency * timeStart, startVolume);
        m_pChannel->addFadePoint(m_frequency * timeEnd, targetVolume);

        if (Equals(targetVolume, 0.0f))
            m_pChannel->setDelay(0, m_frequency * timeEnd, true);
    }

    //-----------------------------------------------------------------------------------
    bool CSound::InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt)
    {
        m_strFilename = strFilename;
        m_strName = strName;

        m_pSystem = gEngine->GetAudioSystem()->GetSystem();

        return LoadResource();
    }

    //-----------------------------------------------------------------------------------
    bool CSound::LoadResource()
    {
        bool bWasPlaying = false;
        m_pChannel->isPlaying(&bWasPlaying);

        if (m_pSound)
            m_pSound->release();

#ifdef ENV_USE_FILE_PACKAGES
        std::vector<char> soundData;
        gSoundPackageMgr->LoadFile(m_strFilename, soundData);

        FMOD_CREATESOUNDEXINFO info;
        memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        info.length = soundData.size();
        auto result = m_pSystem->createSound((const char*)&soundData[0], FMOD_OPENMEMORY, &info, &m_pSound);
#else
        m_pSystem->createSound(m_strFilename.c_str(), FMOD_DEFAULT, 0, &m_pSound);
#endif // ENV_USE_FILE_PACKAGES

        if (m_pSound && bWasPlaying)
        {
            PlaySound(m_channelGroup, m_loopCount);
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    CAudioSystem::CAudioSystem(const std::string& strResourceRootPath)
        : CSound::CSoundMgr(strResourceRootPath)
        , m_pSystem(NULL)
        , m_pMusic(nullptr)
        , m_currentChapter(-1)
        , m_currentMusicType(MUSIC_TYPE_NO_MUSIC)
    {
        m_fades.resize(2);
    }

    //-----------------------------------------------------------------------------------
    CAudioSystem::~CAudioSystem()
    {
        for (int i = 0; i < m_channelGroups.size(); ++i)
            m_channelGroups[i]->release();

        if (m_pSystem)
            m_pSystem->release();
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::SetAudioParameters(const AudioParameters& audioParams)
    {
        m_audioParameters = audioParams;

        SetVolume(AUDIO_CHANNELGROUP_MUSIC, static_cast<float>(m_audioParameters.musicVolume) / 100.0f);
        SetVolume(AUDIO_CHANNELGROUP_EFFECTS, static_cast<float>(m_audioParameters.sfxVolume) / 100.0f);
        SetVolume(AUDIO_CHANNELGROUP_AMBIENT, static_cast<float>(m_audioParameters.sfxVolume) / 100.0f);
        SetVolume(AUDIO_CHANNELGROUP_VOICE, static_cast<float>(m_audioParameters.sfxVolume) / 100.0f);

        SetVolume(AUDIO_CHANNELGROUP_MUSIC_PREVIEW, static_cast<float>(m_audioParameters.musicVolume) / 100.0f);
        SetVolume(AUDIO_CHANNELGROUP_EFFECTS_PREVIEW, static_cast<float>(m_audioParameters.sfxVolume) / 100.0f);
    }

    //-----------------------------------------------------------------------------------
    bool CAudioSystem::Initialize()
    {
        FMOD_RESULT result;

        result = FMOD::System_Create(&m_pSystem);      // Create the main system object.
        if (result != FMOD_OK)
        {
            //printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            ENV_LOG_ERROR("COULD NOT CREATE FMOD SOUNDSYSTEM.");

            return false;
        }

        result = m_pSystem->init(512, FMOD_INIT_THREAD_UNSAFE, 0);

        if (result != FMOD_OK)
        {
            //printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            ENV_LOG_ERROR("COULD NOT INIT FMOD SOUNDSYSTEM.");

            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::Update(float dt)
    {
        for (int i = 0; i < m_fades.size(); ++i)
        {
            if (m_fades[i].m_currentFadeTime >= 0.0f)
            {
                float alpha = m_fades[i].m_currentFadeTime / m_fades[i].m_fadeTime;
                float volume = alpha * m_fades[i].m_fadeStartVolume + (1.0f - alpha) * m_fades[i].m_fadeTargetVolume;

                m_fades[i].m_currentFadeTime -= dt;

                m_channelGroups[i]->setVolume(volume);
            }
        }

        m_pSystem->update();
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::SetFadeDuration(AUDIO_CHANNELGROUP channelGroup, float duration, float targetVolume)
    {
        m_fades[(int)channelGroup].m_fadeTime = duration;
        m_fades[(int)channelGroup].m_currentFadeTime = duration;
        m_fades[(int)channelGroup].m_fadeTargetVolume = targetVolume;
        m_channelGroups[channelGroup]->getVolume(&m_fades[(int)channelGroup].m_fadeStartVolume);
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::SetFadeDuration(AUDIO_CHANNELGROUP channelGroup, float duration, float startVolume, float targetVolume)
    {
        m_fades[(int)channelGroup].m_fadeTime = duration;
        m_fades[(int)channelGroup].m_currentFadeTime = duration;
        m_fades[(int)channelGroup].m_fadeStartVolume = startVolume;
        m_fades[(int)channelGroup].m_fadeTargetVolume = targetVolume;
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::AddChannelGroup(AUDIO_CHANNELGROUP channelGroup, const std::string& strName)
    {
        if (m_channelGroups.size() <= (int)channelGroup)
            m_channelGroups.resize(channelGroup + 1);

        m_pSystem->createChannelGroup(strName.c_str(), &m_channelGroups[channelGroup]);
    }

    //-----------------------------------------------------------------------------------
    FMOD::ChannelGroup* CAudioSystem::GetChannelGroup(AUDIO_CHANNELGROUP channelGroup)
    {
        return m_channelGroups[channelGroup];
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::SetVolume(AUDIO_CHANNELGROUP channelGroup, float volume)
    {
        m_channelGroups[channelGroup]->setVolume(volume);
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::SetMute(AUDIO_CHANNELGROUP channelGroup, bool bMute)
    {
        m_channelGroups[channelGroup]->setMute(bMute);
    }

    //-----------------------------------------------------------------------------------
    CSound* CAudioSystem::GetRandomTrack(const std::vector<std::string> playlist)
    {
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> dis(0, playlist.size() - 1);

        int trackIndex = dis(gen);

        std::string selectedTrackTitle = playlist[trackIndex];
        CSound* selectedTrack = AddGetPtr(selectedTrackTitle);
        return selectedTrack;
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::PlayMusicForMenu()
    {
        if (m_pMusic != nullptr && m_pMusic->IsPlaying())
        {
            return;
        }

        m_pMusic = GetRandomTrack(AUDIO_MUSIC_PLAYLIST_MENU);
        m_currentMusicType = MUSIC_TYPE_MENU;
        StartMusic();
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::PlayMusicForBoss()
    {
        if (m_pMusic != nullptr && m_pMusic->IsPlaying())
        {
            return;
        }

        m_pMusic = GetRandomTrack(AUDIO_MUSIC_PLAYLIST_BOSS);
        m_currentMusicType = MUSIC_TYPE_BOSS;

        StartMusic();
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::PlayMusicForChapter(const int chapter)
    {
        if (m_pMusic != nullptr && m_pMusic->IsPlaying())
        {
            return;
        }

        std::vector<std::string> playlist;

        switch (chapter)
        {
        case 0:
            playlist = AUDIO_MUSIC_PLAYLIST_CHAPTER1;
            break;
        case 1:
            playlist = AUDIO_MUSIC_PLAYLIST_CHAPTER2;
            break;
        case 2:
            playlist = AUDIO_MUSIC_PLAYLIST_CHAPTER3;
            break;
        case 3:
            playlist = AUDIO_MUSIC_PLAYLIST_CHAPTER4;
            break;
        default:
            playlist = AUDIO_MUSIC_PLAYLIST_CHAPTER1;
            break;
        }

        if (m_currentMusicType == MUSIC_TYPE_CHAPTER)
            m_pMusic = GetRandomTrack(playlist);
        else
            m_pMusic = AddGetPtr(playlist[0]);

        m_currentMusicType = MUSIC_TYPE_CHAPTER;
        StartMusic();
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::StartMusic(float fadeDuration)
    {
        if (m_pMusic == nullptr)
        {
            return;
        }

        if (m_pMusic->IsPlaying() == false)
        {
            m_pMusic->PlaySound();
        }

        m_pMusic->SetFadeDuration(fadeDuration, m_pMusic->GetVolume(), 1.0f);
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::StopMusic(float fadeDuration)
    {
        if (m_pMusic == nullptr ||
            Equals(m_pMusic->GetTargetVolume(), 0.0f))
        {
            return;
        }

        m_pMusic->SetFadeDuration(fadeDuration, m_pMusic->GetVolume(), 0.0f);
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::PauseSound()
    {
        for (int i = 0; i < m_channelGroups.size(); ++i)
        {
            if (i == AUDIO_CHANNELGROUP_MUSIC_PREVIEW || i == AUDIO_CHANNELGROUP_EFFECTS_PREVIEW)
            {
                continue;
            }
            m_channelGroups[i]->setPaused(true);
        }
    }

    //-----------------------------------------------------------------------------------
    void CAudioSystem::ContinueSound()
    {
        for (int i = 0; i < m_channelGroups.size(); ++i)
        {
            if (i == AUDIO_CHANNELGROUP_MUSIC_PREVIEW || i == AUDIO_CHANNELGROUP_EFFECTS_PREVIEW)
            {
                continue;
            }
            m_channelGroups[i]->setPaused(false);
        }
    }



} // env

#endif
