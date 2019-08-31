/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef POST_PROCESSING_ENTITY_H
#define POST_PROCESSING_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>

namespace env
{
    class CPostProcessingEntity : public CSpriteEntity
    {
    public: 
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS 
        { 
            POSTPROCESSING_EVENT_START = SPRITE_EVENT_END,
            POSTPROCESSING_EVENT_DISTORTIONLIGHT,
            POSTPROCESSING_EVENT_DISTORTIONSTRONG,
            POSTPROCESSING_EVENT_SET_SATURATION
        };

        CPostProcessingEntity();
        virtual ~CPostProcessingEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        void SetTargetBloomThreshold(float threshold) { m_targetBloomThreshold = threshold; }
        float GetTargetBloomThreshold() const { return m_targetBloomThreshold; }

        /*void SetEndBloomThreshold(float threshold) { m_endBloomThreshold = threshold; }
        float GetEndBloomThreshold() const { return m_endBloomThreshold; }*/

        void SetTargetBloomScale(float s) { m_targetBloomScale = s; }
        float GetTargetBloomScale() const { return m_targetBloomScale; }

        /*void SetEndBloomScale(float s) { m_endBloomScale = s; }
        float GetEndBloomScale() const { return m_endBloomScale; }*/


        void SetBloomFadeIn(float fadeIn) { m_fadeInTime = fadeIn; }
        float GetBloomFadeIn() const { return m_fadeInTime; }

        void SetBloomFadeInExponent(float e) { m_fadeInExponent = e; }
        float GetBloomFadeInExponent() const { return m_fadeInExponent; }
        /*void SetBloomFadeOut(float fade) { m_fadeOutTime = fade; }
        float GetBloomFadeOut() const { return m_fadeOutTime; }*/

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & m_targetBloomThreshold;
            ar & m_targetBloomScale;
            //ar & m_endBloomThreshold;
            //ar & m_endBloomScale;
            ar & m_fadeInTime;

            if(version > 0)
                ar & m_fadeInExponent;
            //ar & m_fadeOutTime;
        }

    protected:

        float m_targetBloomThreshold;
        float m_targetBloomScale;

        float m_startBloomThreshold;
        float m_startBloomScale;

        //float m_endBloomThreshold;
        //float m_endBloomScale;

        float m_fadeInTime;
        float m_fadeInExponent;
        //float m_fadeOutTime;


        float m_currentFadeInTime;
        //float m_currentFadeOutTime;

        float m_distortionOffset;
        float m_distortionAmount;
        float m_distortionTime;
        Vec2 m_distortionDirection;
        Vec2 m_distortionScaling;


        float m_currentSaturationTime;
        float m_saturationDuration;
        float m_saturationBegin;
        float m_saturationEnd;
        float m_currentSaturation;

    };



} // env

#endif // POST_PROCESSING_ENTITY_H