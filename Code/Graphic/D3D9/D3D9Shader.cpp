#ifdef D3D9

#include <Graphic/D3D9/D3D9Shader.h>
#include <Graphic/RenderSystems/RenderSystem.h>


namespace env
{
    //CShaderMgr* gShaderMgr = NULL;

    CD3D9Shader::ShaderAllocationPolicy::ShaderAllocationPolicy(CD3D9RenderDevice* pDevice)
        : m_pDevice(pDevice) {}

    CD3D9Shader* CD3D9Shader::ShaderAllocationPolicy::Create()
    {
        return m_pDevice->CreateShader();
    }


    CD3D9Shader::CD3D9Shader(IDirect3DDevice9* pDevice)
        : m_pDevice(pDevice),
        m_pEffect(NULL),
        m_defines(NULL),
        m_bActive(false),
        m_bDirty(true)
    {

    }

    CD3D9Shader::~CD3D9Shader()
    {
        ENV_RELEASE(m_pEffect);
    }

    bool CD3D9Shader::InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt)
    {
        m_strFilename = strName + ".fx";
        m_strName = str;

        if (!LoadResource())
            return false;

        GetFileLastWriteTime(m_strFilename, pFt);

        return true;
    }

    bool CD3D9Shader::LoadResource()
    {
        ENV_RELEASE(m_pEffect);

        ID3DXBuffer* pBfr = NULL;
        if (FAILED(D3DXCreateEffectFromFile(m_pDevice, m_strFilename.c_str(),
            0, NULL, D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_PREFER_FLOW_CONTROL, NULL, &m_pEffect, &pBfr)))
        {
            ENV_LOG_ERROR(std::string("FAILED TO LOAD SHADER!"));
            if (pBfr)
            {
                ENV_LOG_ERROR((char*)pBfr->GetBufferPointer());
                ENV_RELEASE(pBfr);
            }
        }

        if (pBfr)
        {
            ENV_LOG_ERROR((char*)pBfr->GetBufferPointer());
            ENV_RELEASE(pBfr);
        }

        if (m_pEffect)
        {
            for (mapUniforms::iterator it = m_uniforms.begin(); it != m_uniforms.end(); ++it)
            {
                pairShaderHandleUniform& pairHandleUniform = it->second;
                pairHandleUniform.first = m_pEffect->GetParameterByName(NULL, it->first.c_str());
            }
        }

        return true;
    }

    bool CD3D9Shader::LoadFromFile(IDirect3DDevice9* pDevice, const std::string& strFilename)
    {
        m_strFilename = strFilename;
        return LoadResource();
    }

    void CD3D9Shader::UpdateUniforms()
    {
        if (!m_pEffect)
            return;

        m_bDirty = true;
        //if(!m_bActive)
        //	m_pEffect->BeginParameterBlock();

        bool bUniformsSet = false;

        for (mapUniforms::iterator it = m_uniforms.begin(); it != m_uniforms.end(); ++it)
        {
            pairShaderHandleUniform& pairHandleUniform = it->second;
            if (!m_bDirty && !pairHandleUniform.second->HasChanged())
                continue;

            if (!pairHandleUniform.second->SetUniformValue(m_pEffect, pairHandleUniform.first))
            {
                ENV_LOG_ERROR("COULD NOT SET UNIFORM '" + it->first + "' @ " + m_strName);
            }
            else
                bUniformsSet = true;
        }

        m_bDirty = false;

        /*if(!m_bActive)
        {
        D3DXHANDLE hParamBlock = m_pEffect->EndParameterBlock();
        if (bUniformsSet)
        m_pEffect->ApplyParameterBlock(hParamBlock);
        m_pEffect->DeleteParameterBlock(hParamBlock);
        }
        */

        if (m_bActive)
            m_pEffect->CommitChanges();

    }

    void CD3D9Shader::Begin()
    {
        if (!m_pEffect)
            return;

        UpdateUniforms();

        m_pEffect->Begin(NULL, 0);

        m_bActive = true;
    }



    void CD3D9Shader::BeginPass(unsigned int pass)
    {
        if (!m_pEffect)
            return;

        m_pEffect->BeginPass(pass);
    }

    void CD3D9Shader::EndPass()
    {
        if (!m_pEffect)
            return;

        m_pEffect->EndPass();
    }

    void CD3D9Shader::End()
    {
        if (!m_pEffect)
            return;

        m_pEffect->End();

        m_bActive = false;
    }

    void CD3D9Shader::OnLostDevice()
    {
        m_bDirty = true;

        if (!m_pEffect)
            return;

        m_pEffect->OnLostDevice();
    }

    void CD3D9Shader::OnResetDevice()
    {
        if (!m_pEffect)
            return;

        m_pEffect->OnResetDevice();
    }

    void CD3D9Shader::SetUniform(const std::string& strParam, IShaderUniform* pUniform)
    {
        pairShaderHandleUniform& pairHandleUniform = m_uniforms[strParam];
        pairHandleUniform.first = m_pEffect->GetParameterByName(NULL, strParam.c_str());
        pairHandleUniform.second = pUniform;
    }

// 	void CD3D9Shader::SetTechnique(const std::string& strTechnique)
// 	{
// 		m_bDirty = true;
// 		if (!m_pEffect)
// 			return;
// 
// 		m_strCurrentTechnique = strTechnique;
// 
// 		m_pEffect->SetTechnique(strTechnique.c_str());
// 	}

    void CD3D9Shader::SetDefines(D3DXMACRO* defines, const std::vector< ShaderMacro >& defs)
    {
        m_defs = defs;
        m_defines = defines;

        LoadResource();

        if (!m_strCurrentTechnique.empty())
            m_pEffect->SetTechnique(m_strCurrentTechnique.c_str());

    }

} // env

#endif // D3D9