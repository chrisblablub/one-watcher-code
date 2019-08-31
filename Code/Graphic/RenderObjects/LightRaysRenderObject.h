/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LIGHT_RAYS_RENDER_OBJECT_H
#define LIGHT_RAYS_RENDER_OBJECT_H

#include <Graphic/GraphicCommon.h>

namespace env
{
	struct LightRaysRenderObject
	{
		float m_lightRaysIllumDecay;
		float m_lightRaysDecay;

		bool m_bLightRaysOcclusionMask;
		bool m_bLightRays;
		bool m_bLightRaysOrigin;

		Vec3 m_lightRaysOrigin;
		Vec3 m_lightRaysColor;
	};

} // env
#endif // LIGHT_RAYS_RENDER_OBJECT_H 