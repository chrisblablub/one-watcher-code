/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_SHADER_H
#define D3D11_SHADER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/D3D11/D3D11APITypeConversion.h>
#include <D3Dcompiler.h>

namespace env
{

#define UNIFORM_VERTEX_SHADER   1 << 1
#define UNIFORM_PIXEL_SHADER    1 << 2
#define UNIFORM_GEOMETRY_SHADER 1 << 3
#define UNIFORM_COMPUTE_SHADER  1 << 4

    struct ShaderMacro
    {
        ShaderMacro(const std::string& strName, 
                    const std::string& strDefinition)
            : strName(strName)
            , strDefinition(strDefinition) {}

        std::string strName;
        std::string strDefinition;
    };

    class CRenderDevice;

    class CConstantBufferCPUBase
    {
    public:
        CConstantBufferCPUBase(void* pData, size_t size)
            : m_pData(pData)
            , m_dataSize(size)
            , m_bIsDirty(true) {}

        CConstantBufferCPUBase(const CConstantBufferCPUBase&) = delete;
        const CConstantBufferCPUBase& operator=(const CConstantBufferCPUBase&) = delete;

        inline const void* GetDataPtr(size_t& size)
        {
            size = m_dataSize;

            return m_pData;
        }

        inline void SetDirty(bool bDirty) { m_bIsDirty = bDirty; }
        inline bool GetDirty() const { return m_bIsDirty; }

    private:
        void* m_pData;
        size_t m_dataSize;
        bool m_bIsDirty;

    };
    template<typename ConstantBufferData>
    class CConstantBufferCPU : public CConstantBufferCPUBase
    {
    public:
        typedef ConstantBufferData ConstBufferDataType;

        CConstantBufferCPU()
            : CConstantBufferCPUBase(&m_data, sizeof(m_data)) {}

        inline const ConstBufferDataType& GetData() const { return m_data; }
        inline ConstBufferDataType& GetDataForWriting() { SetDirty(true); return m_data; }

    private:
        ConstBufferDataType m_data;
    };

    class CConstantBufferGPU
    {
    public:
        struct ConstantBufferAllocationPolicy
        {
            ConstantBufferAllocationPolicy()
                : m_pDevice(nullptr) {}

            ConstantBufferAllocationPolicy(CRenderDevice* pDevice)
                : m_pDevice(pDevice) {}

            CConstantBufferGPU* Create();

            CRenderDevice* m_pDevice;
        };
        
        typedef CResMgrBase< std::string, CConstantBufferGPU, ConstantBufferAllocationPolicy > CConstantBufferGPUMgr;

        friend class CConstantBufferGPUMgr;

        CConstantBufferGPU(ID3D11Device* pDevice)
            : m_pDevice(pDevice) {}

        ~CConstantBufferGPU()
        {
            ENV_RELEASE(m_pConstBufGPU);
        }

        inline const std::string& GetName() const { return m_strName; }
        inline ID3D11Buffer* GetGPUBuffer() const { return m_pConstBufGPU; }
        inline const D3D11_BUFFER_DESC& GetBufferDesc() const { return m_bufferDesc; }

    private:

        bool InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt, const D3D11_BUFFER_DESC& bufferDesc)
        {
            m_strName = str;

            m_bufferDesc = bufferDesc;

            ENV_DXCALL_RFALSE(m_pDevice->CreateBuffer(&bufferDesc, 0, &m_pConstBufGPU));

            return true;
        }

        ID3D11Device* m_pDevice;
        ID3D11Buffer* m_pConstBufGPU;
        D3D11_BUFFER_DESC m_bufferDesc;

        std::string m_strName;

    };

    typedef CConstantBufferGPU::CConstantBufferGPUMgr CConstantBufferGPUMgr;

    extern CConstantBufferGPUMgr* gConstBufMgr;


    class CShader
    {
    public:
        enum SHADER_TYPE
        {
            SHADER_TYPE_VERTEX = 0,
            SHADER_TYPE_PIXEL,
            SHADER_TYPE_COMPUTE,
            SHADER_TYPE_GEOMETRY,
            SHADER_TYPE_DOMAIN,
            SHADER_TYPE_HULL,
            SHADER_TYPE_COUNT
        };

        struct ConstantBufferGPU
        {
            ConstantBufferGPU()
                : m_pBuffer(nullptr)
                , m_pConstBufferCPU(nullptr)
                , m_refCount(0) {}

            ID3D11Buffer* m_pBuffer;

            D3D11_SHADER_BUFFER_DESC m_bufferDesc;

            CConstantBufferCPUBase* m_pConstBufferCPU;

            unsigned int m_refCount;
        };

        typedef std::vector< ID3D11Buffer* > vecConstantBuffersGPU;

        struct ShaderReloadPolicy
        {
            bool Reload(const std::string& key, FILETIME& fileTime);
        };

        struct ShaderAllocationPolicy
        {
            ShaderAllocationPolicy()
                : m_pDevice(nullptr) {}

            ShaderAllocationPolicy(CRenderDevice* pDevice)
                : m_pDevice(pDevice) {}

            CShader* Create();

            CRenderDevice* m_pDevice;
        };


        typedef CResMgrBase< std::string,
            CShader,
            ShaderAllocationPolicy,
            ShaderReloadPolicy > CShaderMgr;

        CShader(ID3D11Device* pDevice,
            ID3D11DeviceContext* pDeviceContext)
            : m_pDevice(pDevice)
            , m_pDeviceContext(pDeviceContext)
            , m_InitialLoad(false)
            , m_pVertexShader(nullptr)
            , m_pPixelShader(nullptr)
            , m_pComputeShader(nullptr) {}

        ~CShader();

        bool LoadResource();
        void Begin();
        void BeginPass(unsigned int pass) {}
        void EndPass() {}
        void End();
        void OnLostDevice() {}
        void OnResetDevice() {}

        unsigned int GetConstantBufferIndexByName(const std::string& strConstantBufferName)
        {
            auto& it = m_constantBufferIndex.find(strConstantBufferName);

            if (it != m_constantBufferIndex.end())
                return it->second;

            ENV_ASSERT_DBG(false);
            ENV_LOG_WARNING("Constant buffer does not exist.");

            return 0;
        }

        void LinkConstantBufferCPU(const std::string& strConstBufName, CConstantBufferCPUBase& constBufferCPU)
        {
            unsigned int index = GetConstantBufferIndexByName(strConstBufName);

            m_constantBuffers[index].m_pConstBufferCPU = &constBufferCPU;
            m_constantBuffersLinked[strConstBufName] = &constBufferCPU;
        }

        void CompileConstantBuffers()
        {
            for (unsigned int slot = 0; slot < m_constantBuffers.size(); ++slot)
            {
                ConstantBufferGPU& constBufGPU = m_constantBuffers[slot];

                //ENV_ASSERT_DBG(constBufGPU.m_pConstBufferCPU && "Link constant buffer with shader");

                if (constBufGPU.m_pConstBufferCPU == nullptr ||
                    !constBufGPU.m_pConstBufferCPU->GetDirty())
                    continue;

                size_t dataSize = 0;
                const void* pConstBufferData = constBufGPU.m_pConstBufferCPU->GetDataPtr(dataSize);

                D3D11_MAPPED_SUBRESOURCE mappedResource;
                m_pDeviceContext->Map(constBufGPU.m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

                ENV_ASSERT_DBG(constBufGPU.m_bufferDesc.Size == dataSize);

                memcpy(mappedResource.pData, pConstBufferData, constBufGPU.m_bufferDesc.Size);

                m_pDeviceContext->Unmap(constBufGPU.m_pBuffer, 0);

                constBufGPU.m_pConstBufferCPU->SetDirty(false);
            }
        }
        std::vector<vecConstantBuffersGPU>& GetConstantBuffers()
        {
            return m_constantBuffersGPU;
        }

        ID3DBlob* GetCode() const { return m_pShaderCode; }

        const std::string& GetName() { return m_strName; }

    private:

        friend class CShaderMgr;

        bool InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt);
        bool LoadShader(const std::string& strShaderFilename, const std::string& strShaderCompiledFilename,
            const std::string& strEntryPoint, const std::string& strTarget, ID3DBlob** ppCode);

        void ReleaseConstantBuffers();
        bool InitConstantBuffers();

    private:

        bool m_bActive;
        bool m_bDirty;
        bool m_InitialLoad;

        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pDeviceContext;
        ID3D11VertexShader* m_pVertexShader;
        // ID3D11ShaderReflection* m_pVertexShaderReflection;
        ID3D11PixelShader* m_pPixelShader;
        //ID3D11ShaderReflection* m_pPixelShaderReflection;
        ID3D11HullShader* m_pHullShader;
        //ID3D11ShaderReflection* m_pHullShaderReflection;
        ID3D11DomainShader* m_pDomainShader;
        // ID3D11ShaderReflection* m_pDomainShaderReflection;
        ID3D11GeometryShader* m_pGeometryShader;
        //ID3D11ShaderReflection* m_pGeometryShaderReflection;
        ID3D11ComputeShader* m_pComputeShader;
        //ID3D11ShaderReflection* m_pComputeShaderReflection;
        ID3DBlob* m_pShaderCode;

        std::string m_strName;
        std::string m_strFilename;

        std::vector<vecConstantBuffersGPU> m_constantBuffersGPU;
        std::vector<ConstantBufferGPU> m_constantBuffers;
        std::map<std::string, CConstantBufferCPUBase*> m_constantBuffersLinked;
        std::map<std::string, unsigned int> m_constantBufferIndex;
        

        std::vector< ID3D11ShaderReflection* >  m_shaderReflections;

        std::vector< ShaderMacro > m_defs;
    };

} // env

#endif // D3D11_SHADER_H
#endif // ENV_GRAPHIC_API_D3D11
