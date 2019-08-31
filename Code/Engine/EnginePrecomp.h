/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

// #ifndef WIN32_LEAN_AND_MEAN
// #define WIN32_LEAN_AND_MEAN
// #endif
//#include <windows.h>

#include <Common/CommonIncludes.h>

#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/Texture.h>
#include <Graphic/Core/Material.h>
#include <Graphic/RenderObjects/BloomRenderObject.h>
#include <Graphic/RenderObjects/CameraRenderObject.h>
#include <Graphic/RenderObjects/CharacterRenderObject.h>
#include <Graphic/RenderObjects/EnvironmentRenderObject.h>
#include <Graphic/RenderObjects/FadeRenderObject.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>
#include <Graphic/RenderObjects/LightRaysRenderObject.h>
#include <Graphic/RenderObjects/LightRenderObject.h>
#include <Graphic/RenderObjects/MeshRenderObject.h>
#include <Graphic/RenderObjects/ParticleSystemRenderObject.h>
#include <Graphic/RenderObjects/PostProcessRenderObject.h>
#include <Graphic/RenderObjects/SpriteRenderObject.h>
#include <Graphic/RenderObjects/TextRenderObject.h>
#include <Graphic/RenderObjects/WatcherVideoRenderObject.h>
#include <Graphic/RenderSystems/DeferredRenderSystem.h>

#include <Engine/EngineConfig.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/WorldMgr.h>

#include <Engine/Entity/Entity.h>
#include <Engine/Entity/EntityMgr.h>

#include <Engine/Scene/SceneNode.h>
