/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef COUNTER_ENTITY_H
#define COUNTER_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>

namespace env
{

	class CCounterEntity : public CSpriteEntity
	{
	public: 
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		enum TRIGGERABLE_EVENTS { COUNTER_EVENT_INCREMENT = SPRITE_EVENT_END, COUNTER_EVENT_DECREMENT, COUNTER_EVENT_END };

		CCounterEntity();
		virtual ~CCounterEntity();

		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void Update(float dt);
		virtual void OnFired(unsigned int eventId);
		virtual void OnGameModeChanged(bool bGameMode);

		void SetCount(unsigned int count);
		unsigned int GetCount();


	private:
		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CSpriteEntity >(*this);
			ar & m_count;
		}

	private:
		unsigned int m_count;
		unsigned int m_currentCount;
	};
} // env

#endif // COUNTER_ENTITY_H 