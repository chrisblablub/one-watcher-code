/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LIGHT_SPRITE_ENTITY_H
#define LIGHT_SPRITE_ENTITY_H


#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Graphic/RenderObjects/LightRaysRenderObject.h>

namespace env
{
    class CLight;

    class CLightSpriteEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            LIGHT_EVENT_ON = SPRITE_EVENT_END,
            LIGHT_EVENT_OFF,
            LIGHT_EVENT_END,
            LIGHT_EVENT_SET_DIFFUSE_COLOR = LIGHT_EVENT_END + 50,
            LIGHT_EVENT_SET_SPECULAR_COLOR,
            LIGHT_EVENT_SET_LIGHTRAYS,
            LIGHT_EVENT_SET_ENERGY
        };

        CLightSpriteEntity();
        virtual ~CLightSpriteEntity();

        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);

        CLight* GetLight();

        void SetFlicker(bool bFlicker) { m_bFlicker = bFlicker; }
        bool GetFlicker() const { return m_bFlicker; }

        void SetFlickerTime(float time) { m_flickerTime = time; }
        float GetFlickerTime() const { return m_flickerTime; }

        void SetFlickerPauseTime(float pause) { m_flickerPauseTime = pause; }
        float GetFlickerPauseTime() const { return m_flickerPauseTime; }

        void SetFlickerAmount(float amount) { m_flickerAmount = amount; }
        float GetFlickerAmount() const { return m_flickerAmount; }

        void SetFlickerRandomness(float randomness) { m_flickerRandomness = randomness; }
        float GetFlickerRandomness() const { return m_flickerRandomness; }

        void SaveTemplate(const std::string& strFilename, const std::string& strName);
        void LoadTemplate(const std::string& strFilename, bool bInitMaterial = true);

        void SetInitFromTemplate(bool bSet) { m_bInitFromTemplate = bSet; }
        bool GetInitFromTemplate() const { return m_bInitFromTemplate; }

        void SetTemplate(const std::string& strTemplate) { m_strTemplate = strTemplate; }
        const std::string GetTemplate() const { return m_strTemplate; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_pLight;
            ar & m_bFlicker;

            if (version > 0)
            {
                ar & m_flickerTime;
                ar & m_flickerPauseTime;
                ar & m_flickerAmount;
                ar & m_flickerRandomness;
            }

            if (version > 1)
            {
                ar & m_bInitFromTemplate;
                ar & m_strTemplate;
            }
        }

    protected:
        CLight* m_pLight;

        bool m_bFlicker;
        bool m_bFlickering;
        bool m_bInitFromTemplate;

        bool m_bLightraysInitial;
        bool m_bLightraysSet;

        std::string m_strTemplate;

        float m_flickerTime;
        float m_flickerPauseTime;
        float m_flickerAmount;
        float m_flickerRandomness;

        float m_flickerTargetTime;
        float m_flickerCurrentTime;

        float m_lightOffTime;
        float m_lightOnTime;

        Vec3 m_diffuseColorBegin;
        Vec3 m_diffuseColorEnd;

        float m_diffuseDuration;
        float m_diffuseTime;

        Vec3 m_specularColorBegin;
        Vec3 m_specularColorEnd;

        float m_specularDuration;
        float m_specularTime;

        float m_energyBegin;
        float m_energyEnd;

        float m_energyDuration;
        float m_energyTime;

        LightRaysRenderObject m_lightRaysRenderObject;

    };


} // env

#endif // LIGHT_SPRITE_ENTITY_H