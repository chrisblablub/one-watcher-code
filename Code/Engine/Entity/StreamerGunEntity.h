/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef STREAM_GUN_ENTITY_H
#define STREAM_GUN_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/LightEntity.h>
#include <Engine/Entity/SoundSpriteEntity.h>

namespace env
{
	enum STREAM_TYPE
	{
		STREAM_TYPE_SMALL,
		STREAM_TYPE_BIG
	};

	class CStreamEntity : public CParticleSystemEntity
	{
	public:
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		CStreamEntity();
		virtual ~CStreamEntity();

		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void Update(float dt);
		virtual void OnGameModeChanged(bool bGameMode);
		

		void SetEnabled(bool bEnable);
		

		void SetStreamType(STREAM_TYPE type);
		STREAM_TYPE GetStreamType() const { return m_type; }

		//void SetSpeed(float speed) { m_speed = speed; }

		//void SetLifeTime(float lifeTime) { m_lifeTime = lifeTime; }
		
		float GetCurrentLifeTime() const { return m_currentLifeTime; }
    protected:
        CSoundSpriteEntity* m_pSound;
        void SetupSounds();
		
	private:
		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CParticleSystemEntity >(*this);
			
			if(version > 0)
				ar & m_type;
			//ar & m_pLight;
            if (version > 1)
                ar & m_pSound;
		}
	private:
		//CLightSpriteEntity* m_pLight;

		float m_currentLifeTime;

		//float m_speed;
		//float m_lifeTime;

		bool m_bEnabled;

		STREAM_TYPE m_type;
	};


	class CStreamerGunEntity : public CMeshEntity
	{
	public: 
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		CStreamerGunEntity ();
		virtual ~CStreamerGunEntity ();

		virtual void Update(float dt);
		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void OnGameModeChanged(bool bGameMode);
		virtual void OnEntityCloned(CEntity& entity);

		void SetSpeed(float speed) { m_speed = speed; }
		float GetSpeed() const { return m_speed; }

		void SetLifeTime(float lifeTime) { m_lifeTime = lifeTime; UpdateStreams(); }
		float GetLifeTime() const { return m_lifeTime; }

		void SetFireRate(float rate) { m_fireDelay = rate; UpdateStreams(); }
		float GetFireRate() const { return m_fireDelay; }

		void SetDelay(float delay) { m_delay = delay; }
		float GetDelay() const { return m_delay; }

		void SetStreamType(STREAM_TYPE type) { if(m_type != type) { m_type = type; UpdateStreams(); } }
		STREAM_TYPE GetStreamType() const { return m_type; }


	private:
		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CMeshEntity >(*this);
			ar & m_streams;
			ar & m_lifeTime;
			ar & m_speed;
			ar & m_fireDelay;

			if(version > 0)
				ar & m_type;

			if(version > 1)
				ar & m_delay;
		}

		void UpdateStreams();

	private:
		float m_delay;
		float m_fireDelay;
		float m_fireAccum;

		float m_lifeTime;
		float m_speed;

		float m_time;

		std::vector< CStreamEntity* > m_streams;

		unsigned int m_currentNumActiveStreams;

		float m_rotatationTime;

		STREAM_TYPE m_type;


	};

	
} // env

#endif // FIRE_BALL_GUN_ENTITY_H 