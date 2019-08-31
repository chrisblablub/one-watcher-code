#include <Engine/Entity/SoundSpriteEntity.h>
#include <Engine/Entity/CharacterEntity.h>

namespace env
{
    ENTITY_TYPEID CSoundSpriteEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SOUNDSPRITE;

    //-----------------------------------------------------------------------------------
    CSoundSpriteEntity::CSoundSpriteEntity()
        : m_bIsAutoplay(true)
        , m_pSound(nullptr)
        , m_pListener(nullptr)
        , m_Volume(1.0f)
        , m_bIsMuted(false)
        , m_bPlayBossMusic(false)
        , m_PeakVolumeDistance(90000)
        , m_AudibleDistance(360000)
        , m_FileName("")
        , m_FadeDuration(0.0f)
        , m_CurrentFadeDuration(0.0f)
        , m_FadeVolumeStart(0.0f)
    {
        SetMetaTypeID(CSoundSpriteEntity::ENTITY_META_TYPEID);

        RegisterEvent("[Sound] Play", SOUND_EVENT_PLAY);
        RegisterEvent("[Sound] Play Boss Music", SOUND_EVENT_PLAY_BOSS_MUSIC);
        RegisterEvent("[Sound] Stop", SOUND_EVENT_STOP);
        RegisterEventParameter(SOUND_EVENT_STOP, "FadeDuration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f);
    }

    //-----------------------------------------------------------------------------------
    CSoundSpriteEntity::~CSoundSpriteEntity()
    {
        ReleaseSound();
    }

    //-----------------------------------------------------------------------------------
    void CSoundSpriteEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);

        if (m_FadeDuration != 0.0f)
        {
            m_CurrentFadeDuration = m_CurrentFadeDuration - dt;
            float fadeFactor = m_CurrentFadeDuration / m_FadeDuration;
            float fadeVolume = fadeFactor * m_FadeVolumeStart;

            if (fadeVolume > 0)
            {
                SetVolume(fadeFactor * m_FadeVolumeStart);
            }
            else
            {
                // avoid regression risk
                SetVolume(m_FadeVolumeStart);
                m_bIsMuted = true;

                m_FadeDuration = 0.0f;
                m_CurrentFadeDuration = 0.0f;
                m_FadeVolumeStart = 0.0f;
            }
        }

        if (m_bPlayBossMusic)
            gEngine->GetAudioSystem()->PlayMusicForBoss();


        if (m_bIsAutoplay)
        {
            PlaySound();
        }
    }

    //-----------------------------------------------------------------------------------
    void CSoundSpriteEntity::ReleaseSound()
    {
        if (m_pSound == nullptr)
        {
            return;
        }

        int channelID = (int) this;
        m_pSound->StopMultiChannelSound(channelID);
        gEngine->GetAudioSystem()->Release(m_FileName);
        m_pSound = nullptr;
    }

    //-----------------------------------------------------------------------------------
    void CSoundSpriteEntity::LoadSound(std::string fileName)
    {
        ReleaseSound();

        m_FileName = fileName;
        m_pSound = gEngine->GetAudioSystem()->AddGetPtr(fileName);
        m_pSound->SetChannelGroup(AUDIO_CHANNELGROUP_AMBIENT);
    }

    //-----------------------------------------------------------------------------------
    void CSoundSpriteEntity::OnGameModeChanged(bool bGameMode)
    {
        m_pListener = gEngine->GetEntityMgr()->GetCharacter();

        if (m_pListener == nullptr)
        {
            m_pListener = this;
        }

        m_bRender = bGameMode == false;
        m_bIsMuted = bGameMode == false;

        m_CurrentFadeDuration = 0.0f;

        if (m_pSound == nullptr && m_FileName.length() > 0)
        {
            LoadSound(m_FileName);
        }

        if (m_pSound)
        {
            m_pSound->StopSound();

            if (Equals(m_AudibleDistance, 0.0f) ||
                Equals(m_PeakVolumeDistance, 0.0f))
            {
                m_pSound->SetChannelGroup(AUDIO_CHANNELGROUP_VOICE);
            }
            else
            {
                m_pSound->SetChannelGroup(AUDIO_CHANNELGROUP_AMBIENT);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CSoundSpriteEntity::PlaySound()
    {
        if (m_pSound == nullptr || m_pListener == nullptr)
        {
            return;
        }

        int channelID = (int) this;

        if (m_bIsMuted == true || m_Volume == 0)
        {
            m_pSound->SetMultiChannelVolume(channelID, 0);
            m_pSound->StopMultiChannelSound(channelID);
            return;
        }

        Vec3 listenerPosition = m_pListener->GetNode()->GetPosition();
        Vec3 distanceVector = GetNode()->GetDerivedPosition() - listenerPosition;

        const float aspectRatio = 1.77f;
        float distance = (distanceVector[0] * distanceVector[0]) + aspectRatio * (distanceVector[1] * distanceVector[1]);

        if (distance > m_AudibleDistance && m_AudibleDistance > 0.0f)
        {
            m_pSound->SetMultiChannelVolume(channelID, 0);
            m_pSound->StopMultiChannelSound(channelID);
            return;
        }

        float distanceFactor = 1;

        if (m_AudibleDistance > m_PeakVolumeDistance &&
            m_AudibleDistance > 0.0f)
        {
            distanceFactor = (m_AudibleDistance - distance) / (m_AudibleDistance - m_PeakVolumeDistance);
            distanceFactor = distanceFactor < 1 ? distanceFactor : 1;
        }

        float playbackVolume = m_Volume * distanceFactor;

        if (m_pSound->IsPlayingMultiChannelSound(channelID) == false)
        {
            m_pSound->PlayMultiChannelSound(channelID, m_bIsAutoplay);
        }

        m_pSound->SetMultiChannelVolume(channelID, playbackVolume);
    }

    void CSoundSpriteEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case SOUND_EVENT_PLAY:
            PlaySound();
            break;

        case SOUND_EVENT_STOP:
        {
            if (eventParameter.IsParameterSet("FadeDuration"))
            {
                eventParameter.GetParameter("FadeDuration", m_FadeDuration);
                eventParameter.GetParameter("FadeDuration", m_CurrentFadeDuration);
                m_FadeVolumeStart = m_Volume;
            }
            else
            {
                m_bIsMuted = true;
            }
        } break;

        case SOUND_EVENT_PLAY_BOSS_MUSIC:
            gEngine->GetAudioSystem()->StopMusic(0.0f);
            gEngine->GetAudioSystem()->PlayMusicForBoss();

            m_bPlayBossMusic = true;
            break;
        }
    }
} // env