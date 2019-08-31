/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BOMB_ENTITY_H
#define BOMB_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/ExplosionEntity.h>
#include <Engine/Core/PhysicsNode.h>
#include <Common/Core/Timer.h>

namespace env 
{
	class CBombEntity : public CMeshEntity
	{
	public: 
		enum BOMB_STATE
		{
			BOMB_STATE_INACTIVE,
			BOMB_STATE_LOADING,
			BOMB_STATE_EXPLODING
		};

		static ENTITY_TYPEID ENTITY_META_TYPEID;

		CBombEntity();

		virtual ~CBombEntity();
		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void Update(float dt);
		virtual void OnGameModeChanged(bool bGameMode);
		virtual bool HasCollision(CEntity& e);

		void SetState(BOMB_STATE state);
		BOMB_STATE GetState() const { return m_state; }

		void SetEnabled(bool bEnable);

		void SetLifetime(float lifetime) { m_lifeTime = lifetime; }
		float GetLifeTime() const { return m_lifeTime; }

		float GetCurrentLifetime() const { return m_currentLifeTime; }
		float GetCurrentStateTime() const { return m_stateTimer.GetTime(); }

		void SetImpulse(float imp ) { m_impulse = imp; }

		CPhysicsNode2D& GetPhysicsNode() { return m_bombPhysics; }

	private:
		void UpdateBombLoading(float dt);
		void UpdateBombExploding(float dt);

		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CMeshEntity >(*this);
			ar & m_pExplosion;
		}

	protected:
		bool m_bEnabled;

		float m_impulse;

		float m_currentLifeTime;
		float m_lifeTime;

		BOMB_STATE m_state;

		CTimer m_stateTimer;

		CPhysicsNode2D m_bombPhysics;

		CExplosionEntity* m_pExplosion;
	};	


	class CBombEntityGun : public CMeshEntity
	{
	public: 
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		CBombEntityGun();

		virtual ~CBombEntityGun();
		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void Update(float dt);
		virtual void OnGameModeChanged(bool bGameMode);

		void SetFireDelayMin(float del) { m_fireDelayMin = del; UpdateBombs(); }
		float GetFireDelayMin() const { return m_fireDelayMin; }

		void SetFireDelayMax(float del) { m_fireDelayMax = del; UpdateBombs(); }
		float GetFireDelayMax() const { return m_fireDelayMax; }

		void SetLifetimeMin(float del) { m_lifeTimeMin = del; UpdateBombs(); }
		float GetLifetimeMin() const { return m_lifeTimeMin; }

		void SetLifetimeMax(float del) { m_lifeTimeMax = del; UpdateBombs(); }
		float GetLifetimeMax() const { return m_lifeTimeMax; }

		void SetImpulseMin(float imp) { m_impulseMin = imp; UpdateBombs(); }
		float GetImpulseMin() const { return m_impulseMin; }

		void SetImpulseMax(float imp) { m_impulseMax = imp; UpdateBombs(); }
		float GetImpulseMax() const { return m_impulseMax; }

	private:

		
		void UpdateBombs();

		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CMeshEntity >(*this);
			ar & m_fireDelayMin;
			ar & m_fireAccum;

			if(version > 0)
			{
				ar & m_lifeTimeMin;
				ar & m_bombs;
				ar & m_impulseMin;
			}

			if(version > 1)
			{
				ar & m_lifeTimeMax;
				ar & m_impulseMax;
				ar & m_fireDelayMax;
			}
		}

		float m_fireDelayMin;
		float m_fireDelayMax;
		float m_fireAccum;
		float m_fireDelay;
		float m_lifeTimeMin;
		float m_lifeTimeMax;
		float m_impulseMin;
		float m_impulseMax;

		float m_currentRotatationTime;


		std::vector< CBombEntity* > m_bombs;

		unsigned int m_currentNumActiveBombs;
	};


	
} // env

#endif // BOMB_ENTITY_H 