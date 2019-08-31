/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef EDITOR_STATE_H
#define EDITOR_STATE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/State.h>
#include <Engine/Core/Input.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{

    class CEditorState : public CState, public CMouseListener
    {
    public:
        enum ENTITY_PICK_TYPE
        {
            ENTITY_PICK_TYPE_ALL = 0,
            ENTITY_PICK_TYPE_SPRITES,
            ENTITY_PICK_TYPE_MESHES,
            ENTITY_PICK_TYPE_ENTITIES
        };

        enum CLICK_HANDLER
        {
            CLICK_HANDLER_NONE = 0,
            CLICK_HANDLER_PICK = 1,
            CLICK_HANDLER_EDITTERRAIN = 2,
            CLICK_HANDLER_EDITTERRAIN_DIFFUSE = 4,
            CLICK_HANDLER_EDITTERRAIN_EMISSIVE = 8,
            CLICK_HANDLER_SELECTTERRAIN = 16,
            CLICK_HANDLER_PAINTTERRAIN = 32,
            CLICK_HANDLER_PICKTERRAIN = 64,

            CLICK_HANDLER_TERRAIN_MASK = CLICK_HANDLER_EDITTERRAIN | CLICK_HANDLER_EDITTERRAIN_DIFFUSE | CLICK_HANDLER_EDITTERRAIN_EMISSIVE | CLICK_HANDLER_PICKTERRAIN,
        };

        static const std::string STATE_NAME;

        CEditorState();
        virtual ~CEditorState();

        virtual bool Load(const std::string& strPreviousState);
        virtual bool Unload();


        virtual void Update(float time, float dt, RenderQueue& renderQueue);

        void SetClickHandler(CLICK_HANDLER clickEvent) { m_clickHandler = clickEvent; }

        void LoadTerrainTemplate(const std::string& strFilename);

        void OnNotified(unsigned long id, const MouseInputEvent& Event);

        void SetPickType(ENTITY_PICK_TYPE pickType) { m_entityPickType = pickType; }
        ENTITY_PICK_TYPE GetPickType() const { return m_entityPickType; }

        void SetLightingMode(RenderQueue::LIGHTING_MODE mode) { m_lightingMode = mode; }
        RenderQueue::LIGHTING_MODE GetLightingMode() const { return m_lightingMode; }

        int m_selectMinX, m_selectMinY, m_selectMaxX, m_selectMaxY;

    private:
        void ProcessInput(float dt, RenderQueue& renderQueue);

        CLICK_HANDLER m_clickHandler;

        ENTITY_PICK_TYPE m_entityPickType;

        bool m_bFreeLook;
        bool m_bValidGridIndex;
        bool m_bSelectedTerrain;

        float m_rightBtnDownTime;

        unsigned int m_selectionCellX, m_selectionCellY;

        Vec3 m_viewingRay;
        Vec3 m_viewingOrigin;

        RenderQueue::LIGHTING_MODE m_lightingMode;

    };
} // env
#endif // EDITOR_STATE_H