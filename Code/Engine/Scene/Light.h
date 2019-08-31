/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/AttachableObject.h>
#include <Engine/Core/Camera.h>
#include <Graphic/RenderObjects/LightRenderObject.h>
#include <Common/Core/BoundingVolume.h>

namespace env
{
    class CLight : public CAttachableObject
    {
    public:
        CLight();
        CLight(const std::string& strName, const Vec4& vDiffColor, const Vec4& vSpecColor, float SpotRange);
        ~CLight();

        void AddToRenderQueue(RenderQueue& renderQueue);
        void InitializeAtLoad();
        void InitializeAtCreation();
        void OnGameModeChanged();

        inline void SetType(LIGHTTYPE type) { m_type = type; }
        inline LIGHTTYPE GetType() const { return m_type; }

        void SetInitialDiffuseColor(const Vec4& vDiffColor) { m_initialDiffColor = vDiffColor; m_currentDiffColor = vDiffColor; }
        Vec4 GetInitialDiffuseColor() const { return m_initialDiffColor; }

        void SetDiffuseColor(const Vec4& vDiffColor) { m_currentDiffColor = vDiffColor; }
        Vec4 GetDiffuseColor() const { return m_currentDiffColor; }

        void SetInitialSpecularColor(const Vec4& vSpecColor) { m_initialSpecularColor = vSpecColor; m_currentSpecularColor = vSpecColor; }
        Vec4 GetInitialSpecularColor() const { return m_initialSpecularColor; }

        void SetSpecularColor(const Vec4& specColor) { m_currentSpecularColor = specColor; }
        Vec4 GetSpecularColor() const { return m_currentSpecularColor; }

        void SetAttenuationConstant(float att) { m_vAttenuation.x = att; }
        float GetAttenuationConstant() const { return m_vAttenuation.x; }

        void SetAttenuationLinear(float att) { m_vAttenuation.y = att; }
        float GetAttenuationLinear() const { return m_vAttenuation.y; }

        void SetAttenuationQuadratic(float att) { m_vAttenuation.z = att; }
        float GetAttenuationQuadratic() const { return m_vAttenuation.z; }

        void SetAttenuation(const Vec3& vAtt) { m_vAttenuation = vAtt; }
        Vec3 GetAttenuation() const { return m_vAttenuation; }

        void SetCastingShadows(bool bCasting = true) {}
        bool IsCastingShadows() const { return m_bShadows; }

        void SetCastingSoftShadows(bool bSoftShadows = true) { m_bSoftShadows = bSoftShadows; }
        bool IsCastingSoftShadows() const { return m_bSoftShadows; }

        void SetShadowSoftness(float softness) { m_softness = softness; }
        float GetShadowSoftness() const { return m_softness; }

        void SetShadowJitter(float jitter) { m_jitter = jitter; }
        float GetShadowJitter() const { return m_jitter; }

        void SetShadowSamples(unsigned int samples) { m_shadowSamples = samples; }
        unsigned int GetShadowSamples() const { return m_shadowSamples; }

        void SetShadowMapSize(unsigned int size) { m_shadowMapSize = size; }
        unsigned int GetShadowMapSize() const { return m_shadowMapSize; }

        void SetSpotInnerAngle(float rad);
        float GetSpotInnerAngle() const { return m_spotInnerAngle; }

        void SetSpotOuterAngle(float rad);
        float GetSpotOuterAngle() const { return m_spotOuterAngle; }

        void SetRenderInGame(bool bSet) { m_bRenderInGame = bSet; }
        bool GetRenderInGame() { return m_bRenderInGame; }

        void SetRenderInEditor(bool bSet) { m_bRenderInEditor = bSet; }
        bool GetRenderInEditor() { return m_bRenderInEditor; }

        void SetConeLength(float length) { m_coneLength = length; }
        float GetConeLength() { return m_coneLength; }

        void SetRadius(float radius) { m_radius = radius; }
        float GetRadius() { return m_radius; }

        void SetCone(bool bCone) { m_bLightCone = bCone; }
        bool GetCone() { return m_bLightCone; }

        void SetInitialEnergy(float energy) { m_initialEnergy = energy; m_currentEnergy = energy; }
        float GetInitialEnergy() const { return m_initialEnergy; }

        void SetEnergy(float energy) { m_currentEnergy = energy; }
        float GetEnergy() { return m_currentEnergy; }

        void SetLightRaysDecay(float dec) { m_lightRaysDecay = dec; }
        float GetLightRaysDecay() const { return m_lightRaysDecay; }

        void SetLightRaysIllumDecay(float dec) { m_lightRaysIllumDecay = dec; }
        float GetLightRaysIllumDecay() const { return m_lightRaysIllumDecay; }

        const std::string& GetName() const { return m_strName; }

        void SetSharpEdges(bool bSet) { m_bSharpEdges = bSet; }
        bool GetSharpEdges() const { return m_bSharpEdges; }

        void SetSpecular(bool bSet) { m_bSpecular = bSet; }
        bool GetSpecular() const { return m_bSpecular; }

        void SetStatic(bool bSet) { m_bStatic = bSet; }
        bool GetStatic() const { return m_bStatic; }

        void SetAffectsTerrain(bool bSet) { m_bAffectsTerrain = bSet; }
        bool GetAffectsTerrain() const { return m_bAffectsTerrain; }

        void SetAffectsMeshes(bool bSet) { m_bAffectsMeshes = bSet; }
        bool GetAffectsMeshes() const { return m_bAffectsMeshes; }

        void SetLightRays(bool bSet) { m_bLightRays = bSet; }
        bool GetLightRays() const { return m_bLightRays; }

        void SetLightRaysUseOcclusionMask(bool bSet) { m_bLightRaysUseOcclusionMask = bSet; }
        bool GetLightRaysUseOcclusionMask() const { return m_bLightRaysUseOcclusionMask; }

        void SetLightRaysUseWorldOrigin(bool bSet) { m_bLightRaysUseWorldOrigin = bSet; }
        bool GetLightRaysUseWorldOrigin() const { return m_bLightRaysUseWorldOrigin; }

        void Serialize(bool bSerialize = true) { m_bSerialize = bSerialize; }
        bool IsSerialized() const { return m_bSerialize; }

    private:

        friend class boost::serialization::access;


        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CAttachableObject >(*this);
            ar & m_strName;
            ar & m_spotInnerAngle;
            ar & m_spotOuterAngle;
            ar & m_softness;
            ar & m_jitter;
            ar & m_type;
            ar & m_bShadows;
            ar & m_bSoftShadows;
            ar & m_bSerialize;
            ar & m_shadowSamples;
            ar & m_shadowMapSize;
            ar & m_initialDiffColor;
            ar & m_initialSpecularColor;
            ar & m_vAttenuation;
            ar & m_pShadowCamNode;
            ar & m_shadowCamera;

            if (version > 0)
            {
                ar & m_coneLength;
                ar & m_radius;
                ar & m_bLightCone;
            }

            if (version > 1)
                ar & m_initialEnergy;

            if (version > 2)
                ar & m_bSharpEdges;

            if (version > 3)
                ar & m_bLightRays;

            if (version > 4)
                ar & m_bSpecular;

            if (version > 5)
            {
                ar & m_bLightRaysUseOcclusionMask;
                ar & m_bLightRaysUseWorldOrigin;
            }

            if (version > 6)
            {
                ar & m_lightRaysDecay;
                ar & m_lightRaysIllumDecay;
            }

            if (version > 7)
                ar & m_bStatic;

            if (version > 8)
                ar & m_bAffectsMeshes;
        }


    protected:
        std::string m_strName;

        CCamera m_shadowCamera;
        CSceneNode* m_pShadowCamNode;
        Vec4 m_initialDiffColor;
        Vec4 m_currentDiffColor;
        Vec4 m_initialSpecularColor;
        Vec4 m_currentSpecularColor;
        Vec3 m_vAttenuation;

        float m_softness;
        float m_jitter;
        float m_spotInnerAngle;
        float m_spotOuterAngle;
        float m_coneLength;
        float m_radius;
        float m_currentEnergy;
        float m_initialEnergy;
        float m_lightRaysDecay;
        float m_lightRaysIllumDecay;
        LIGHTTYPE m_type;

        bool m_bShadows;
        bool m_bSoftShadows;
        bool m_bSerialize;
        bool m_bLightCone;
        bool m_bRenderInGame;
        bool m_bRenderInEditor;
        bool m_bSharpEdges;
        bool m_bSpecular;
        bool m_bStatic;
        bool m_bLightRays;
        bool m_bAffectsMeshes;
        bool m_bAffectsTerrain;
        bool m_bLightRaysUseOcclusionMask;
        bool m_bLightRaysUseWorldOrigin;

        unsigned int m_shadowSamples;
        unsigned int m_shadowMapSize;

        LightRenderObject m_lightRenderObject;
    };

    inline bool IsLightPrecomputable(CLight* pLight)
    {
        return !pLight->GetSpecular() &&
            pLight->GetAffectsTerrain() &&
            pLight->GetStatic();
    }

    
} // env
#endif // LIGHT_H