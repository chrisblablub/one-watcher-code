#pragma once
#ifndef DATA_CACHE_H
#define DATA_CACHE_H

#include <Engine/EngineCommon.h>
#include <Graphic/Core/VertexDeclarations.h>

namespace env
{

    template< class T > 
    class CDataCache
    {
    public:
        typedef T DataType;

        CDataCache()
            : m_pDecl(nullptr)
            , m_primType(PRIMITIVE_TYPE_POINTLIST) {}

        virtual ~CDataCache() {}

        virtual CVertexDeclaration* GetDeclaration() const { return m_pDecl; }
        virtual const void* GetData() const { return &m_data[0]; }
        virtual unsigned int GetStride() const { return sizeof(DataType); }
        virtual unsigned int GetNumElements() const { return m_data.size(); }
        virtual PRIMITIVE_TYPE GetPrimitiveType() const { return m_primType; }


        void Add(const T& v1)
        {
            m_data.push_back(v1);
        }
        void Reset()
        {
            m_data.clear();
        }
        void Reserve(int numElements)
        {
            m_data.reserve(numElements);
        }

        void SetPrimitiveType(PRIMITIVE_TYPE primType) { m_primType = primType; }
        void SetFVF(unsigned long fvf) { m_fvf = fvf; }
        void SetDeclaration(CVertexDeclaration* pDecl) { m_pDecl = pDecl; }
    
    protected:
        std::vector< T > m_data;
        CVertexDeclaration* m_pDecl;
        PRIMITIVE_TYPE m_primType;
    };


    class CVector3Cache : public CDataCache< PolygonVertex > 
    {
    public:
        virtual ~CVector3Cache() {}

        CVector3Cache()
        {
            SetPrimitiveType(PRIMITIVE_TYPE_TRIANGLELIST);
            //SetDeclaration(VertexPolygon::pDecl);
        }

        void Transform(const MATRIX& mtxTransform)
        {
            m_transformedData.resize(m_data.size());
            for(int i = 0; i < m_data.size(); ++i)
            {
                 m_transformedData[i].pos = VECTOR3(VECTOR4(m_data[i].pos, 1.0f) * mtxTransform);
                 m_transformedData[i].color = m_data[i].color;
            }
        }

        virtual const void* GetData() const { return m_transformedData.empty() ?  &m_data[0] : &m_transformedData[0]; }

    protected:
        std::vector< PolygonVertex > m_transformedData;
    };

} // env

#endif // DATA_CACHE_H