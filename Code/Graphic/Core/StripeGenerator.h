/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/

#pragma once
#ifndef STRIPE_GENERATOR_H
#define STRIPE_GENERATOR_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/RenderObjects/StripeRenderObject.h>

namespace env
{
    class CStripeGenerator
    {
    public:
        virtual float Generate(std::vector<Stripe>& stripes) const = 0;
    };

    template<class Settings>
    class CBaseStripeGenerator : public CStripeGenerator
    {
    public:
        typedef Settings SettingsType;

        CBaseStripeGenerator(const Settings& settings)
            : m_settings(settings) {}

    protected:
        Settings m_settings;
    };

    struct StripeParameters
    {
        StripeParameters()
            : m_numSlices(10)
            , m_aspectRatio(1.0f)
            , m_timeSpawnOffset(0.025f)
            , m_timeSpawnDuration(0.1f)
            , m_timeFadeDuration(0.0f)
            , m_startWidth(0.0f)
            , m_endWidth(1.0f)
            , m_bGrowAlternatingly(false)
            , m_bGrowVertically(false)
            , m_bFadeOut(false)
            , m_startTopColor(0.0f, 0.0f, 0.0f, 1.0f) 
            , m_startBotColor(0.0f, 0.0f, 0.0f, 1.0f)
            , m_endTopColor(0.0f, 0.0f, 0.0f, 1.0f)
            , m_endBotColor(0.0f, 0.0f, 0.0f, 1.0f) {}

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_numSlices;
            ar & m_aspectRatio;
            ar & m_timeSpawnOffset;
            ar & m_timeSpawnDuration;
            ar & m_startWidth;
            ar & m_endWidth;
            ar & m_bGrowAlternatingly;
            ar & m_bGrowVertically;
            ar & m_bFadeOut;
            ar & m_startTopColor;
            ar & m_startBotColor;
            ar & m_endTopColor;
            ar & m_endBotColor;

            if (version > 0)
                ar & m_timeFadeDuration;
        }

        unsigned int m_numSlices;

        float m_aspectRatio;
        float m_timeSpawnOffset;
        float m_timeSpawnDuration;
        float m_timeFadeDuration;
        float m_startWidth;
        float m_endWidth;

        bool m_bGrowAlternatingly;
        bool m_bGrowVertically;
        bool m_bFadeOut;

        Vec4 m_startTopColor;
        Vec4 m_startBotColor;
        Vec4 m_endTopColor;
        Vec4 m_endBotColor;

        void SaveTemplate(CScript& script)
        {
            script["numSlices"] = (int)m_numSlices;
            script["timeSpawnOffset"] = m_timeSpawnOffset;
            script["timeSpawnDuration"] = m_timeSpawnDuration;
            script["timeFadeDuration"] = m_timeFadeDuration;
            script["startWidth"] = m_startWidth;
            script["endWidth"] = m_endWidth;
            script["growAlternatingly"] = m_bGrowAlternatingly;
            script["growVertically"] = m_bGrowVertically;
            script["fadeOut"] = m_bFadeOut;
            script["startTopColor"] = m_startTopColor;
            script["startBotColor"] = m_startBotColor;
            script["endTopColor"] = m_endTopColor;
            script["endBotColor"] = m_endBotColor;
        }

        void LoadTemplate(CScript& script)
        {
            m_numSlices = (int)script["numSlices"];
            m_timeSpawnOffset = script["timeSpawnOffset"];
            m_timeSpawnDuration = script["timeSpawnDuration"];
            m_timeFadeDuration = script["timeFadeDuration"];
            m_startWidth = script["startWidth"];
            m_endWidth = script["endWidth"];
            m_bGrowAlternatingly = script["growAlternatingly"];
            m_bGrowVertically = script["growVertically"];
            m_bFadeOut = script["fadeOut"];
            m_startTopColor = script["startTopColor"];
            m_startBotColor = script["startBotColor"];
            m_endTopColor = script["endTopColor"];
            m_endBotColor = script["endBotColor"];
        }
    };

    class CDefaultStripeGenerator : public CBaseStripeGenerator<StripeParameters>
    {
    public:
        CDefaultStripeGenerator(const StripeParameters& settings)
            : CBaseStripeGenerator<StripeParameters>(settings) {}

        virtual float Generate(vecStripes& stripes) const;

        void GetStripeDistributionParams(unsigned int stripeIndex,
                                         const StripeParameters& settings,
                                         float& topCoordX, float& botCoordX,
                                         float& topCoordY, float& botCoordY,
                                         float& width) const;
    };

    class CMidStripeGenerator
    {

    };

    class CStripeManager
    {
    public:
        float Generate(const CStripeGenerator& generator);
        void AddToRenderQueue(RenderQueue& renderQueue);

        const StripeRenderObject& GetStripeRenderObject() const { return m_stripeRenderObject; }

    private:
        StripeRenderObject m_stripeRenderObject;
    };

    
} // env
#endif // STRIPE_GENERATOR_H