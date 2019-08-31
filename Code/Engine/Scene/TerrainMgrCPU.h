/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TERRAIN_CPU_H
#define TERRAIN_CPU_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/TerrainCollidable.h>
#include <Engine/Scene/TerrainCell.h>
#include <Graphic/Core/Material.h>
#include <Engine/Scene/TerrainMgrGPU.h>


namespace env
{

    struct TerrainPatternMaterial
    {
        TerrainPatternMaterial()
            : diffuse(1.0f, 1.0f, 1.0f),
            specular(0.0f, 0.0f, 0.0f, 25.0f) {}

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & diffuse;
            ar & specular;
        }

        Vec3 diffuse;
        Vec4 specular;
    };

    typedef std::vector< TerrainPatternMaterial > vecPatternMaterials;

    class CTerrainMaterial
    {
    public:
        CTerrainMaterial()
            : m_pMaterial(NULL)
            , m_emissiveColor(1.0f, 1.0f, 1.0f, 1.0f) {}

        CTerrainMaterial(const CTerrainMaterial& other);

        ~CTerrainMaterial();

        const CTerrainMaterial& operator=(const CTerrainMaterial& rhs);

        void InitializeAtLoad();

        void SetMaterial(const std::string& strMaterial);
        const std::string GetMaterial() { return m_strMaterial; }
        CManagedMaterial* GetMaterialPtr() { return m_pMaterial; }

        void SetName(const std::string& strName) { m_strName = strName; }
        const std::string& GetName() { return m_strName; }

        void SetMaterialDiffuse(unsigned int pattern, const Vec3& diffuse);
        Vec3 GetMaterialDiffuse(unsigned int pattern);
        void GetMaterialsDiffuse(std::vector< Vec3 >& diffuse);

        void SetMaterialSpecular(unsigned int pattern, const Vec4& specular);
        Vec4 GetMaterialSpecular(unsigned int pattern);
        void GetMaterialsSpecular(std::vector< Vec4 >& specular);

        void SetEmissiveMtrl(const Vec4& emissive) { m_emissiveColor = emissive; }
        const Vec4& GetEmissiveMtrl() const { return m_emissiveColor; }

        void SetEnvMtrl(const Vec4& envColor) { m_envColor = envColor; }
        const Vec4& GetEnvMtrl() const { return m_envColor; }

        void UpdateMaterial();

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_terrainPatternMaterials;
            ar & m_strMaterial;
            ar & m_strName;

            if (version > 0)
                ar & m_emissiveColor;

            if (version > 1)
                ar & m_envColor;
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & m_terrainPatternMaterials;
            ar & m_strMaterial;
            ar & m_strName;

            if (version > 0)
                ar & m_emissiveColor;

            if (version > 1)
                ar & m_envColor;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    private:

        CManagedMaterial* m_pMaterial;

        std::string m_strMaterial;
        std::string m_strName;

        Vec4 m_emissiveColor;
        Vec4 m_envColor;

        vecPatternMaterials m_terrainPatternMaterials;
    };

    const std::string REGISTERED_RESOURCES = "RegisteredResources.txt";

    typedef std::list< CTerrainMaterial > listTerrainMaterials;

    class CTerrainMgrCPU
    {
    public:
        CTerrainMgrCPU();
        ~CTerrainMgrCPU();

        void InitializeAtLoad();
        void InitializeAtCreation(unsigned int cellsX, unsigned int cellsY);

        void SetEditSequence(int sequence);
        void SetCell(unsigned int x, unsigned int y, bool bStickyCeiling = false);
        void SetCell(unsigned int x, unsigned int y, LevelHalfCell* pFirst, LevelHalfCell* pSecond);
        void SetCellAnimationPattern(unsigned int x, unsigned int y, unsigned int sequence, unsigned int frame, bool bFirst = true);

        void SetLayerDepth(int layer, float depth);
        float GetLayerDepth(int layer);
        unsigned int GetNumLayers() const { return m_layerDepths.size(); }

        void SetLayerMaxDepth(float d);
        float GetLayerMaxDepth() const { return m_maxDepth; }
        std::vector<float>& GetLayerDepths() { return m_layerDepths; }

        void SetBorderColorPreset(unsigned int preset, const Vec3& color);
        void SetActiveBorderColorPreset(unsigned int preset);

        void SetNumCellsX(unsigned int x) { m_numCellsX = x; m_sizeX = (float)m_numCellsX * CELL_SIZE; }
        unsigned int GetNumCellsX() const { return m_numCellsX; }

        void SetNumCellsY(unsigned int y) { m_numCellsY = y; m_sizeY = (float)m_numCellsY * CELL_SIZE; }
        unsigned int GetNumCellsY() const { return m_numCellsY; }

        float GetSizeX() const { return m_sizeX; }
        float GetSizeY() const { return m_sizeY; }
        void GetSize(unsigned int& gridsX, unsigned int& gridsY, float& width);

        void SetWireframe(bool bSet);
        bool GetWireframe();

        LevelCellEditParameters& GetCellEditParameters() { return m_editParameters; }

        bool FindCell(TerrainCell** ppGrid, int x, int y);

        void UnsetCell(unsigned int x, unsigned int y);
        void UnsetBothCells(unsigned int x, unsigned int y);

        void GetRotatedTexCoords(Vec2& outV1, Vec2& outV2, Vec2& outV3, Vec2& outV4, CELL_ROTATION rot);

        void ApplyBorderColor(const Vec3& color);
        void ApplyDiffuseColor(unsigned int layer, const Vec3& color);
        void ApplyEmissiveColor(unsigned int layer, const Vec4& color);

        const Vec3& GetBorderColorPreset(int layer);

        void SetLevelSize(unsigned int numCellsX, unsigned int numCellsY);
        void OffsetLevel(int offsetX, int offsetY);

        CTerrainMaterial& GetMaterial() { return m_material; }
        void SetMaterial(const std::string& strMaterial);

        mapHalfCells& GetHalfCells() { return m_halfCells; }
        mapCells& GetCells() { return m_cells; }

        void ResetPainterCells();

        void SetBorderRotationAngle(float angle);
        float GetBorderRotationAngle() const { return m_borderRotationAngle; }
        const Vec3& GetBorderRotationCenter() const { return m_borderRotationCenter; }
        void SetBorderRotationCenter(const Vec3& center);

        void Paint(bool bPaint, unsigned int cellX, unsigned int cellY, const Vec2& mouseIntersection, float dt);

        CTerrainMgrGPU* GetLevelRenderObject() { return m_pRenderObject; }

        void SetFirstMaterial(const std::string& strAnim);
        const std::string& GetFirstMaterialName() const { return m_strFirstMaterial; }

        void SetSecondMaterial(const std::string& strAnim);
        const std::string& GetSecondMaterialName() const { return m_strSecondMaterial; }

        void SetActiveMaterial(unsigned int anim) { m_activeMaterial = anim; }
        unsigned int GetActiveMaterial() const { return m_activeMaterial; }

        void SetFirstMaterialScale(float scale) { m_firstScale = scale;  }
        float GetFirstMaterialScale() const { return m_firstScale; }

        void SetSecondMaterialScale(float scale) { m_secondScale = scale; }
        float GetSecondMaterialScale() const { return m_secondScale; }

        void SetBrushIntensity(float intensity) { m_brushIntensity = intensity; }
        float GetBrushIntensity() const { return m_brushIntensity; }

        void SetPainterStrength(float intensity) { m_painterStrength = intensity; }
        float GetPainterStrength() const { return m_painterStrength; }

        void SetBrushRadius(float radius) { m_brushRadius = radius; }
        float GetBrushRadius() const { return m_brushRadius; }

        void SetPainterFirstColor(const Vec3& clr) { m_painterFirstColor = clr;  }
        const Vec3& GetPainterFirstColor() const { return m_painterFirstColor; }

        void SetPainterSecondColor(const Vec3& clr) { m_painterSecondColor = clr; }
        const Vec3& GetPainterSecondColor() const { return m_painterSecondColor; }

        

    private:
        void RemoveHalfCell(unsigned int index, LevelHalfCell* ptr);

        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_numCellsX;
            ar & m_numCellsY;
            ar & m_halfCells;
            ar & m_maxDepth;
            ar & m_borderRotationAngle;
            ar & m_borderRotationCenter;
            ar & m_layerDepths;
            ar & m_borderColorPresets;
            ar & m_material;

            if (version > 0)
            {
                ar & m_strFirstMaterial;
                ar & m_strSecondMaterial;
                ar & m_brushRadius;
                ar & m_brushIntensity;
                ar & m_firstScale;
                ar & m_secondScale;
                ar & m_painterVertices;
            }

            if (version > 1)
            {
                ar & m_painterFirstColor;
                ar & m_painterSecondColor;
            }

            if (version > 2)
                ar & m_painterStrength;

        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & m_numCellsX;
            ar & m_numCellsY;
            ar & m_halfCells;
            ar & m_maxDepth;
            ar & m_borderRotationAngle;
            ar & m_borderRotationCenter;
            ar & m_layerDepths;
            ar & m_borderColorPresets;
            ar & m_material;

            if (version > 0)
            {
                ar & m_strFirstMaterial;
                ar & m_strSecondMaterial;
                ar & m_brushRadius;
                ar & m_brushIntensity;
                ar & m_firstScale;
                ar & m_secondScale;
                ar & m_painterVertices;
            }

            if (version > 1)
            {
                ar & m_painterFirstColor;
                ar & m_painterSecondColor;
            }

            if (version > 2)
                ar & m_painterStrength;

            m_brushIntensity = 50;
            SetNumCellsX(m_numCellsX);
            SetNumCellsY(m_numCellsY);
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()



    private:
        std::string m_strFirstMaterial;
        std::string m_strSecondMaterial;

        CManagedMaterial* m_pPainterFirstMaterial;
        CManagedMaterial* m_pPainterSecondMaterial;

        int m_activeMaterial;

        float m_brushRadius;
        float m_brushIntensity;
        float m_firstScale;
        float m_secondScale;
        float m_vertexDistance;
        float m_painterStrength;

        unsigned int m_numCellsX;
        unsigned int m_numCellsY;
        unsigned int m_currentBorderColorPreset;

        float m_sizeX;
        float m_sizeY;
        float m_maxDepth;
        float m_borderRotationAngle;



        Vec3 m_borderRotationCenter;
        Vec3 m_painterFirstColor;
        Vec3 m_painterSecondColor;


        LevelCellEditParameters m_editParameters;

        mapCells m_cells;
        mapHalfCells m_halfCells;
        vecMapPainterVertices m_painterVertices;
        //vecvecPainterVertices m_gridPainterVertices;

        std::vector<float> m_layerDepths;
        std::vector<Vec3> m_borderColorPresets;

        CTerrainMaterial m_material;
        CTerrainMgrGPU* m_pRenderObject;
    };
} // env
#endif // TERRAIN_CPU_H