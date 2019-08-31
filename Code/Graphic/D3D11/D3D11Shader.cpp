
#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11Shader.h>
#include <Graphic/D3D11/D3D11RenderDevice.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Common/CommonIncludes.h>
#include <locale>
#include <codecvt>

namespace env
{
    //-----------------------------------------------------------------------------------
    CConstantBufferGPU* CConstantBufferGPU::ConstantBufferAllocationPolicy::Create()
    {
        return new CConstantBufferGPU(m_pDevice->GetDevice());
    }

    //-----------------------------------------------------------------------------------
    inline std::string GetVertexShaderName(const std::string& strFilename) { return strFilename + ".hlsl"; }
    inline std::string GetVertexShaderCompiledName(const std::string& strFilename) { return strFilename + ".cvsh"; }
    inline std::string GetPixelShaderName(const std::string& strFilename) { return strFilename + ".hlsl"; }
    inline std::string GetPixelShaderCompiledName(const std::string& strFilename) { return strFilename + ".cpsh"; }
    inline std::string GetGeometryShaderName(const std::string& strFilename) { return strFilename + ".hlsl"; }
    inline std::string GetGeometryShaderCompiledName(const std::string& strFilename) { return strFilename + ".cgsh"; }
    inline std::string GetComputeShaderName(const std::string& strFilename) { return strFilename + ".hlsl"; }
    inline std::string GetComputeShaderCompiledName(const std::string& strFilename) { return strFilename + ".ccsh"; }

    //-----------------------------------------------------------------------------------
    bool CompileShaderFromFile(const std::string& strInputFilename,
                               const std::string& strOutputFilename,
                               const std::string& strEntryPoint,
                               const std::string& strTarget,
                               ID3DBlob** ppCode)
    {
        ID3DBlob* pErrors = nullptr;
        *ppCode = nullptr;

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        const std::wstring strInputFilenameUnicode = converter.from_bytes(strInputFilename);

        unsigned long flags = 0;

#if defined(ENV_FINAL) | defined(ENV_PROFILE)
        flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
        flags = D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        if (FAILED(D3DCompileFromFile(strInputFilenameUnicode.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, strEntryPoint.c_str(), strTarget.c_str(), flags, 0, ppCode, &pErrors)))
        {
            ENV_LOG_INFO("Compiling shader failed [" + strEntryPoint + "] [" + strTarget + "]: " + strInputFilename);
            ENV_LOG_ERROR_SIMPLE(static_cast<const char*>(pErrors->GetBufferPointer()));
            ENV_RELEASE(pErrors);

            return false;
        }
        else
        {
            ENV_LOG_INFO("Compiled shader [" + strEntryPoint + "] [" + strTarget + "]: " + strInputFilename);
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool ReadShaderCodeFromFile(const std::string& strFilename, ID3DBlob** ppShaderCode)
    {
        std::fstream shaderFileStream(strFilename.c_str(), std::ios::in | std::ios::binary);

        if (!shaderFileStream.is_open())
            return false;

        shaderFileStream.seekg(0, shaderFileStream.end);

        ENV_DXCALL_RFALSE(D3DCreateBlob(shaderFileStream.tellg(), ppShaderCode));

        shaderFileStream.seekg(0, shaderFileStream.beg);

        shaderFileStream.read(static_cast<char*>((*ppShaderCode)->GetBufferPointer()), (*ppShaderCode)->GetBufferSize());

        if (!shaderFileStream.good())
        {
            ENV_RELEASE((*ppShaderCode));
            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool WriteShaderCodeToFile(const std::string& strFilename, ID3DBlob* pShaderCode)
    {
        std::fstream shaderFileStream(strFilename.c_str(), std::ios::out | std::ios::binary);

        if (!shaderFileStream.is_open())
            return false;

        shaderFileStream.write(static_cast<char*>(pShaderCode->GetBufferPointer()), pShaderCode->GetBufferSize());

        return shaderFileStream.good();
    }

    //-----------------------------------------------------------------------------------
    bool CShader::ShaderReloadPolicy::Reload(const std::string& key, FILETIME& fileTime)
    {
        std::vector< std::string > shaderFilenames;
        shaderFilenames.reserve(6);
        shaderFilenames.push_back(GetVertexShaderName(key));
        shaderFilenames.push_back(GetVertexShaderCompiledName(key));
        shaderFilenames.push_back(GetPixelShaderName(key));
        shaderFilenames.push_back(GetPixelShaderCompiledName(key));
        shaderFilenames.push_back(GetComputeShaderName(key));
        shaderFilenames.push_back(GetComputeShaderCompiledName(key));

        for (size_t i = 0; i < shaderFilenames.size(); ++i)
        {
            FILETIME currentFileTime;
            if (!GetFileLastWriteTime(shaderFilenames[i], &currentFileTime))
                continue;

            if (CompareFileTime(&currentFileTime, &fileTime) == 1)
            {
                fileTime = currentFileTime;
                return true;
            }
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    CShader* CShader::ShaderAllocationPolicy::Create()
    {
        return new CShader(m_pDevice->GetDevice(), m_pDevice->GetDeviceContext());
    }

    //-----------------------------------------------------------------------------------
    CShader::~CShader()
    {
        ReleaseConstantBuffers();

        for (unsigned int i = 0; i < m_shaderReflections.size(); ++i)
            ENV_RELEASE(m_shaderReflections[i]);

        ENV_RELEASE(m_pShaderCode);
        ENV_RELEASE(m_pComputeShader);
        ENV_RELEASE(m_pPixelShader);
        ENV_RELEASE(m_pVertexShader);
    }

    //-----------------------------------------------------------------------------------
    bool CShader::InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt)
    {
        m_strFilename = strFilename;
        m_strName = strName;

        if (!LoadResource())
            return false;

        GetFileLastWriteTime(strFilename + ".hlsl", pFt);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CShader::LoadShader(const std::string& strShaderFilename, const std::string& strShaderCompiledFilename,
                             const std::string& strEntryPoint, const std::string& strTarget, ID3DBlob** ppCode)
    {
        *ppCode = nullptr;

        bool existsShader = GetFileExists(strShaderFilename);
        bool existsCompiledShader = GetFileExists(strShaderCompiledFilename);

        // Try to load shader
        if (existsShader || existsCompiledShader)
        {
            if ((existsCompiledShader && !m_InitialLoad) || !existsShader)
            {
                // Load compiled vertex shader
                if (!ReadShaderCodeFromFile(strShaderCompiledFilename, ppCode))
                    return false;
            }
            else
            {
                // Compile vertex shader
                if (!CompileShaderFromFile(strShaderFilename, strShaderCompiledFilename, strEntryPoint, strTarget, ppCode))
                    return false;

                //if (!WriteShaderCodeToFile(strShaderCompiledFilename, *ppCode))
                //    ENV_RELEASE((*ppCode));

                //if (!GetFileExists(strShaderCompiledFilename))
                //    ENV_RELEASE((*ppCode));
            }
        }

        return (*ppCode != nullptr);
    }

    //-----------------------------------------------------------------------------------
    bool CShader::LoadResource()
    {
        ReleaseConstantBuffers();

        std::string strVertexShader = GetVertexShaderName(m_strFilename);
        std::string strVertexShaderCompiled = GetVertexShaderCompiledName(m_strFilename);
        std::string strPixelShader = GetPixelShaderName(m_strFilename);
        std::string strPixelShaderCompiled = GetPixelShaderCompiledName(m_strFilename);
        std::string strComputeShader = GetComputeShaderName(m_strFilename);
        std::string strComputeShaderCompiled = GetComputeShaderCompiledName(m_strFilename);

        m_shaderReflections.resize(SHADER_TYPE_COUNT);
        m_constantBuffers.resize(SHADER_TYPE_COUNT);

        for (unsigned int i = 0; i < m_shaderReflections.size(); ++i)
        {
            m_shaderReflections[i] = nullptr;
            m_constantBuffers.clear();
        }

        // Load and create vertex shader
        if (LoadShader(strVertexShader, strVertexShaderCompiled, "VS", "vs_4_1", &m_pShaderCode))
        {
            ENV_DXCALL_RFALSE(m_pDevice->CreateVertexShader(m_pShaderCode->GetBufferPointer(), m_pShaderCode->GetBufferSize(), 0, &m_pVertexShader));
            ENV_DXCALL_RFALSE(D3DReflect(m_pShaderCode->GetBufferPointer(), m_pShaderCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_shaderReflections[SHADER_TYPE_VERTEX]));
        }

        ID3DBlob* pCode = nullptr;
        // Load and create pixel shader
        if (LoadShader(strPixelShader, strPixelShaderCompiled, "PS", "ps_4_1", &pCode))
        {
            ENV_DXCALL_RFALSE(m_pDevice->CreatePixelShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), 0, &m_pPixelShader));
            ENV_DXCALL_RFALSE(D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_shaderReflections[SHADER_TYPE_PIXEL]));
            ENV_RELEASE(pCode);
        }

        // Load and create compute shader
        if (LoadShader(strComputeShader, strComputeShaderCompiled, "CS", "cs_5_0", &pCode))
        {
            ENV_DXCALL_RFALSE(m_pDevice->CreateComputeShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), 0, &m_pComputeShader));
            ENV_DXCALL_RFALSE(D3DReflect(pCode->GetBufferPointer(), pCode->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_shaderReflections[SHADER_TYPE_COMPUTE]));
            ENV_RELEASE(pCode);
        }

        m_InitialLoad = false;

        if (!m_pVertexShader &&
            !m_pPixelShader &&
            !m_pComputeShader)
        {
            ENV_LOG_ERROR("COULD NOT LOAD SHADERS: " + m_strFilename);
        }

        if (!InitConstantBuffers())
            return false;

        return m_pVertexShader || m_pPixelShader || m_pComputeShader;
    }

    //-----------------------------------------------------------------------------------
    void CShader::ReleaseConstantBuffers()
    {
        for (auto it : m_constantBuffers)
        {
            for (unsigned int i = 0; i < it.m_refCount; ++i)
                gConstBufMgr->Release(it.m_bufferDesc.Name);
        }

        m_constantBuffers.clear();
        m_constantBuffersGPU.clear();
        m_constantBufferIndex.clear();
    }

    //-----------------------------------------------------------------------------------
    bool CShader::InitConstantBuffers()
    {
        m_constantBuffersGPU.resize(m_shaderReflections.size());
        m_constantBufferIndex.clear();
        m_constantBuffers.clear();

        for (unsigned int i = 0; i < m_shaderReflections.size(); ++i)
        {
            if (!m_shaderReflections[i])
                continue;

            vecConstantBuffersGPU& constantBuffersGPU = m_constantBuffersGPU[i];

            D3D11_SHADER_DESC shaderDesc;
            m_shaderReflections[i]->GetDesc(&shaderDesc);

            constantBuffersGPU.reserve(shaderDesc.ConstantBuffers);

            for (unsigned int j = 0; j < shaderDesc.ConstantBuffers; ++j)
            {
                ID3D11ShaderReflectionConstantBuffer* pConstBuf = m_shaderReflections[i]->GetConstantBufferByIndex(j);

                ConstantBufferGPU constantBuffer;

                pConstBuf->GetDesc(&constantBuffer.m_bufferDesc);

                auto it = m_constantBufferIndex.find(constantBuffer.m_bufferDesc.Name);

                if (it != m_constantBufferIndex.end())
                {
                    constantBuffersGPU.push_back(m_constantBuffers[it->second].m_pBuffer);

                    m_constantBuffers[it->second].m_refCount++;
                }
                else
                {
                    D3D11_BUFFER_DESC constBufferDesc;
                    constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
                    constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                    constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    constBufferDesc.MiscFlags = 0;
                    constBufferDesc.StructureByteStride = 0;
                    constBufferDesc.ByteWidth = constantBuffer.m_bufferDesc.Size;//std::max(dataSize / 16, (size_t)1) * 16;

                    CConstantBufferGPU* pBufferGPU = gConstBufMgr->AddGetPtr(constantBuffer.m_bufferDesc.Name, constBufferDesc);
                    constantBuffer.m_pBuffer = pBufferGPU->GetGPUBuffer();
                    constantBuffer.m_refCount++;

                    ENV_ASSERT_DBG(constBufferDesc.ByteWidth == pBufferGPU->GetBufferDesc().ByteWidth);
                    //ENV_DXCALL_RFALSE(m_pDevice->CreateBuffer(&constBufferDesc, 0, &constantBuffer.m_pBuffer));

                    m_constantBuffers.push_back(constantBuffer);

                    m_constantBufferIndex[constantBuffer.m_bufferDesc.Name] = m_constantBuffers.size() - 1;

                    constantBuffersGPU.push_back(constantBuffer.m_pBuffer);
                }
            }
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CShader::Begin()
    {
        if (m_pVertexShader)
        {
            m_pDeviceContext->VSSetShader(m_pVertexShader, 0, 0);
        }

        if (m_pPixelShader)
        {
            m_pDeviceContext->PSSetShader(m_pPixelShader, 0, 0);
        }

        if (m_pComputeShader)
        {
            m_pDeviceContext->CSSetShader(m_pComputeShader, 0, 0);
        }
    }

    //-----------------------------------------------------------------------------------
    void CShader::End()
    {
        if (m_pVertexShader)
            m_pDeviceContext->VSSetShader(nullptr, 0, 0);

        if (m_pPixelShader)
            m_pDeviceContext->PSSetShader(nullptr, 0, 0);

        if (m_pComputeShader)
            m_pDeviceContext->CSSetShader(nullptr, 0, 0);
    }

} // env
#endif // ENV_GRAPHIC_API_D3D11
