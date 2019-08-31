/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_SHADER_H
#define D3D9_SHADER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>
#include <Graphic/D3D9/D3D9APITypeConversion.h>
//#include <Engine/RenderSystem/D3D9/D3D9RenderDevice.h>

namespace env
{

    class IShaderUniform
    {
    public:
        IShaderUniform()
            : m_bDirty(true) {}

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle) = 0;
        virtual bool HasChanged() { return m_bDirty; }

    protected:
        bool m_bDirty;
    };

    template< class Type >
    class CShaderUniform : public IShaderUniform
    {
    public:
        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            return SUCCEEDED(pEffect->SetValue(handle, (const void*)&m_data, sizeof(Type)));
        }

        virtual void SetData(const Type& data)
        {
            m_data = data;

            m_bDirty = true;
        }
    protected:
        Type m_data;
    };


    class CIntUniform : public CShaderUniform < int >
    {
    public:
        CIntUniform()
        {
            m_data = 0;
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            return SUCCEEDED(pEffect->SetInt(handle, m_data));
        }
    };

    class CIntArrayUniform : public CShaderUniform < std::vector< int > >
    {
    public:
        CIntArrayUniform()
        {
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            if (!m_data.empty())
            {
                return SUCCEEDED(pEffect->SetIntArray(handle, &m_data[0], m_data.size()));
            }
            else
                return true;
        }
    };


    class CFloatUniform : public CShaderUniform < float >
    {
    public:
        CFloatUniform()
        {
            m_data = 0.0f;
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            return SUCCEEDED(pEffect->SetFloat(handle, m_data));
        }
    };

    class CFloatArrayUniform : public CShaderUniform < std::vector< float > >
    {
    public:
        CFloatArrayUniform()
        {
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            if (!m_data.empty())
                return SUCCEEDED(pEffect->SetFloatArray(handle, &m_data[0], m_data.size()));
            else
                return true;
        }
    };

    class CVec2Uniform : public CShaderUniform < Vec2 >
    {
    public:
        CVec2Uniform()
        {
            m_data = Vec2(0.0f, 0.0f);
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            return SUCCEEDED(pEffect->SetVector(handle, (D3DXVECTOR4*)&m_data));
        }
    };

    class CVec3Uniform : public CShaderUniform < Vec3 >
    {
    public:
        CVec3Uniform()
        {
            m_data = Vec3(0.0f, 0.0f, 0.0f);
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            return SUCCEEDED(pEffect->SetVector(handle, (D3DXVECTOR4*)&m_data));
        }
    };

    class CVec4Uniform : public CShaderUniform < Vec4 >
    {
    public:
        CVec4Uniform()
        {
            m_data = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            return SUCCEEDED(pEffect->SetVector(handle, (D3DXVECTOR4*)&m_data));
        }
    };

    class CMatrixUniform : public CShaderUniform < Matrix4x4 >
    {
    public:
        CMatrixUniform()
        {
            m_data = Matrix4x4(1.0f);
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;
            D3DXMATRIX mtx = convertToD3D9Matrix(m_data);
            return SUCCEEDED(pEffect->SetMatrix(handle, (D3DXMATRIX*)&mtx));
        }
    };

    class CMatrix3x3Uniform : public CShaderUniform < Matrix3x3 >
    {
    public:
        CMatrix3x3Uniform()
        {
            m_data = Matrix3x3(1.0f);
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;
            //D3DXMATRIX mtx = convertToD3D9Matrix(m_data);
            float* pData = &m_data[0][0];
            return SUCCEEDED(pEffect->SetFloatArray(handle, pData, 36));
        }
    };

    class CTextureUniform : public CShaderUniform < IDirect3DTexture9* >
    {
    public:
        CTextureUniform()
        {
            m_data = NULL;
        }

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            if (m_data)
                return SUCCEEDED(pEffect->SetTexture(handle, m_data));
            else
                return true;
        }
    };

    template< class Type >
    class CShaderUniformArray : public CShaderUniform < std::vector< Type > >
    {
    public:
        //typedef std::vector< Type > vecType;

        virtual bool SetUniformValue(ID3DXEffect* pEffect, D3DXHANDLE handle)
        {
            m_bDirty = false;

            if (!m_data.empty())
            {
                unsigned int s = sizeof(Type);
                return SUCCEEDED(pEffect->SetValue(handle, (const void*)&m_data[0], m_data.size() * sizeof(Type)));
            }
            else
                return true;
        }
    };


    typedef CShaderUniformArray< Vec4 > CVec4ArrayUniform;
    typedef CShaderUniformArray< Vec3 > CVec3ArrayUniform;
    typedef CShaderUniformArray< Vec2 > CVec2ArrayUniform;


    struct ShaderMacro
    {
        ShaderMacro(const std::string& strName, const std::string& strDefinition)
            : strName(strName), strDefinition(strDefinition) {}

        std::string strName;
        std::string strDefinition;
        //bool bEndMarker;
    };

    class CD3D9RenderDevice;

    class CD3D9Shader
    {
    public:
        struct ShaderAllocationPolicy
        {
            ShaderAllocationPolicy() : m_pDevice(0) {}
            ShaderAllocationPolicy(CD3D9RenderDevice* pDevice);

            CD3D9Shader* Create();

            CD3D9RenderDevice* m_pDevice;
        };


        typedef CResMgrBase< std::string, CD3D9Shader, ShaderAllocationPolicy > CShaderMgr;

        CD3D9Shader(IDirect3DDevice9* pDevice);
        ~CD3D9Shader();

        bool LoadFromFile(IDirect3DDevice9* pDevice, const std::string& strFilename);
        bool LoadResource();

        void Begin();
        void BeginPass(unsigned int pass);
        void EndPass();
        void End();

        void SetDefines(D3DXMACRO* defines, const std::vector< ShaderMacro >& defs);

        void UpdateUniforms();

        void OnLostDevice();
        void OnResetDevice();

        void SetUniform(const std::string& strParam, IShaderUniform* pUniform);

        const std::string& GetName() { return m_strName; }

    private:
        friend class CShaderMgr;

        bool InitResource(const std::string& strName, const std::string& str, _FILETIME* pFt);

    private:
        bool m_bActive;
        bool m_bDirty;

        IDirect3DDevice9* m_pDevice;
        ID3DXEffect* m_pEffect;

        std::string m_strName;
        std::string m_strFilename;
        std::string m_strCurrentTechnique;

        typedef std::pair< D3DXHANDLE, IShaderUniform* > pairShaderHandleUniform;
        typedef std::map< std::string, pairShaderHandleUniform > mapUniforms;
        mapUniforms m_uniforms;

        D3DXMACRO* m_defines;

        std::vector< ShaderMacro > m_defs;
    };

    typedef CD3D9Shader CShader;

    //typedef CShader::CShaderMgr CShaderMgr;

    //extern CShaderMgr* gShaderMgr;

} // env

#endif // SHADER_H
#endif // D3D9