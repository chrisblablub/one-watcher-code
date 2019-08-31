/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SKY_DOME_ENTITY_H
#define SKY_DOME_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{
    class CSkyDomeEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS { };

        CSkyDomeEntity();
        virtual ~CSkyDomeEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);
        virtual void SetMeshFilename(const std::string& strName);

        void SetGradientColor1(const Vec3& clr) { m_gradientColor1 = clr; }
        const Vec3& GetGradientColor1() const { return m_gradientColor1; }

        void SetGradientColor2(const Vec3& clr) { m_gradientColor2 = clr; }
        const Vec3& GetGradientColor2() const { return m_gradientColor2; }

        void SetEnergyFieldColor(const Vec3& clr) { m_energyFieldColor = clr; }
        const Vec3& GetEnergyFieldColor() const { return m_energyFieldColor; }

        float GetCurrentEnergyFieldParam() const { return m_currentEnergyFieldParam; }

        void SetSkyTextureName(const std::string& strTex);
        const std::string& GetSkyTextureName() const { return m_strSkyTexture; }
        CTextureManaged* GetSkyTexture() { return m_pSkyTexture; }

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 0)
                ar & m_energyFieldColor;

            if (version > 1)
                ar & m_strSkyTexture;
        }

    protected:
        std::string m_strSkyTexture;
        CTextureManaged* m_pSkyTexture;

        Vec3 m_gradientColor1;
        Vec3 m_gradientColor2;

        float m_currentEnergyFieldParam;

        float m_currentEnergyFieldTime;
        float m_flickerTime;
        float m_flickerTargetTime;
        float m_flickerAmount;
        float m_flickerRandomness;
        float m_flickerPauseTime;

        bool m_bEnergyField;

        Vec3 m_energyFieldColor;

        SkyDomeRenderObject m_skyDomeRenderObject;
    };


    class CHaloEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS 
        { 
            HALO_EVENT_SHOW = MESH_EVENT_END, 
            HALO_EVENT_HIDE, 
            HALO_EVENT_STARTSHOW, 
            HALO_EVENT_STARTHIDE, 
            HALO_EVENT_SET_INTENSITY,
            HALO_EVENT_SET_INTENSITY_LEFT,
            HALO_EVENT_SET_INTENSITY_RIGHT
        };

        CHaloEntity();
        virtual ~CHaloEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void AddToRenderQueue(float dt, RenderQueue& renderQueue);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);
        }

    private:
        HaloRenderObject m_haloRenderObject;

        float m_startShowTime;
        float m_startHideTime;

        float m_intensityLeftTime;
        float m_intensityLeftDuration;
        float m_intensityRightTime;
        float m_intensityRightDuration;

        float m_beginIntensityLeft;
        float m_endIntensityLeft;

        float m_beginIntensityRight;
        float m_endIntensityRight;
    };


} // env

#endif // SKY_DOME_ENTITY_H 