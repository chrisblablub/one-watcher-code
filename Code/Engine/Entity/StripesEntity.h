/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef STRIPE_ENTITY_H
#define STRIPE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Graphic/RenderObjects/StripeRenderObject.h>
#include <Graphic/Core/StripeGenerator.h>

namespace env
{

    class CStripesEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            STRIPE_EVENT_EXECUTE = SPRITE_EVENT_END,
            STRIPE_EVENT_SET_SPAWN_DURATION,
            STRIPE_EVENT_SET_SPAWN_OFFSET,
            STRIPE_EVENT_SET_WIDTH_START,
            STRIPE_EVENT_SET_WIDTH_END,
            STRIPE_EVENT_SET_GROW_VERTICALLY,
            STRIPE_EVENT_SET_GROW_ALTERNATINGLY,
            STRIPE_EVENT_SET_PARAMETER,
            STRIPE_EVENT_RESET,
            STRIPE_EVENT_SET_FADE_DURATION,
            STRIPE_EVENT_SET_FADE_OUT,
            STRIPE_EVENT_SET_LOADING_SCREEN
        };

        CStripesEntity();
        virtual ~CStripesEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnEntityCloned(CEntity& entity);
        virtual void OnInput(unsigned int eventID, const EventParameter& eventParameter);
        virtual void OnGameModeChanged(bool bGameMode);

        inline void SetStripeParameters(const StripeParameters& stripeParams) { m_defaultStripeParams = stripeParams; GenerateStripes(); }
        inline const StripeParameters& GetStripeParameters() const { return m_defaultStripeParams; }

        void SaveTemplate(const std::string& strFilename, const std::string& strName);
        void LoadTemplate(const std::string& strFilename);

        void SetInitFromTemplate(bool bSet) { m_bInitTemplate = bSet; }
        bool GetInitFromTemplate() const { return m_bInitTemplate; }

        const std::string& GetTemplate() const { return m_strTemplate; }

    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_defaultStripeParams;

            if (version > 0)
            {
                ar & m_strTemplate;
                ar & m_bInitTemplate;
            }
        }

        void GenerateStripes();
        void ExecStripes();

    private:

        StripeParameters   m_defaultStripeParams;
        StripeParameters   m_stripeParams;
        StripeRenderObject m_stripeRenderObject;
        CStripeManager     m_stripeManager;

        float m_currentStripeTime;
        float m_stripeTime;

        bool m_bInitTemplate;
        std::string m_strTemplate;
        
    };
} // env

#endif // STRIPE_ENTITY_H 