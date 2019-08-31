/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef POLYGON_ENTITY_H
#define POLYGON_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Core/DataCache.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
	typedef CDataCache< PolygonVertex > CPolygonEntityCache;

	class CPolygonEntity : public CSpriteEntity
	{
	public: 
		static ENTITY_TYPEID ENTITY_META_TYPEID;

		enum TRIGGERABLE_EVENTS { };

		CPolygonEntity();
		virtual ~CPolygonEntity();

		//virtual bool HasCollision(const VECTOR3& vRayOrigin, const VECTOR3& vRayDirection, float& distance, CSceneNode** ppNode);
		
		virtual void InitializeAtCreation();
		virtual void InitializeAtLoad();
		virtual void AddToRenderQueue(float dt, RenderQueue& queue);
		virtual void Update(float dt);
		virtual void OnGameModeChanged(bool bGameMode);

		void SetColor(const Vec3& color) { m_color = color; }
		const Vec3& GetColor() { return m_color; }

		void AddPoint(const Vec3& point);


	private:
		friend class boost::serialization::access;

		template< class Archive > void serialize(Archive& ar, const unsigned int version)
		{
			ar & boost::serialization::base_object< CSpriteEntity >(*this);
			ar & m_points;
			ar & m_color;
			ar & m_newPoints;
		}

	private:

		typedef std::list< CSpriteEntity* > listPoints;
		listPoints m_points;

		typedef std::vector< CSpriteEntity* > vecPoints;
		vecPoints m_newPoints;

		CPolygonEntityCache m_vertices;
		//vecVertexPolygon m_vertices;

		Vec3 m_color;

	};
} // env

#endif // POLYGON_ENTITY_H 