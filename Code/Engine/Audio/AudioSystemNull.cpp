#include "AudioSystem.h"

#if 0

namespace env
{

	CSound::CSound()
	{

	}

	CSound::~CSound()
	{
	}
	
	void CSound::SetChannelGroup(AUDIO_CHANNELGROUP channelGroup)
	{
	}

	void CSound::PlaySound(int loopCount)
	{
	
	}


	void CSound::PlaySound(AUDIO_CHANNELGROUP channelGroup, int loopCount)
	{
		
	}

	void CSound::StopSound()
	{
	}
	
	bool CSound::IsPlaying()
	{
        return false;
	}

	void CSound::SetVolume(float vol)
	{
	}
		
	void CSound::SetSpeed(float speed)
	{
	}

	void CSound::SetFadeDuration(float duration, float targetVolume)
	{
		
	}

	void CSound::SetFadeDuration(float duration, float startVolume, float targetVolume)
	{
		
	}

	void CSound::SetFade(float timeStart, float timeEnd, float targetVolume)
	{
	
	}

	void CSound::SetFade(float timeStart, float timeEnd, float startVolume, float targetVolume)
	{

	}


	bool CSound::InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt)
	{
        return true;
	}

	bool CSound::LoadResource()
	{
		return true;
	}





	CAudioSystem::CAudioSystem(const std::string& strResourceRootPath)
		: CSound::CSoundMgr(strResourceRootPath)
	{
	}

    CAudioSystem::~CAudioSystem()
    {
    }


	bool CAudioSystem::Initialize(const AudioParameters& audioParams)
	{

		return true;
	}


	void CAudioSystem::Update(float dt)
	{
		
	}

	void CAudioSystem::SetFadeDuration(AUDIO_CHANNELGROUP channelGroup, float duration, float targetVolume)
	{
	
	}

	void CAudioSystem::SetFadeDuration(AUDIO_CHANNELGROUP channelGroup, float duration, float startVolume, float targetVolume)
	{

	}

	void CAudioSystem::AddChannelGroup(AUDIO_CHANNELGROUP channelGroup, const std::string& strName)
	{

	}

	FMOD::ChannelGroup* CAudioSystem::GetChannelGroup(AUDIO_CHANNELGROUP channelGroup)
	{
        return nullptr;
	}

	void CAudioSystem::SetVolume(AUDIO_CHANNELGROUP channelGroup, float volume)
	{
	}

	void CAudioSystem::SetMute(AUDIO_CHANNELGROUP channelGroup, bool bMute)
	{
	}
} // env

#endif;