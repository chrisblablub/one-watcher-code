/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SOUND_SPRITE_ENTITY_H
#define SOUND_SPRITE_ENTITY_H


#include <Engine/EngineCommon.h>
#include <Engine/Audio/AudioSystem.h>
#include <Engine/Entity/SpriteEntity.h>

namespace env
{
    class CSoundSpriteEntity : public CSpriteEntity
    {
        enum TRIGGERABLE_EVENTS
        {
            SOUND_EVENT_PLAY = SPRITE_EVENT_END,
            SOUND_EVENT_PLAY_BOSS_MUSIC,
            SOUND_EVENT_STOP,
            SOUND_EVENT_END
        };


    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CSoundSpriteEntity();
        virtual ~CSoundSpriteEntity();

        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);


        void LoadSound(std::string fileName);
        void ReleaseSound();
        std::string GetFileName() { return m_FileName; }

        void PlaySound();

        float GetVolume() { return m_Volume; }
        void SetVolume(float volume) { m_Volume = volume; }

        bool GetIsAutoplay() { return m_bIsAutoplay; }
        void SetIsAutoplay(bool isAutoplay) { m_bIsAutoplay = isAutoplay; }

        bool GetIsMuted() { return m_bIsMuted; }
        void SetIsMuted(bool isMuted) { m_bIsMuted = isMuted; }

        void SetAudibleDistance(float audibleDistance) { m_AudibleDistance = audibleDistance * audibleDistance; }
        float GetAudibleDistance() { return sqrt(m_AudibleDistance); }

        void SetPeakVolumeDistance(float peakVolumeDistance) { m_PeakVolumeDistance = peakVolumeDistance * peakVolumeDistance; }
        float GetPeakVolumeDistance() { return sqrt(m_PeakVolumeDistance); }

    protected:
        CEntity* m_pListener;
        CSound* m_pSound;

        std::string m_FileName;
        float m_Volume;
        float m_AudibleDistance;
        float m_PeakVolumeDistance;
        float m_LastPlaybackVolume;

        bool m_bIsMuted;
        bool m_bIsAutoplay;
        bool m_bPlayBossMusic;

		float m_FadeDuration;
		float m_CurrentFadeDuration;
		float m_FadeVolumeStart;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);

            ar & m_FileName;
            ar & m_Volume;
            ar & m_AudibleDistance;
            ar & m_PeakVolumeDistance;

            ar & m_bIsMuted;
            ar & m_bIsAutoplay;
        }
    };


} // env

#endif // SOUND_SPRITE_ENTITY_H