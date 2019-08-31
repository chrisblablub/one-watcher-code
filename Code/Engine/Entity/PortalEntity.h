/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef PORTAL_ENTITY_H
#define PORTAL_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>

namespace env 
{
	class CPortalEntity : public CSpriteEntity
	{
	public: 
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		CPortalEntity();
		virtual ~CPortalEntity();

		virtual bool HasCollision(CEntity& entity);
		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void Update(float dt);
		virtual void OnGameModeChanged(bool bGameMode);
		virtual void OnTriggerLeft();

		void SetTriggerDisabled(bool bDisabled) { m_bTriggerDisabled = bDisabled; }
		void SetPortalMate(CPortalEntity* pMate);

	private:
		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CSpriteEntity >(*this);
			ar & m_pPortalMate;
		}

	protected:
		bool m_bActive;
		bool m_bTriggerDisabled;

		CPortalEntity* m_pPortalMate;
	};	

	
} // env

#endif // PORTAL_ENTITY_H 