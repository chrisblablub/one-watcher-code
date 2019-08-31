#include <Graphic/Core/Material.h>

namespace env
{
    CMaterialMgr* gMaterialMgr = nullptr;

    //-----------------------------------------------------------------------------------
    CMaterial::CMaterial()
        : m_pTexture(nullptr)
        , m_pNormalMap(nullptr)
        , m_pSpecularMap(nullptr)
        , m_pEmissiveMap(nullptr)
        , m_bHasMaterial(false)
        , m_bHasAnimation(false)
        , m_bMaterialTemplateInit(false)
        , m_bHasBeenSerialized(false)
        , m_currentFrame(0)
        , m_currentSequence(0)
        , m_currentTimeElapsed(0.0f)
        , m_frameWidth(0)
        , m_frameHeight(0)
        , m_frameTime(0.0f)
        , m_numFrames(1)
        , m_numSequences(1)
        , m_posLeft(0)
        , m_posTop(0)
        , m_diffuseColor(1.0f, 1.0f, 1.0f)
        , m_specularColor(1.0f, 1.0f, 1.0f)
        , m_emissiveColor(1.0f, 1.0f, 1.0f)
        , m_diffuseEnergy(1.0f)
        , m_envColor(1.0f, 1.0f, 1.0f)
        , m_shininess(40.0f)
        , m_opacity(1.0f)
        , m_emissiveEnergy(1.0f)
        , m_envReflectivity(0.3f)
    
        , m_renderMode(MESH_RENDER_CATEGORY_DEFERRED)
        , m_renderCategoryMask(RenderCategory2Bitmask(MESH_RENDER_CATEGORY_DEFERRED))
        , m_bNoDirectionalLighting(false)
        , m_bMotionBlur(false)
        , m_bRenderInEditor(true)
        , m_bRenderInGame(true)
        , m_motionBlurScaling(1.0f) {}

    //-----------------------------------------------------------------------------------
    CMaterial::CMaterial(const CMaterial& other)
        : CMaterial()
    {
        operator=(other);
    }

    //-----------------------------------------------------------------------------------
    CMaterial& CMaterial::operator=(const CMaterial& other)
    {
        ENV_ASSERT_DBG(this != &other);

        if (this == &other)
            return *this;

        gTextureMgr->Release(m_pTexture);
        gTextureMgr->Release(m_pNormalMap);
        gTextureMgr->Release(m_pSpecularMap);
        gTextureMgr->Release(m_pEmissiveMap);

        m_pTexture = other.m_pTexture;
        m_pNormalMap = other.m_pNormalMap;
        m_pSpecularMap = other.m_pSpecularMap;
        m_pEmissiveMap = other.m_pEmissiveMap;
        m_bHasMaterial = other.m_bHasMaterial;
        m_bHasAnimation = other.m_bHasAnimation;
        m_bMaterialTemplateInit = other.m_bMaterialTemplateInit;
        m_bHasBeenSerialized = other.m_bHasBeenSerialized;
        m_strMaterialTemplate = other.m_strMaterialTemplate;
        m_currentFrame = other.m_currentFrame;
        m_currentSequence = other.m_currentSequence;
        m_currentTimeElapsed = other.m_currentTimeElapsed;
        m_frameWidth = other.m_frameWidth;
        m_frameHeight = other.m_frameHeight;
        m_frameTime = other.m_frameTime;
        m_numFrames = other.m_numFrames;
        m_numSequences = other.m_numSequences;
        m_posLeft = other.m_posLeft;
        m_posTop = other.m_posTop;
        m_diffuseColor = other.m_diffuseColor;
        m_specularColor = other.m_specularColor;
        m_emissiveColor = other.m_emissiveColor;
        m_diffuseEnergy = other.m_diffuseEnergy;
        m_opacity = other.m_opacity;
        m_envColor = other.m_envColor;
        m_shininess = other.m_shininess;
        m_emissiveEnergy = other.m_emissiveEnergy;
        m_envReflectivity = other.m_envReflectivity;
        m_renderMode = other.m_renderMode;
        m_renderCategoryMask = other.m_renderCategoryMask;
        m_bNoDirectionalLighting = other.m_bNoDirectionalLighting;
        m_bMotionBlur = other.m_bMotionBlur;
        m_bRenderInEditor = other.m_bRenderInEditor;
        m_bRenderInGame = other.m_bRenderInGame;
        m_motionBlurScaling = other.m_motionBlurScaling;

        if (m_pTexture)
        {
            // Increase the reference counter on copying, because
            // initialize is not called!
            CTextureManaged* p = gTextureMgr->AddGetPtr(m_pTexture->GetName());

            ENV_ASSERT_DBG(p == m_pTexture);
        }

        if (m_pNormalMap)
        {
            // Increase the reference counter on copying, because
            // initialize is not called!
            CTextureManaged* p = gTextureMgr->AddGetPtr(m_pNormalMap->GetName());

            ENV_ASSERT_DBG(p == m_pNormalMap);
        }

        if (m_pSpecularMap)
        {
            // Increase the reference counter on copying, because
            // initialize is not called!
            CTextureManaged* p = gTextureMgr->AddGetPtr(m_pSpecularMap->GetName());

            ENV_ASSERT_DBG(p == m_pSpecularMap);
        }

        if (m_pEmissiveMap)
        {
            // Increase the reference counter on copying, because
            // initialize is not called!
            CTextureManaged* p = gTextureMgr->AddGetPtr(m_pEmissiveMap->GetName());

            ENV_ASSERT_DBG(p == m_pEmissiveMap);
        }

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CMaterial::~CMaterial()
    {
        Unload();
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::Unload()
    {
        if (m_pEmissiveMap)
            gTextureMgr->Release(m_pEmissiveMap->GetName());

        if (m_pSpecularMap)
            gTextureMgr->Release(m_pSpecularMap->GetName());

        if (m_pNormalMap)
            gTextureMgr->Release(m_pNormalMap->GetName());

        if (m_pTexture)
            gTextureMgr->Release(m_pTexture->GetName());

        m_pTexture = nullptr;
        m_pNormalMap = nullptr;
        m_pSpecularMap = nullptr;
        m_pEmissiveMap = nullptr;

        m_bHasMaterial = false;
        m_bHasAnimation = false;
        m_currentFrame = 0;
        m_currentSequence = 0;
        m_currentTimeElapsed = 0.0f;
        m_frameWidth = 0;
        m_frameHeight = 0;
        m_frameTime = 0.0f;
        m_numFrames = 1;
        m_numSequences = 1;
        m_posLeft = 0;
        m_posTop = 0;

        m_renderMode = MESH_RENDER_CATEGORY_DEFERRED;
        m_renderCategoryMask = RenderCategory2Bitmask(MESH_RENDER_CATEGORY_DEFERRED);
        m_bNoDirectionalLighting = false;
        m_bMotionBlur = false;
        m_bRenderInEditor = true;
        m_bRenderInGame = true;
        m_motionBlurScaling = 1.0f;

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::Load(CTextureManaged*& pTex, const std::string& strFile, const std::string& strSampler, bool bSRGB)
    {
        if (strFile.empty())
            return false;

        if (pTex)
            gTextureMgr->Release(pTex->GetFilename());

        CTextureMgr::AllocPolicyType& allocPolicy = gTextureMgr->GetAllocPolicy();
        bool bCurSRGB = allocPolicy.m_bSRGB;
        allocPolicy.m_bSRGB = bSRGB;

        pTex = gTextureMgr->AddGetPtr(strFile);

        if (pTex)
            pTex->SetTextureSampler(CDefaultTextureSampler::GetSampler(strSampler));

        allocPolicy.m_bSRGB = bCurSRGB;

        m_frameWidth  = (pTex != nullptr) ? pTex->GetWidth() : 0;
        m_frameHeight = (pTex != nullptr) ? pTex->GetHeight() : 0;

        return pTex != nullptr;
    }

    //-----------------------------------------------------------------------------------
    void SaveTexture(CScript* pScript, const std::string& strName, CTextureManaged* pTex)
    {
        CScript* pTextureScript = pScript->GetSubscript(strName);
        if (!pTextureScript)
            pTextureScript = pScript->AddSubscript(strName);

        if (pTextureScript && pTex)
        {
            (*pTextureScript)[strName] = pTex->GetName();
            (*pTextureScript)["srgb"] = pTex->IsSRGB();
            (*pTextureScript)["sampler"] = CDefaultTextureSampler::GetSamplerName(pTex->GetTextureSampler());
        }
        else
            pScript->RemoveSubscript(strName);
    }

    //-----------------------------------------------------------------------------------
    void CMaterial::SaveTemplate(CScript& script)
    {
        SaveTexture(&script, "texture", m_pTexture);
        SaveTexture(&script, "normalmap", m_pNormalMap);
        SaveTexture(&script, "specularmap", m_pSpecularMap);
        SaveTexture(&script, "emissivemap", m_pEmissiveMap);

        CScript* pAnimationScript = script.GetSubscript("animation");
        if (!pAnimationScript)
            pAnimationScript = script.AddSubscript("animation");

        if (pAnimationScript)
        {
            (*pAnimationScript)["posLeft"] = static_cast<int>(m_posLeft);
            (*pAnimationScript)["posTop"] = static_cast<int>(m_posTop);
            (*pAnimationScript)["Sequences"] = static_cast<int>(m_numSequences);
            (*pAnimationScript)["FrameCount"] = static_cast<int>(m_numFrames);
            (*pAnimationScript)["FrameWidth"] = m_frameWidth;
            (*pAnimationScript)["FrameHeight"] = m_frameHeight;
            (*pAnimationScript)["FrameTime"] = m_frameTime;
        }

        script["diffuse"] = m_diffuseColor;
        script["diffuseEnergy"] = m_diffuseEnergy;
        script["emissive"] = m_emissiveColor;
        script["emissiveEnergy"] = m_emissiveEnergy;
        script["env"] = m_envColor;
        script["envReflectivity"] = m_envReflectivity;
        script["specular"] = m_specularColor;
        script["specularPower"] = m_shininess;
        script["opacity"] = m_opacity;
        script["materialTemplateInit"] = m_bMaterialTemplateInit;
        script["renderInGame"] = GetRenderInGame();
        script["renderInEditor"] = GetRenderInEditor();
        script["directionalLighting"] = GetDirectionalLighting();
        script["motionBlur"] = GetMotionBlur();
        script["motionBlurScaling"] = GetMotionBlurScaling();
        script["categorySolid"] = IsRenderCategorySet(MESH_RENDER_CATEGORY_DEFERRED);
        script["categoryTransmissive"] = IsRenderCategorySet(MESH_RENDER_CATEGORY_TRANSMISSIVE);
    }

    //-----------------------------------------------------------------------------------
    void CMaterial::SaveTemplate(const std::string& strFilename, const std::string& strFile)
    {
        CScript script;

        SetMaterialTemplate(strFile);

        SaveTemplate(script);

        script.SaveScript(strFilename);
    }

    //-----------------------------------------------------------------------------------
    void CMaterial::LoadTextureFromScript(CScript* pScript, const std::string& strName, CTextureManaged*& pTex)
    {
        CScript* pTextureScript = pScript->GetSubscript(strName);
        if (pTextureScript)
        {
            if (pTextureScript->Exists(strName))
            {
                std::string strTexture = (*pTextureScript)[strName];
                std::string strSampler = pTextureScript->Exists("sampler") ? ((*pTextureScript)["sampler"]).AsString() : "LinearWrap";
                bool bSRGB = pTextureScript->Exists("srgb") ? (*pTextureScript)["srgb"] : true;

                Load(pTex, strTexture, strSampler, bSRGB);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::LoadTemplate(CScript& script)
    {
        Unload();

        LoadTextureFromScript(&script, "texture", m_pTexture);
        LoadTextureFromScript(&script, "normalmap", m_pNormalMap);
        LoadTextureFromScript(&script, "specularmap", m_pSpecularMap);
        LoadTextureFromScript(&script, "emissivemap", m_pEmissiveMap);

        CScript* pAnimationScript = script.GetSubscript("animation");
        if (pAnimationScript)
        {
            ENV_ASSERT_DBG(pAnimationScript->Exists("posLeft"));
            ENV_ASSERT_DBG(pAnimationScript->Exists("posTop"));
            ENV_ASSERT_DBG(pAnimationScript->Exists("Sequences"));
            ENV_ASSERT_DBG(pAnimationScript->Exists("FrameCount"));
            ENV_ASSERT_DBG(pAnimationScript->Exists("FrameWidth"));
            ENV_ASSERT_DBG(pAnimationScript->Exists("FrameHeight"));
            ENV_ASSERT_DBG(pAnimationScript->Exists("FrameTime"));

            m_posLeft = (*pAnimationScript)["posLeft"];
            m_posTop = (*pAnimationScript)["posTop"];
            m_numSequences = (*pAnimationScript)["Sequences"];
            m_numFrames = (*pAnimationScript)["FrameCount"];
            m_frameWidth = (*pAnimationScript)["FrameWidth"];
            m_frameHeight = (*pAnimationScript)["FrameHeight"];
            m_frameTime = (*pAnimationScript)["FrameTime"];

            m_bHasAnimation = true;

            ENV_ASSERT_DBG(m_numSequences > 0);
            ENV_ASSERT_DBG(m_numFrames > 0);
        }
        else
            m_bHasAnimation = false;

        if (script.Exists("directionalLighting"))
            SetDirectionalLighting(script["directionalLighting"]);

        if (script.Exists("diffuse"))
            m_diffuseColor = script["diffuse"];

        if (script.Exists("diffuseEnergy"))
            m_diffuseEnergy = script["diffuseEnergy"];

        if (script.Exists("emissive"))
            m_emissiveColor = script["emissive"];

        if (script.Exists("emissiveEnergy"))
            m_emissiveEnergy = script["emissiveEnergy"];

        if (script.Exists("specular"))
            m_specularColor = script["specular"];

        if (script.Exists("specularPower"))
            m_shininess = script["specularPower"];

        if (script.Exists("env"))
            m_envColor = script["env"];

        if (script.Exists("envReflectivity"))
            m_envReflectivity = script["envReflectivity"];

        if (script.Exists("opacity"))
            m_opacity = script["opacity"];

        if (script.Exists("materialTemplateInit"))
            m_bMaterialTemplateInit = script["materialTemplateInit"];

        if (script.Exists("renderInGame"))
            SetRenderInGame(script["renderInGame"]);

        if (script.Exists("renderInEditor"))
            SetRenderInEditor(script["renderInEditor"]);

        if (script.Exists("motionBlur"))
            SetMotionBlur(script["motionBlur"]);

        if (script.Exists("motionBlurScaling"))
            SetMotionBlurScaling(script["motionBlurScaling"]);

        m_renderCategoryMask = 0;

        if (script.Exists("categorySolid") &&
            (bool)script["categorySolid"] == true)
        {
            AddRenderCategory(MESH_RENDER_CATEGORY_DEFERRED);
        }

        if (script.Exists("categoryTransmissive") &&
            (bool)script["categoryTransmissive"] == true)
        {
            AddRenderCategory(MESH_RENDER_CATEGORY_TRANSMISSIVE);
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::LoadTemplate(const std::string& strFile)
    {
        Unload();

        SetMaterialTemplate(strFile);

        // Material scripts are assumed to be located relative to Textures/
        std::string strScript = "../Textures/" + strFile;
        CScript* pScript = gScriptMgr->AddGetPtr(strScript);
        if (!pScript)
            return false;

        CScript& script = *pScript;

        LoadTemplate(script);

        gScriptMgr->Release(strScript);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CMaterial::Update(float dt)
    {
        if (!m_bHasAnimation)
            return;

        if (m_numFrames > 1 && m_frameTime > 0.0f)
        {
            m_currentTimeElapsed += dt;
            while (m_currentTimeElapsed >= m_frameTime)
            {
                m_currentTimeElapsed -= m_frameTime;
                m_currentFrame = (m_currentFrame + 1) % m_numFrames;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::GetTextureCoordinates(unsigned int sequence, unsigned int frame, float& x1, float& y1, float& x2, float& y2)
    {
        if (sequence >= m_numSequences || frame >= m_numFrames)
            return false;

        if (!m_bHasAnimation)
        {
            x1 = 0.0f;
            x2 = 1.0f;
            y1 = 0.0f;
            y2 = 1.0f;
        }
        else if (m_pTexture || 
                 m_pNormalMap || 
                 m_pSpecularMap || 
                 m_pEmissiveMap)
        {
            float width = 0.0f, height = 0.0f;

            if (m_pTexture)
            {
                width = static_cast<float>(m_pTexture->GetWidth());
                height = static_cast<float>(m_pTexture->GetHeight());
            }
            else if (m_pNormalMap)
            {
                width = static_cast<float>(m_pTexture->GetWidth());
                height = static_cast<float>(m_pTexture->GetHeight());
            }
            else if (m_pSpecularMap)
            {
                width = static_cast<float>(m_pTexture->GetWidth());
                height = static_cast<float>(m_pTexture->GetHeight());
            }
            else if (m_pEmissiveMap)
            {
                width = static_cast<float>(m_pTexture->GetWidth());
                height = static_cast<float>(m_pTexture->GetHeight());
            }

            x1 = (float)(m_posLeft + frame * m_frameWidth) / width;
            y1 = (float)(m_posTop + sequence * m_frameHeight) / height;
            x2 = (float)(m_posLeft + (frame + 1) * m_frameWidth) / width;
            y2 = (float)(m_posTop + (sequence + 1) * m_frameHeight) / height;
        }


        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::GetTextureCoordinates(float& x1, float& y1, float& x2, float& y2)
    {
        return GetTextureCoordinates(m_currentSequence, m_currentFrame, x1, y1, x2, y2);
    }

    //-----------------------------------------------------------------------------------
    void CMaterial::GetTextureCoordinates(unsigned int sequence, unsigned int frame, TextureRect& textureRect)
    {
        float width = 0.0f, height = 0.0f;
        if (m_pTexture)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        else if (m_pNormalMap)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        else if (m_pSpecularMap)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        else if (m_pEmissiveMap)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        textureRect.left = (float)(m_posLeft + frame * m_frameWidth) / width;
        textureRect.top = (float)(m_posTop + sequence * m_frameHeight) / height;
        textureRect.right = (float)(m_posLeft + (frame + 1) * m_frameWidth) / width;
        textureRect.bottom = (float)(m_posTop + (sequence + 1) * m_frameHeight) / height;
    }

    //-----------------------------------------------------------------------------------
    void CMaterial::GetTextureCoordinates(TextureRect& textureRect)
    {
        float width = 0.0f, height = 0.0f;
        if (m_pTexture)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        else if (m_pNormalMap)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        else if (m_pSpecularMap)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        else if (m_pEmissiveMap)
        {
            width = static_cast<float>(m_pTexture->GetWidth());
            height = static_cast<float>(m_pTexture->GetHeight());
        }
        textureRect.left = (float)(m_posLeft + m_currentFrame * m_frameWidth) / width;
        textureRect.top = (float)(m_posTop + m_currentSequence * m_frameHeight) / height;
        textureRect.right = (float)(m_posLeft + (m_currentFrame + 1) * m_frameWidth) / width;
        textureRect.bottom = (float)(m_posTop + (m_currentSequence + 1) * m_frameHeight) / height;
    }

    //-----------------------------------------------------------------------------------
    unsigned int CMaterial::GetWidth()
    {
        if (m_pTexture)
            return m_pTexture->GetWidth();
        else if (m_pNormalMap)
            return m_pNormalMap->GetWidth();
        else if (m_pSpecularMap)
            return m_pSpecularMap->GetWidth();
        else if (m_pEmissiveMap)
            return m_pEmissiveMap->GetWidth();
        else
            return 0;
    }

    //-----------------------------------------------------------------------------------
    unsigned int CMaterial::GetHeight()
    {
        if (m_pTexture)
            return m_pTexture->GetHeight();
        else if (m_pNormalMap)
            return m_pNormalMap->GetHeight();
        else if (m_pSpecularMap)
            return m_pSpecularMap->GetHeight();
        else if (m_pEmissiveMap)
            return m_pEmissiveMap->GetHeight();
        else
            return 0;
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::IsNormalMap(const std::string& strFile)
    {
        return (strFile.find("_nmap.") != std::string::npos);
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::IsSpecularMap(const std::string& strFile)
    {
        return (strFile.find("_specmap.") != std::string::npos);
    }

    //-----------------------------------------------------------------------------------
    bool CMaterial::IsEmissiveMap(const std::string& strFile)
    {
        return (strFile.find("_emismap.") != std::string::npos);
    }


    //-----------------------------------------------------------------------------------
    CManagedMaterial::CManagedMaterial()
    {

    }

    //-----------------------------------------------------------------------------------
    CManagedMaterial::~CManagedMaterial()
    {
        Unload();
    }

    //-----------------------------------------------------------------------------------
    bool CManagedMaterial::LoadResource()
    {
        Unload();

        std::string strExtension;
        GetFileExtension(m_strFilename, strExtension);

        if (strExtension == ".mtrl")
        {
            // Load material
            return LoadTemplate(m_strName);
        }
        else
        {
            std::string strFile = m_strFilename;
            if (IsNormalMap(strFile))
            {
                // Load normal map
                return Load(m_pNormalMap, m_strName, "LinearWrap", false);
            }
            else if (IsSpecularMap(strFile))
            {
                // Load specular map
                return Load(m_pSpecularMap, m_strName, "LinearWrap", true);
            }
            else if (IsEmissiveMap(strFile))
            {
                // Load emissive map
                return Load(m_pEmissiveMap, m_strName, "LinearWrap", true);
            }
            else
            {
                // Load texture
                return Load(m_pTexture, m_strName, "LinearWrap", true);
             }
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CManagedMaterial::InitResource(const std::string& strFilename, const std::string& str)
    {
        m_strFilename = strFilename;
        m_strName = str;
        return LoadResource();
    }

    //-----------------------------------------------------------------------------------
    bool CManagedMaterial::InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt)
    {
        m_strFilename = strName;
        m_strName = str;

        if (!LoadResource())
            return false;

        GetFileLastWriteTime(m_strFilename, pFt);

        return true;
    }

}