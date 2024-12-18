/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/Texture.h>

namespace env
{
    enum MESH_RENDER_CATEGORY
    {
        MESH_RENDER_CATEGORY_DEFERRED = 0,
        MESH_RENDER_CATEGORY_FORWARD = 1,
        MESH_RENDER_CATEGORY_CHARACTER = 2,
        MESH_RENDER_CATEGORY_OUTLINE = 3,
        MESH_RENDER_CATEGORY_TRANSMISSIVE = 4,
        MESH_RENDER_CATEGORY_DEFERRED_LAYER0 = 5,
    };


    typedef unsigned long MeshRenderCategoryMask;

    extern const MESH_RENDER_CATEGORY gMeshRenderCategories[6];

#define MESH_RENDER_MODE_COUNT (sizeof(gMeshRenderCategories) / sizeof(gMeshRenderCategories[0]))

    inline MeshRenderCategoryMask RenderCategory2Bitmask(MESH_RENDER_CATEGORY renderCategory) { return 1 << (renderCategory + 1); }

    class CMaterial
    {
    public:

        static std::string GetNormalMapFilename(const std::string& strTexture);

        CMaterial();
        CMaterial(const CMaterial& other);
        CMaterial& operator=(const CMaterial& other);

        virtual ~CMaterial();

        bool Unload();
        void Update(float dt);

        inline bool HasTexture() const { return m_pTexture != nullptr; }
        inline bool HasNormalMap() const { return m_pNormalMap != nullptr; }
        inline bool HasSpecularMap() const { return m_pSpecularMap != nullptr; }
        inline bool HasEmissiveMap() const { return m_pEmissiveMap != nullptr; }

        inline CTextureManaged* GetTexture() const { return m_pTexture; }
        inline CTextureManaged* GetNormalMap() const { return m_pNormalMap; }
        inline CTextureManaged* GetSpecularMap() const { return m_pSpecularMap; }
        inline CTextureManaged* GetEmissiveMap() const { return m_pEmissiveMap; }

        inline void LoadTexture(const std::string& strName, const std::string& strSampler, bool bSRGB) { Load(m_pTexture, strName, strSampler, bSRGB); }
        inline void LoadNormalMap(const std::string& strName, const std::string& strSampler, bool bSRGB) { Load(m_pNormalMap, strName, strSampler, bSRGB); }
        inline void LoadSpecularMap(const std::string& strName, const std::string& strSampler, bool bSRGB) { Load(m_pSpecularMap, strName, strSampler, bSRGB); }
        inline void LoadEmissiveMap(const std::string& strName, const std::string& strSampler, bool bSRGB) { Load(m_pEmissiveMap, strName, strSampler, bSRGB); }

        inline const std::string& GetName() const { return m_strName; }
        inline const std::string& GetFilename() const { return m_strFilename; }

        bool SetSequence(unsigned int sequence) { if (sequence > m_numSequences) return false; m_currentSequence = sequence; return true; }
        inline unsigned int GetSequence() const { return m_currentSequence; }

        bool SetFrame(unsigned int frame) { if (frame > m_numFrames) return false; m_currentFrame = frame; return true; }
        inline unsigned int GetFrame() const { return m_currentFrame; }

        inline unsigned int GetNumFrames() const { return m_numFrames; }
        inline unsigned int GetNumSequences() const { return m_numSequences; }
        inline int GetFrameHeight() const { return m_frameHeight; }
        inline int GetFrameWidth() const { return m_frameWidth; }
        inline float GetFrametime() const { return m_frameTime; }

        void SetFrametime(float frametime) { m_frameTime = frametime; }

        bool GetTextureCoordinates(unsigned int sequence, unsigned int frame, float& x1, float& y1, float& x2, float& y2);
        bool GetTextureCoordinates(float& x1, float& y1, float& x2, float& y2);
        void GetTextureCoordinates(unsigned int sequence, unsigned int frame, TextureRect& textureRect);
        void GetTextureCoordinates(TextureRect& textureRect);

        unsigned int GetWidth();
        unsigned int GetHeight();

        bool IsNormalMap(const std::string& strFile);
        bool IsSpecularMap(const std::string& strFile);
        bool IsEmissiveMap(const std::string& strFile);

        inline void SetDiffuseColor(const Vec3& diffuseColor) { m_diffuseColor = diffuseColor; }
        inline const Vec3& GetDiffuseColor() const { return m_diffuseColor; }

        inline const Vec3& GetOutlineColor() const { return m_outlineColor; }

        inline void SetDiffuseEnergy(float diffuseEnergy) { m_diffuseEnergy = diffuseEnergy; }
        inline float GetDiffuseEnergy() const { return m_diffuseEnergy; }

        inline void SetSpecularColor(const Vec3& specularColor) { m_specularColor = specularColor; }
        inline const Vec3& GetSpecularColor() const { return m_specularColor; }

        inline void SetEmissiveColor(const Vec3& emissiveColor) { m_emissiveColor = emissiveColor; }
        inline const Vec3& GetEmissiveColor() const { return m_emissiveColor; }

        inline void SetEmissiveEnergy(float emissiveEnergy) { m_emissiveEnergy = emissiveEnergy; }
        inline float GetEmissiveEnergy() const { return m_emissiveEnergy; }

        inline void SetEnvColor(const Vec3& envColor) { m_envColor = envColor; }
        inline const Vec3& GetEnvColor() const { return m_envColor; }

        inline void SetEnvReflectivity(float envReflectivity) { m_envReflectivity = envReflectivity; }
        inline float GetEnvReflectivity() const { return m_envReflectivity; }

        inline void SetShininess(float shi) { m_shininess = shi; }
        inline float GetShininess() const { return m_shininess; }

        inline void SetOpacity(float opa) { m_opacity = opa; }
        inline float GetOpacity() const { return m_opacity; }

        bool LoadTemplate(const std::string& strFile);
        bool LoadTemplate(CScript& script);
        void SaveTemplate(const std::string& strFilename, const std::string& strFile);
        void SaveTemplate(CScript& script);

        // Move to CMaterial!
        inline void SetMaterialTemplate(const std::string& strMaterialTemplate) { m_strMaterialTemplate = strMaterialTemplate; }
        inline const std::string& GetMaterialTemplate() const { return m_strMaterialTemplate; }

        inline void SetMaterialTemplateInit(bool bMaterialTemplate) { m_bMaterialTemplateInit = bMaterialTemplate; }
        inline bool GetMaterialTemplateInit() const { return m_bMaterialTemplateInit; }

        inline bool HasBeenSerialized() const { return m_bHasBeenSerialized; }


        inline void SetRender(bool bSet) { SetRenderInGame(bSet); SetRenderInEditor(bSet); }
        inline void SetRenderInGame(bool bSet) { m_bRenderInGame = bSet; }
        inline bool GetRenderInGame() const { return m_bRenderInGame; }

        inline void SetRenderCategory(MESH_RENDER_CATEGORY meshRenderCategory) { m_renderCategoryMask = RenderCategory2Bitmask(meshRenderCategory); }
        inline void AddRenderCategory(MESH_RENDER_CATEGORY meshRenderCategory) { m_renderCategoryMask |= RenderCategory2Bitmask(meshRenderCategory); }
        inline void RemoveRenderCategory(MESH_RENDER_CATEGORY meshRenderCategory) { m_renderCategoryMask &= ~RenderCategory2Bitmask(meshRenderCategory); }
        inline bool IsRenderCategorySet(MESH_RENDER_CATEGORY meshRenderCategory) { return (m_renderCategoryMask & RenderCategory2Bitmask(meshRenderCategory)); }
        inline void SetRenderCategoryMask(MeshRenderCategoryMask mask) { m_renderCategoryMask = mask; }
        inline MeshRenderCategoryMask GetRenderCategoryMask() const { return m_renderCategoryMask; }
        inline MESH_RENDER_CATEGORY GetRenderCategory() const { return m_renderMode; }

        inline void SetRenderInEditor(bool bSet) { m_bRenderInEditor = bSet; }
        inline bool GetRenderInEditor() const { return m_bRenderInEditor; }

        inline void SetDirectionalLighting(bool bDirectionalLighting) { m_bNoDirectionalLighting = !bDirectionalLighting; }
        inline bool GetDirectionalLighting() const { return !m_bNoDirectionalLighting; }

        inline void SetMotionBlur(bool bMotionBlur) { m_bMotionBlur = bMotionBlur; }
        inline bool GetMotionBlur() const { return m_bMotionBlur; }

        inline void SetMotionBlurScaling(float intensity) { m_motionBlurScaling = intensity; }
        inline float GetMotionBlurScaling() const { return m_motionBlurScaling; }

        inline void SetOutlineColor(const Vec3& c) { m_outlineColor = c; }


    private:
        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & m_strFilename;
            ar & m_strName;
            ar & m_diffuseColor;
            ar & m_specularColor;
            ar & m_emissiveColor;
            ar & m_envColor;
            ar & m_diffuseEnergy;
            ar & m_emissiveEnergy;
            ar & m_opacity;
            ar & m_shininess;
            ar & m_envReflectivity;
            ar & m_frameWidth;
            ar & m_frameHeight;
            ar & m_numFrames;
            ar & m_numSequences;
            ar & m_posLeft;
            ar & m_posTop;
            ar & m_bHasMaterial;
            ar & m_bHasAnimation;
            ar & m_bMaterialTemplateInit;
            ar & m_strMaterialTemplate;

            if (version > 0)
            {
                ar & m_bNoDirectionalLighting;
                ar & m_bMotionBlur;
                ar & m_bRenderInGame;
                ar & m_bRenderInEditor;
                ar & m_renderMode;
                ar & m_renderCategoryMask;
                ar & m_motionBlurScaling;
            }
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            m_bHasBeenSerialized = true;

            ar & m_strFilename;
            ar & m_strName;
            ar & m_diffuseColor;
            ar & m_specularColor;
            ar & m_emissiveColor;
            ar & m_envColor;
            ar & m_diffuseEnergy;
            ar & m_emissiveEnergy;
            ar & m_opacity;
            ar & m_shininess;
            ar & m_envReflectivity;
            ar & m_frameWidth;
            ar & m_frameHeight;
            ar & m_numFrames;
            ar & m_numSequences;
            ar & m_posLeft;
            ar & m_posTop;
            ar & m_bHasMaterial;
            ar & m_bHasAnimation;
            ar & m_bMaterialTemplateInit;
            ar & m_strMaterialTemplate;

            if (version > 0)
            {
                ar & m_bNoDirectionalLighting;
                ar & m_bMotionBlur;
                ar & m_bRenderInGame;
                ar & m_bRenderInEditor;
                ar & m_renderMode;
                ar & m_renderCategoryMask;
                ar & m_motionBlurScaling;
            }

            if (m_bMaterialTemplateInit)
            {
                // Load from template
                LoadTemplate(m_strMaterialTemplate);
            }
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER();

    protected:

        std::string m_strMaterialTemplate;

        bool m_bMaterialTemplateInit;
        bool m_bHasBeenSerialized;

        void LoadTextureFromScript(CScript* pScript, const std::string& strName, CTextureManaged*& pTex);

        bool Load(CTextureManaged*& pTex, const std::string& strFile, const std::string& strSampler, bool bSRGB);

        std::string m_strFilename;
        std::string m_strName;

        CTextureManaged* m_pTexture;
        CTextureManaged* m_pNormalMap;
        CTextureManaged* m_pSpecularMap;
        CTextureManaged* m_pEmissiveMap;

        Vec3 m_diffuseColor;
        Vec3 m_specularColor;
        Vec3 m_emissiveColor;
        Vec3 m_outlineColor;
        Vec3 m_envColor;

        float m_diffuseEnergy;
        float m_emissiveEnergy;
        float m_opacity;
        float m_shininess;
        float m_envReflectivity;

        float m_frameTime;
        float m_currentTimeElapsed;

        int m_frameWidth;
        int m_frameHeight;


        // PATCH
        bool m_bNoDirectionalLighting;
        bool m_bMotionBlur;
        bool m_bRenderInGame;
        bool m_bRenderInEditor;

        MESH_RENDER_CATEGORY m_renderMode;

        MeshRenderCategoryMask m_renderCategoryMask;
        float m_motionBlurScaling;
        //

        unsigned int m_numFrames;
        unsigned int m_numSequences;
        unsigned int m_posLeft;
        unsigned int m_posTop;
        unsigned int m_currentFrame;
        unsigned int m_currentSequence;

        bool m_bHasMaterial;
        bool m_bHasAnimation;
    };

    class CManagedMaterial : public CMaterial
    {
    public:
        typedef CResMgrBase< std::string, CManagedMaterial > CMaterialMgr;
        CManagedMaterial();


        virtual ~CManagedMaterial();

        bool InitResource(const std::string& strFilename, const std::string& str);

    protected:
        bool LoadResource();

    private:
        friend class CResMgrBase< std::string, CManagedMaterial >;

        bool InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt);
    };

    typedef CManagedMaterial::CMaterialMgr CMaterialMgr;

    extern CMaterialMgr* gMaterialMgr;




} // env
#endif // MATERIAL_H
