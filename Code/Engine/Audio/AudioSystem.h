/*
* ============================================================
* Copyright (C) 2014 by Envisioned.
* This file is part of the envisioned engine source code.
* http://www.envisioned.de
* ============================================================
*/
#pragma once
#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/Settings.h>

namespace FMOD
{
    struct Sound;
    struct Channel;
    struct System;
    struct ChannelGroup;
}

namespace env
{


    const std::string AUDIO_EFFECT_MENUE_SELECTION = "MenueSelection.wav";
    const std::string AUDIO_EFFECT_MENUE_CONFIRMATION = "MenueConfirmation.wav";
    const std::string AUDIO_EFFECT_TRANSITION_IN = "TransitionIn.wav";
    const std::string AUDIO_EFFECT_TRANSITION_OUT = "TransitionOut.wav";

	const std::string AUDIO_EFFECT_MENUE_UNLOCK_LEVEL = "MenueUnlockLevel.wav";
	const std::string AUDIO_EFFECT_MENUE_UNLOCK_CHAPTER = "MenueUnlockChapter.wav";

    const std::string AUDIO_MUSIC_TRACK_00 = "08 Cutscenes.wav";
    const std::string AUDIO_MUSIC_TRACK_01 = "01 The Way Up_master.wav";
    const std::string AUDIO_MUSIC_TRACK_02 = "02 The Morgenstern Song_master.wav";
    const std::string AUDIO_MUSIC_TRACK_03 = "03 Icecold Metal_master.wav";
    const std::string AUDIO_MUSIC_TRACK_04 = "04 Through the Stratosphere_master.wav";
    const std::string AUDIO_MUSIC_TRACK_05 = "05 Boss theme_master.wav";
    const std::string AUDIO_MUSIC_TRACK_06 = "06 Higher!_master.wav";
    const std::string AUDIO_MUSIC_TRACK_07 = "07 UnderRock.wav";
    
    const std::vector<std::string> AUDIO_MUSIC_PLAYLIST_MENU = { AUDIO_MUSIC_TRACK_00 };
    const std::vector<std::string> AUDIO_MUSIC_PLAYLIST_BOSS = { AUDIO_MUSIC_TRACK_05 };
    const std::vector<std::string> AUDIO_MUSIC_PLAYLIST_CHAPTER1 = { AUDIO_MUSIC_TRACK_02, AUDIO_MUSIC_TRACK_07 };
    const std::vector<std::string> AUDIO_MUSIC_PLAYLIST_CHAPTER2 = { AUDIO_MUSIC_TRACK_01, AUDIO_MUSIC_TRACK_06 };
    const std::vector<std::string> AUDIO_MUSIC_PLAYLIST_CHAPTER3 = { AUDIO_MUSIC_TRACK_03 };
    const std::vector<std::string> AUDIO_MUSIC_PLAYLIST_CHAPTER4 = { AUDIO_MUSIC_TRACK_04 };

    enum AUDIO_CHANNELGROUP
    {
        AUDIO_CHANNELGROUP_MUSIC = 0,
        AUDIO_CHANNELGROUP_EFFECTS,
        AUDIO_CHANNELGROUP_AMBIENT,
        AUDIO_CHANNELGROUP_VOICE,
		AUDIO_CHANNELGROUP_MUSIC_PREVIEW,
		AUDIO_CHANNELGROUP_EFFECTS_PREVIEW
    };

	enum MUSIC_TYPE
	{
		MUSIC_TYPE_NO_MUSIC = 0,
		MUSIC_TYPE_BOSS,
		MUSIC_TYPE_MENU,
		MUSIC_TYPE_CHAPTER
	};

    class CSound
    {
    public:
        ~CSound();

        typedef CResMgrBase<std::string, CSound> CSoundMgr;

        friend class CResMgrBase<std::string, CSound>;
        friend struct DefaultAllocationPolicy<CSound>;

        void PlaySound(int loopCount = 0);
        void PlaySound(AUDIO_CHANNELGROUP channelGroup, int loopCount = 0);
        void StopSound();

        void SetVolume(float vol);

        void SetSpeed(float speed);

        void SetChannelGroup(AUDIO_CHANNELGROUP channelGroup);

        void SetFadeDuration(float duration, float targetVolume);
        void SetFadeDuration(float duration, float startVolume, float targetVolume);
        void SetFade(float timeStart, float timeEnd, float targetVolume);
        void SetFade(float timeStart, float timeEnd, float startVolume, float targetVolume);

        float GetTargetVolume() const { return m_targetVolume; }
        float GetVolume() const { return m_volume; }
        bool IsPlaying();

        std::map < int, FMOD::Channel* > m_ChannelMap;

        bool IsPlayingMultiChannelSound(const int channelID);
        void StopMultiChannelSound(const int channelID);
        void PlayMultiChannelSound(const int channelID, const bool isAutoplay);
        void SetMultiChannelVolume(const int channelID, const float volume);

        inline const std::string& GetName() const { return m_strName; }

    private:
        CSound();


        bool InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt);
        bool LoadResource();

        std::string m_strFilename;
        std::string m_strName;

        FMOD::Sound* m_pSound;
        FMOD::Channel* m_pChannel;
        FMOD::System* m_pSystem;

        AUDIO_CHANNELGROUP m_channelGroup;

        float m_frequency;
        float m_speed;
        float m_volume;
        float m_targetVolume;

        int m_loopCount;
    };


    class CAudioSystem : public CSound::CSoundMgr
    {
    public:
        struct Fade
        {
            Fade()
                : m_currentFadeTime(-1.0f),
                m_fadeTime(0.0f),
                m_fadeStartVolume(0.0f),
                m_fadeTargetVolume(1.0f) {}

            float m_currentFadeTime;
            float m_fadeTime;
            float m_fadeStartVolume;
            float m_fadeTargetVolume;
        };

        CAudioSystem(const std::string& strResourceRootPath);
        ~CAudioSystem();

        bool Initialize();

        void Update(float dt);

        FMOD::System* GetSystem() { return m_pSystem; }

        void AddChannelGroup(AUDIO_CHANNELGROUP channelGroup, const std::string& strName);
        FMOD::ChannelGroup* GetChannelGroup(AUDIO_CHANNELGROUP channelGroup);

        void SetFadeDuration(AUDIO_CHANNELGROUP channelGroup, float duration, float targetVolume);
        void SetFadeDuration(AUDIO_CHANNELGROUP channelGroup, float duration, float startVolume, float targetVolume);

        void SetVolume(AUDIO_CHANNELGROUP channelGroup, float volume);

        void SetMute(AUDIO_CHANNELGROUP channelGroup, bool bMute);

        AudioParameters GetAudioParameters() const { return m_audioParameters; }

        void SetAudioParameters(const AudioParameters& audioParams);

        void PlayMusicForMenu();
        void PlayMusicForBoss();
        void PlayMusicForChapter(const int chapter);

        void StartMusic(float fadeDuration = 0.5f);
        void StopMusic(float fadeDuration = 0.5f);

		MUSIC_TYPE GetCurrentMusicType() { return m_currentMusicType; }

		void PauseSound();
		void ContinueSound();

    private:
        CSound* m_pMusic;
        CSound* GetRandomTrack(const std::vector<std::string> playlist);

        AudioParameters m_audioParameters;

        FMOD::System* m_pSystem;

        std::vector< FMOD::ChannelGroup* > m_channelGroups;

        std::vector< Fade > m_fades;

        int m_currentChapter;

        MUSIC_TYPE m_currentMusicType;
    };

} // env

#endif // AUDIOSYSTEM_H 