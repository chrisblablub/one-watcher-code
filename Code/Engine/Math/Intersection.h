/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

/*#pragma once
#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/BoundingVolume.h>

namespace env
{
	class CFrustum;

	namespace intersection
	{
		enum INTERSECTION 
		{ 
			INTERSECTION_INSIDE = 0, 
			INTERSECTION_OUTSIDE, 
			INTERSECTION_INTERSECTING 
		};


		INTERSECTION FrustumIntersectsAABB(const CFrustum& frustum, VECTOR3 vBMin, VECTOR3 vBMax);
		INTERSECTION FrustumIntersectsBB(const CFrustum& frustum, const CBoundingVolume& box, const MATRIX& mtxBBoxTransform);
		bool FrustumIntersectsPoint(const CFrustum& frustum, const VECTOR3& vPoint);
		bool FrustumIntersectsSphere(const CFrustum& frustum, const VECTOR3& vCenter, float radius);
		bool RayIntersectsBB(const CBoundingVolume& bbox, const VECTOR3& vOrigin, const VECTOR3& vDirection, float* pDistance = NULL);
	}
} // env

#endif // INTERSECTION_H*/