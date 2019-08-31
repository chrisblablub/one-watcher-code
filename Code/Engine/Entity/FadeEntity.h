/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FADE_ENTITY_H
#define FADE_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Graphic/RenderObjects/FadeRenderObject.h>

namespace env
{
    class CFadeEntity : public CSpriteEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        struct FadeFrameParams
        {
            FadeFrameParams()
                : m_fadeFrameDuration(0.0f)
                , m_fadeFrameDelay(0.0f)
                , m_fadeFrameParamsBegin(0, 0, 0, 0)
                , m_fadeFrameParamsEnd(0, 0, 0, 0)
                , m_fadeFrameBeginColorTop(0, 0, 0, 1)
                , m_fadeFrameBeginColorBot(0, 0, 0, 1)
                , m_fadeFrameEndColorTop(0, 0, 0, 1)
                , m_fadeFrameEndColorBot(0, 0, 0, 1) {}

            template< class Archive > void serialize(Archive& ar, const unsigned int version)
            {
                ar & m_fadeFrameDuration;
                ar & m_fadeFrameDelay;
                ar & m_fadeFrameParamsBegin;
                ar & m_fadeFrameParamsEnd;

                if (version > 0)
                {
                    ar & m_fadeFrameBeginColorTop;
                    ar & m_fadeFrameBeginColorBot;
                    ar & m_fadeFrameEndColorTop;
                    ar & m_fadeFrameEndColorBot;
                }
                
            }

            void SaveTemplate(CScript& script)
            {
                script["fadeFrameDuration"] = m_fadeFrameDuration;
                script["fadeFrameDelay"] = m_fadeFrameDelay;
                script["fadeFrameParamsBegin"] = m_fadeFrameParamsBegin;
                script["fadeFrameParamsEnd"] = m_fadeFrameParamsEnd;
                script["fadeFrameBeginColorTop"] = m_fadeFrameBeginColorTop;
                script["fadeFrameBeginColorBot"] = m_fadeFrameBeginColorBot;
                script["fadeFrameEndColorTop"] = m_fadeFrameEndColorTop;
                script["fadeFrameEndColorBot"] = m_fadeFrameEndColorBot;
            }

            void LoadTemplate(CScript& script)
            {
                m_fadeFrameDuration = script["fadeFrameDuration"];
                m_fadeFrameDelay = script["fadeFrameDelay"];
                m_fadeFrameParamsBegin = script["fadeFrameParamsBegin"];
                m_fadeFrameParamsEnd = script["fadeFrameParamsEnd"];
                m_fadeFrameBeginColorTop = script["fadeFrameBeginColorTop"];
                m_fadeFrameBeginColorBot = script["fadeFrameBeginColorBot"];
                m_fadeFrameEndColorTop = script["fadeFrameEndColorTop"];
                m_fadeFrameEndColorBot = script["fadeFrameEndColorBot"];
            }

            Vec4 m_fadeFrameParamsBegin;
            Vec4 m_fadeFrameParamsEnd;
            Vec4 m_fadeFrameBeginColorTop;
            Vec4 m_fadeFrameBeginColorBot;
            Vec4 m_fadeFrameEndColorTop;
            Vec4 m_fadeFrameEndColorBot;

            float m_fadeFrameDuration;
            float m_fadeFrameDelay;
        };

        enum TRIGGERABLE_EVENTS
        {
            FADE_EVENT_RESET = SPRITE_EVENT_END,
            FADE_EVENT_SCREEN,
            FADE_EVENT_FRAME_SET_PARAMS,
            FADE_EVENT_BARS,
            FADE_EVENT_FRAME_EXECUTE
//             FADE_EVENT_FADE_BARS,
//             FADE_EVENT_DISABLEBARS,
//             FADE_EVENT_FADEOUT_BARS,
//             FADE_EVENT_FADEIN_BARS
        };

        CFadeEntity();
        virtual ~CFadeEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void OnGameModeChanged(bool bGameMode);

        void SetFadeScreenTime(float time) { m_fadeScreenTime = time; }
        float GetFadeScreenTime() const { return m_fadeScreenTime; }

//         void SetFadeScreenBegin(float am) { m_fadeScreenBegin = am; }
//         float GetFadeScreenBegin() const { return m_fadeScreenBegin; }
// 
//         void SetFadeScreenEnd(float amount) { m_fadeScreenEnd = amount; }
//         float GetFadeScreenEnd() const { return m_fadeScreenEnd; }
// 
//         void SetFadeBeginColor(const Vec3& color) { m_fadeBeginColorTop = color; }
//         const Vec3& GetFadeBeginColor() const { return m_fadeBeginColorTop; }
// 
//         void SetFadeEndColor(const Vec3& color) { m_fadeEndColorTop = color; }
//         const Vec3& GetFadeEndColor() const { return m_fadeEndColorTop; }


        void SetScreenDuration(float duration) { m_screenDuration = duration; }
        float GetScreenDuration() const { return m_screenDuration; }
        //void SetFadeOutTime(float time) { m_fadeOutTime = time; }
        //float GetFadeOutTime() const { return m_fadeOutTime; }


        FadeFrameParams GetDefaultFadeFrameParams() const { return m_defaultFadeFrameParams; }
        void SetDefaultFadeFrameParams(FadeFrameParams& params) { m_defaultFadeFrameParams = params; }

        void SaveTemplate(const std::string& strFilename, const std::string& strName);
        void LoadTemplate(const std::string& strFilename);

        void SetInitFromTemplate(bool bSet) { m_bInitTemplate = bSet; }
        bool GetInitFromTemplate() const { return m_bInitTemplate; }

        const std::string& GetTemplate() const { return m_strTemplate; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            Vec3 dummyVec3;
            float dummyFloat;

            ar & boost::serialization::base_object< CSpriteEntity >(*this);
            ar & dummyFloat;
            ar & dummyFloat;
            ar & dummyVec3;

            if (version > 0)
            {
                ar & dummyVec3;
                ar & m_fadeScreenTime;
            }

            if (version > 1)
                ar & m_screenDuration;

            if (version > 2)
            {
                ar & m_defaultFadeFrameParams;
                ar & m_strTemplate;
                ar & m_bInitTemplate;
            }
        }

    private:

        bool m_bInitTemplate;
        std::string m_strTemplate;

        FadeFrameParams m_defaultFadeFrameParams;
        FadeFrameParams m_currentFadeFrameParams;

        float m_currentFadeFrameTime;
        float m_currentFadeFrameDelay;
        

        Vec4 m_fadeBeginColorTop;
        Vec4 m_fadeBeginColorBot;
        Vec4 m_fadeEndColorTop;
        Vec4 m_fadeEndColorBot;

        float m_screenDuration;
        float m_fadeScreenTime;
        float m_fadeBarsTime;
//         float m_fadeScreenBegin;
//         float m_fadeScreenEnd;

        float m_currentScreenTime;
        float m_currentFadeScreenTime;
//        bool m_bFadeScreenEnabled;

        Vec4 m_currentFadeColorTop;
        Vec4 m_currentFadeColorBot;

        float m_currentFadeBarsTime;
        Vec2 m_FadeBarsAmount;
        Vec4 m_fadeBarsBeginColor;
        Vec4 m_fadeBarsEndColor;
        int m_fadeBarsType;
        
//         bool m_bFadeOutBars;
//         float m_currentFadeOutBarsTime;
// 
//         bool m_bFadeInBars;
//         float m_currentFadeInBarsTime;
//        bool m_bBars;


        /*float m_fadeOutTime;



        bool m_bFadeInBars;
        bool m_bFadeOutBars;

        float m_currentFadeBarTarget;



        float m_currentFadeInBarsTime;
        float m_currentFadeOutBarsTime;*/

        FadeBarsRenderObject m_currentFade;

    };
} // env

#endif // FADE_ENTITY_H 