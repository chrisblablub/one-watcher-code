/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef WEAPON_ENTITY_H
#define WEAPON_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{
    class CWeaponEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CWeaponEntity();
        virtual ~CWeaponEntity();

        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnEntityCloned(CEntity& entity);
        virtual void OnGameModeChanged(bool bGameMode);

        inline void SetRadius(float radius) { m_ChargingRange = radius; }
        inline float GetRadius() const { return m_ChargingRange; }

        inline void SetChargingDuration(float dur) { m_MaxCharge = dur; }
        inline float GetChargingDuration() const { return m_MaxCharge; }

        inline void SetBossLaserDuration(float dur) { m_bossLaserDuration = dur; }
        inline float GetBossLaserDuration() const { return m_bossLaserDuration; }

        void DoDamage();

    protected:
        void InitParticles();
        void InitSprites();
        void SetupSounds() override;
        void FireAtBoss();

        float m_Charge;
        float m_MaxCharge;
        float m_ChargingRange;

        CSprite m_loadingSprite;
        CSceneNode m_loadingSpriteNode;

        CSprite m_laserSprite;
        CSceneNode* m_pLaserNode;
        CParticleSystemEntity* m_pParticles;

    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CMeshEntity >(*this);

            if (version > 1)
            {
                ar & m_laserSprite;
                ar & m_pLaserNode;
            }

            if (version > 2)
            {
                ar & m_bossLaserDuration;
                ar & m_MaxCharge;
                ar & m_ChargingRange;
            }

            if (version > 3)
                ar & m_pParticles;
        }

        float m_bossLaserTime;
        float m_bossLaserDuration;

        bool m_bDisabled;
        bool m_bActive;
        bool m_bJustFired;
    };

} // env

#endif // WEAPON_ENTITY_H 