/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SWITCH_ENTITY_H
#define SWITCH_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{

	class CMeshSwitchEntity : public CMeshEntity
	{
	public: 
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		CMeshSwitchEntity();

		virtual ~CMeshSwitchEntity();
		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void OnGameModeChanged(bool bGameMode);
		virtual void OnTriggerEntered(CEntity& e);
		virtual void Update(float dt);

	private:
		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CMeshEntity >(*this);
			ar & m_pLight;
		}

		bool m_bSwitchTriggered;

		CLightSpriteEntity* m_pLight;

	
		float m_currentTriggerTime;
	};


} // env

#endif // SWITCH_ENTITY_H