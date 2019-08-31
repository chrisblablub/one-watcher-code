/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SCENARIO_H
#define SCENARIO_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/Light.h>
#include <Graphic/GraphicConfig.h>
#include <Graphic/Core/Texture.h>
#include <Graphic/RenderObjects/BloomRenderObject.h>
#include <Graphic/RenderObjects/EnvironmentRenderObject.h>
#include <Graphic/RenderObjects/PostProcessRenderObject.h>

namespace env
{

    struct Skyline
    {
        Skyline()
            : depth(0.0f), posX(0.0f), posY(0.0f),
            color(1.0f, 1.0f, 1.0f),
            scale(1.0f),
            strTexture("Skylines/Skyline08_layer2.png"),
            pTexture(NULL),
            bRepeatX(true),
            bAbsolutePosition(false),
            fogColorParam(1.0f) {}

        std::string strTexture;
        float depth;
        float posX;
        float posY;
        float scale;
        float fogColorParam;

        bool bRepeatX;
        bool bAbsolutePosition;

        Vec3 color;

        CTextureManaged* pTexture;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & strTexture;
            ar & depth;
            ar & posY;
            ar & scale;
            ar & color;

            if (version > 0)
            {
                ar & bRepeatX;
                ar & posX;
            }

            if (version > 1)
                ar & fogColorParam;

            if (version > 2)
                ar & bAbsolutePosition;
        }
    };

    struct Sky
    {
        Sky()
            : color(1.0f, 1.0f, 1.0f) {}

        std::string strSkyTexture;
        std::string strGradientTexture;
        Vec3 color;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & strSkyTexture;
            ar & strGradientTexture;
            ar & color;
        }
    };

    struct Fog
    {
        Fog()
            : fogRange(250.0f)
            , fogStart(4500.0f)
            , initialFogColor(187.0f / 255.0f, 170.0f / 255.0f, 159.0f / 255.0f, 1.0f)
            , fogColor(187.0f / 255.0f, 170.0f / 255.0f, 159.0f / 255.0f, 1.0f)
            , initialSkyColor(0.2f, 0.2f, 0.8f)
            , skyColor(0.2f, 0.2f, 0.8f) {}

        float fogRange;
        float fogStart;
        Vec4 initialFogColor;
        Vec4 fogColor;
        Vec3 initialSkyColor;
        Vec3 skyColor;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & fogRange;
            ar & fogStart;
            ar & initialFogColor;

            if (version > 0)
                ar & initialSkyColor;
        }
    };



    struct LevelBorderOffsets
    {
        LevelBorderOffsets()
            : left(0),
            right(0),
            top(0),
            bottom(0) {}

        unsigned int left;
        unsigned int right;
        unsigned int top;
        unsigned int bottom;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & left;
            ar & right;
            ar & top;
            ar & bottom;
        }
    };

    struct RenderQueue;

    class CLight;

    class CScenario
    {
    public:
        CScenario();
        ~CScenario();

        void AddToRenderQueue(RenderQueue& renderQueue);
        void InitializeAtCreation();
        void InitializeAtLoad();
        void OnGameModeChanged(bool bGameMode);

        Fog& GetFog() { return m_fog; }

        void SetSkyline(unsigned int layer, const Skyline& skyline);
        Skyline& GetSkyline(unsigned int layer);
        std::vector< Skyline >& GetSkylines() { return m_skylines; }

        void SetTerrainDiffuseColor(unsigned int layer, Vec3 vColor);
        const Vec3& GetTerrainDiffuseColor(unsigned int layer);

        void SetTerrainEmissiveColor(unsigned int layer, const Vec4& vColor);
        const Vec4& GetTerrainEmissiveColor(unsigned int layer);

        void SetSilhouettes(bool bSilhouettes) { m_bSilhouettes = bSilhouettes; }
        bool GetSilhouettes() const { return m_bSilhouettes; }

        void SetSilhouetteIntensity(float strength) { m_silhouetteIntensity = strength; }
        float GetSilhouetteIntensity() const { return m_silhouetteIntensity; }

        void SetSilhouetteColor(const Vec4& vColor) { m_silhouetteColor = vColor; }
        const Vec4& GetSilhouetteColor() const { return m_silhouetteColor; }

        void SetContrast(float contrast, bool bNormal = true) { bNormal ? m_contrast = contrast : m_contrastSlowMo = contrast; }
        float GetContrast(bool bNormal = true) { return (bNormal ? m_contrast : m_contrastSlowMo); }

        void SetSaturation(float saturation, bool bNormal = true) { bNormal ? m_saturation = saturation : m_saturationSlowMo = saturation; }
        float GetSaturation(bool bNormal = true) { return (bNormal ? m_saturation : m_saturationSlowMo); }

        void SetSepia(float sepia, bool bNormal = true) { bNormal ? m_sepia = sepia : m_sepiaSlowMo = sepia; }
        float GetSepia(bool bNormal = true) { return (bNormal ? m_sepia : m_sepiaSlowMo); }

        void SetHueRotation(float hue, bool bNormal = true) { bNormal ? m_hueRotation = hue : m_hueRotationSlowMo = hue; }
        float GetHueRotation(bool bNormal = true) { return (bNormal ? m_hueRotation : m_hueRotationSlowMo); }

        void SetColorBalance(const Vec3& colorBalance) { m_colorBalance = colorBalance; }
        const Vec3& GetColorBalance() { return m_colorBalance; }

        void SetColorBrightness(const Vec3& colorBrightness) { m_colorBrightness = colorBrightness; }
        const Vec3& GetColorBrightness() { return m_colorBrightness; }

        void SetWeather(SCENARIO_WEATHER weather) { m_weather = weather; }
        SCENARIO_WEATHER GetWeather() const { return m_weather; }

        void SetCurrentBloomScale(float s) { m_currentBloomScale = s; }
        float GetCurrentBloomScale() const { return m_currentBloomScale; }

        void SetBloomScale(float scale) { m_bloomScale = scale; m_currentBloomScale = scale; }
        float GetBloomScale() { return m_bloomScale; }

        void SetCurrentBloomThreshold(float s) { m_currentBloomThreshold = s; }
        float GetCurrentBloomThreshold() const { return m_currentBloomThreshold; }

        void SetBloomBrightpassThreshold(float threshold) { m_bloomBrightpassThreshold = threshold; m_currentBloomThreshold = threshold; }
        float GetBloomBrightpassThreshold() { return m_bloomBrightpassThreshold; }

        void SetBloomBrightpassOffset(float offset) { m_bloomBrightpassOffset = offset; }
        float GetBloomBrightpassOffset() { return m_bloomBrightpassOffset; }

        void SetBorderTexture(const std::string& strBorder) { m_strBorderTexture = strBorder; }
        const std::string& GetBorderTexture() { return m_strBorderTexture; }

        void SetBorderColor(const Vec3& color) { m_borderColor = color; }
        const Vec3& GetBorderColor() const { return m_borderColor; }

        LevelBorderOffsets& GetLevelBorderOffsets() { return m_levelBorderOffsets; }

        void SetCloudIntensity(float intensity) { m_cloudIntensity = intensity; }
        float GetCloudIntensity() const { return m_cloudIntensity; }

        void SetEnergyFieldIntensity(float intensity) { m_currentEnergyFieldIntensity = intensity; }
        float GetEnergyFieldIntensity() const { return m_currentEnergyFieldIntensity; }

        void SetInitialEnergyFieldIntensity(float intensity) { m_initialEnergyFieldIntensity = intensity; m_currentEnergyFieldIntensity = intensity; }
        float GetInitialEnergyFieldIntensity() const { return m_initialEnergyFieldIntensity; }



    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            static Vec3 dummy_VECTOR3 = Vec3(0.0f, 0.0f, 0.0f);
            static Vec4 dummy_VECTOR4 = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
            static bool dummy_bool = false;
            static float dummy_float = 0.0f;
            static unsigned int dummy_unsigned_int = 0;
            static CLight* pSunLight = 0;
            static CSceneNode* pSunLightNode = 0;

            ar & dummy_VECTOR4;
            ar & dummy_VECTOR4;
            ar & dummy_bool;
            ar & dummy_unsigned_int;
            ar & dummy_unsigned_int;
            ar & pSunLight;
            ar & pSunLightNode;
            ar & dummy_VECTOR3;
            ar & dummy_VECTOR3;
            ar & dummy_VECTOR3;
            ar & dummy_VECTOR3;
            ar & dummy_bool;
            ar & m_silhouetteIntensity;
            ar & dummy_VECTOR4;
            ar & m_contrast;
            ar & m_saturation;
            ar & m_bSilhouettes;
            ar & m_bSilhouettes;
            ar & m_silhouetteColor;
            ar & m_sepia;
            ar & m_hueRotation;
            ar & m_colorBalance;
            ar & m_colorStandardWeights;
            ar & m_colorBrightness;
            ar & m_bloomScale;
            ar & m_bloomBrightpassOffset;
            ar & m_bloomBrightpassThreshold;
            ar & m_sepiaSlowMo;
            ar & m_hueRotationSlowMo;
            ar & m_saturationSlowMo;
            ar & m_contrastSlowMo;
            ar & dummy_float;
            ar & dummy_bool;
            ar & m_fog;
            ar & m_sky;
            ar & m_skylines;

            if (version > 0)
                ar & m_strBorderTexture;

            if (version > 1)
                ar & m_levelBorderOffsets;

            if (version > 2)
                ar & m_terrainDiffuseColors;

            if (version > 3)
                ar & dummy_float;

            if (version > 4)
                ar & m_weather;

            if (version > 5)
                ar & m_cloudIntensity;

            if (version > 6)
                ar & m_initialEnergyFieldIntensity;

            if (version > 7)
                ar & m_terrainEmissiveColors;

            if (version > 8)
                ar & m_borderColor;
        }


    private:

        bool m_bSilhouettes;

        float m_contrast;
        float m_saturation;
        float m_sepia;
        float m_hueRotation;
        float m_contrastSlowMo;
        float m_saturationSlowMo;
        float m_sepiaSlowMo;
        float m_hueRotationSlowMo;
        float m_bloomScale;
        float m_bloomBrightpassThreshold;
        float m_bloomBrightpassOffset;
        float m_currentBloomScale;
        float m_currentBloomThreshold;
        float m_silhouetteIntensity;
        float m_cloudIntensity;
        float m_initialEnergyFieldIntensity;
        float m_currentEnergyFieldIntensity;

        Vec3 m_colorBalance;
        Vec3 m_colorStandardWeights;
        Vec3 m_colorBrightness;
        Vec4 m_silhouetteColor;
        Vec3 m_borderColor;

        Fog m_fog;

        SCENARIO_WEATHER m_weather;

        std::vector< Skyline > m_skylines;
        Sky m_sky;
        std::string m_strSkyColorTexture;
        std::string m_strSkyGradientTexture;
        CTextureManaged* m_pSkyColorTexture;
        CTextureManaged* m_pSkyGradientTexture;

        std::string m_strBorderTexture;
        std::vector< Vec3 > m_terrainDiffuseColors;
        std::vector< Vec4 > m_terrainEmissiveColors;

        LevelBorderOffsets m_levelBorderOffsets;


        BloomRenderObject m_bloomRenderObject;
        EnvironmentRenderObject m_environmentRenderObject;

    };
} // env
#endif // SCENARIO_H 