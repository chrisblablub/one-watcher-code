/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SPRITE_ENTITY_H
#define SPRITE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/BoundingBoxDataCache.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Scene/Sprite.h>

namespace env
{

    class CSpriteEntity : public CEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            SPRITE_EVENT_SHOW = ENTITY_EVENT_END,
            SPRITE_EVENT_HIDE,
            SPRITE_EVENT_END,
            SPRITE_EVENT_SET_SCALING = SPRITE_EVENT_END + 200
        };

        CSpriteEntity();
        virtual ~CSpriteEntity();
        virtual void Update(float dt);
        virtual bool HasCollision(CEntity& entity);
        virtual bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);

        virtual void SetRenderEditor(bool bEditor);
        virtual void SetMaterialFilename(const std::string& strMaterial);

        bool GetRenderEditor() const { return m_bRenderEditor; }

        virtual void SetRenderGame(bool bGame);
        bool GetRenderGame() const { return m_bRenderGamemode; }

        virtual void SetRender(bool bRender);

        void SetTextPosition(const Vec2& vPosition);
        void SetText(const std::string& strText);

//         void SetFadeIn(float fade) { m_fadeInTime = fade; }
//         float GetFadeIn() const { return m_fadeInTime; }
// 
//         void SetFadeOut(float fade) { m_fadeOutTime = fade; }
//         float GetFadeOut() const { return m_fadeOutTime; }
// 
//         void SetApplyFade(bool bApply) { m_sprite.SetApplyFade(bApply); }
//         bool GetApplyFade() const { return m_sprite.GetApplyFade(); }
// 
//         void SetApplyPostProcessing(bool bApply) { m_sprite.SetApplyPostProcessing(bApply); }
//         bool GetApplyPostProcessing() const { return m_sprite.GetApplyPostProcessing(); }

        void SetScreenSpace(bool bApply);
        bool GetScreenSpace() const { return m_bScreenSpace; }

        void SetInitiallyVisible(bool bSet) { m_bInitiallyVisible = bSet; }
        bool GetInitiallyVisible() const { return m_bInitiallyVisible; }

        void SetScreenPosition(const Vec2& screenPos) { m_screenPosition = screenPos; }
        const Vec2& GetScreenPosition() const { return m_screenPosition; }

        void SetForceUpdate(bool bForceUpdate) { m_bForceUpdate = bForceUpdate; }
        bool GetForceUpdate() const { return m_bForceUpdate; }
        CSprite& GetSprite() { return m_sprite; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CEntity >(*this);
            ar & m_strTextureFilename;
            ar & m_vCenter;
            ar & m_screenPosition;
            ar & m_bAlphaBlending;
            ar & m_bBillboard;
            ar & m_strText;
            ar & m_pNode;
            ar & m_initialColor;
            ar & m_bRenderGamemode;
            ar & m_bRenderEditor;
            ar & m_bScreenSpace;
            ar & m_spriteRect;
            ar & m_fadeInTime;
            ar & m_fadeOutTime;
            ar & m_sprite;

            if (version > 0)
                ar & m_bInitiallyVisible;

            if (version < 2)
                m_screenPosition = Vec2(0.5f, 0.5f);

            m_version = version;
        }

        void InitBoundingBox();

    protected:
        CSprite m_sprite;
        CSceneNode m_nodeScreenSpace;

        CVector3Cache m_boundingBox;

        std::string m_strTextureFilename;
        std::string m_strText;

        RECT m_spriteRect;

        Vec3 m_vCenter;
        Vec2 m_screenPosition;
        Vec4 m_initialColor;


        bool m_bAlphaBlending;
        bool m_bBillboard;
        bool m_bScreenSpace;
        bool m_bRender;
        bool m_bRenderGamemode;
        bool m_bRenderEditor;
        bool m_bInitiallyVisible;
        bool m_bForceUpdate;

        unsigned int m_version;

        float m_currentAlphaTime;
        float m_alphaDuration;
        float m_alphaBegin;
        float m_alphaEnd;

        bool m_bScalingSet;

        Vec3 m_scalingBegin;
        Vec3 m_scalingEnd;
        Vec3 m_scalingInitial;

        float m_scalingDuration;
        float m_scalingTime;

        float m_fadeInTime;
        float m_fadeOutTime;
    };

    class CTriggerEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            TRIGGER_EVENT_SET_ENABLED = SPRITE_EVENT_END,
            TRIGGER_EVENT_END = TRIGGER_EVENT_SET_ENABLED + 10
        };

        CTriggerEntity();

        virtual ~CTriggerEntity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnTriggerEntered(CEntity& e);
        virtual void Update(float dt);
        virtual void SetMaterialFilename(const std::string& strMaterial);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
        }
    };

    class CCounterV2Entity : public CTriggerEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            COUNTER_EVENT_INCREMENT = TRIGGER_EVENT_END,
            COUNTER_EVENT_DESCREMENT
        };

        CCounterV2Entity();

        virtual ~CCounterV2Entity();
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void Update(float dt);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);


    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CTriggerEntity >(*this);
            ar & m_counter;
        }

        int m_counter;
        int m_currentCounter;
    };
} // env


#endif // SPRITE_ENTITY_H