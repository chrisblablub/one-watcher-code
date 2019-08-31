/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef BASEPARAM_H
#define BASEPARAM_H

#include <Common/Common.h>
#include <Common/Core/Macros.h>
#include <Common/Core/Serialize.h>

namespace env
{
    namespace paramStorage
    {
        // Why this struct? Desparation. Weird error where a VECTOR4 in a CParamStorage<VECTOR4>
        // is invalid after receiving a pointer with GetData(). Might be related to SIMD opts as it
        // works with VECTOR3.
        // As a workaround, use this Vec4 fake.
        struct Vec4
        {
            float x;
            float y;
            float z;
            float w;

            template< class Archive > void serialize(Archive& ar, const unsigned int version) 
            {
                ar & x;
                ar & y;
                ar & z;
                ar & w;
            }
        };
    }


    //-----------------------------------------------------------------------------------
    class CParamStorageBase
    {
    public:

        virtual ~CParamStorageBase() {}
        virtual void* GetData() = 0;

    private:

        friend class boost::serialization::access;
        template< class Archive > void serialize(Archive& ar, const unsigned int version) {}
    };

    
    //-----------------------------------------------------------------------------------
    template< class T >
    class CParamStorage : public CParamStorageBase
    {
    public:

        CParamStorage() {}

        CParamStorage(const T& val)
            : m_value(val) {}

        virtual ~CParamStorage() {}

        virtual void* GetData() 
        {
            return &m_value;
        }

    private:

        CParamStorage(const CParamStorage& rhs);
        CParamStorage& operator=(const CParamStorage& rhs);

        friend class boost::serialization::access;
        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CParamStorageBase >(*this);
            ar & m_value;
        }

    public:
        T m_value;
    };

    //-----------------------------------------------------------------------------------
    typedef CParamStorage<bool>                 CBoolParamStorage;
    typedef CParamStorage<float>                CFloatParamStorage;
    typedef CParamStorage<std::string>          CStringParamStorage;
    typedef CParamStorage<int>                  CIntParamStorage;
    typedef CParamStorage<Vec2>                 CVector2ParamStorage;
    typedef CParamStorage<Vec3>                 CVector3ParamStorage;
    typedef CParamStorage<Vec4>                 CVector4ParamStorage;
    typedef CParamStorage<paramStorage::Vec4>   CVec4ParamStorage;

    //-----------------------------------------------------------------------------------
    class CBaseParam
    {
    public:

        enum PARAMETER_TYPE
        {
            PARAMETER_TYPE_VOID = 0,
            PARAMETER_TYPE_STRING,
            PARAMETER_TYPE_FLOAT,
            PARAMETER_TYPE_BOOL,
            PARAMETER_TYPE_INT,
            PARAMETER_TYPE_VECTOR3,
            PARAMETER_TYPE_VECTOR4,
            PARAMETER_TYPE_VECTOR2
        };

        explicit CBaseParam();
        explicit CBaseParam(const CBaseParam& rhs);
        CBaseParam& operator=(const CBaseParam& rhs);

        template<class T> CBaseParam(const T& t)
            : m_pValue(nullptr)
            , m_Type(PARAMETER_TYPE_VOID)
            , m_Size(0)
            , m_Columns(1)
            , m_Rows(1)
        {
            operator=(t);
        }
        ~CBaseParam();

        PARAMETER_TYPE GetType() const;
        int GetRows() const;
        int GetCols() const;
        int GetSize() const;
        void* GetValue() const;

        operator float() const;
        operator bool() const;
        operator int() const;
        operator unsigned int() const;
        operator Vec2() const;
        operator Vec3() const;
        operator Vec4() const;
        operator std::string&() const;

        CBaseParam& operator=(bool bVal);
        CBaseParam& operator=(float Val);
        CBaseParam& operator=(int Val);
        CBaseParam& operator=(unsigned int Val);
        CBaseParam& operator=(const Vec2& vVal);
        CBaseParam& operator=(const Vec3& vVal);
        CBaseParam& operator=(const Vec4& vVal);
        CBaseParam& operator=(const std::string& str);

        CBaseParam& operator[](int Index);

        const std::string& AsString();

        bool IsValid() const { return m_Type != PARAMETER_TYPE_VOID &&
                                      m_pValue != nullptr; }

    private:

        template< class T > 
        void AssignValue(const T& val, PARAMETER_TYPE paramType);

        void Free();

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & m_pValue;
            ar & m_Type;
            ar & m_Columns;
            ar & m_Rows;
            ar & m_Size;
        }

    private:

        PARAMETER_TYPE m_Type;

        CParamStorageBase* m_pValue;

        int m_Columns;
        int m_Rows;
        int m_Size;
    };

} // env
#endif // BASEPARAM_H
