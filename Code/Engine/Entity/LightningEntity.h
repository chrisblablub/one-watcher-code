/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LIGHTNING_ENTITY_H
#define LIGHTNING_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/LightSpriteEntity.h>

namespace env 
{
	class CLightningEntity : public CSpriteEntity
	{
	public: 
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		CLightningEntity();
		virtual ~CLightningEntity();

		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void Update(float dt);
		virtual void OnGameModeChanged(bool bGameMode);

	private:
		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CSpriteEntity >(*this);
			ar & m_pLightningLight;
		}

	protected:

		CLightSpriteEntity* m_pLightningLight;

		float m_lightningTime;
		float m_energyFieldIntensity;
		float m_accumLightningTime;
		float m_accumPauseTime;

		int m_currentLightningIteration;
		int m_numLightningIterations;

		//VECTOR4 m_fogColor;
		//VECTOR3 m_skyColor;

	};	

	
} // env

#endif // LIGHTNING_ENTITY_H 