#include <Common/Core/BaseParam.h>

namespace env
{

    //-----------------------------------------------------------------------------------
    CBaseParam::CBaseParam()
        : m_pValue(nullptr)
        , m_Type(PARAMETER_TYPE_VOID)
        , m_Size(0)
        , m_Columns(1)
        , m_Rows(1) {}

    //-----------------------------------------------------------------------------------
    CBaseParam::CBaseParam(const CBaseParam& rhs)
        : m_pValue(nullptr)
        , m_Type(PARAMETER_TYPE_VOID)
        , m_Size(0)
        , m_Columns(1)
        , m_Rows(1) 
    {
        *this = rhs;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::~CBaseParam()
    {
        Free();
        
    }

    //-----------------------------------------------------------------------------------
    void CBaseParam::Free()
    {
        ENV_DELETE(m_pValue);
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::PARAMETER_TYPE CBaseParam::GetType() const
    {
        return m_Type;
    }

    //-----------------------------------------------------------------------------------
    int CBaseParam::GetCols() const
    {
        return m_Columns;
    }

    //-----------------------------------------------------------------------------------
    void* CBaseParam::GetValue() const
    {
        return m_pValue;
    }

    //-----------------------------------------------------------------------------------
    int CBaseParam::GetSize() const
    {
        return m_Size;
    }

    //-----------------------------------------------------------------------------------
    int CBaseParam::GetRows() const
    {
        return m_Rows;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator float() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_FLOAT);
        return *(float*)(m_pValue->GetData());
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator bool() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_BOOL);
        return *(bool*)(m_pValue->GetData());
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator int() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_INT);
        return *(int*)(m_pValue->GetData());
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator unsigned int() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_INT);
        return *(unsigned int*)(m_pValue->GetData());
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator Vec2() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_VECTOR2);
        return *(Vec2*)(m_pValue->GetData());
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator Vec3() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_VECTOR3);
        return *(Vec3*)(m_pValue->GetData());
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator Vec4() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_VECTOR4);

        paramStorage::Vec4* tmp = static_cast<paramStorage::Vec4*>(m_pValue->GetData());

        return Vec4(tmp->x, tmp->y, tmp->z, tmp->w);
    }

    //-----------------------------------------------------------------------------------
    CBaseParam::operator std::string&() const
    {
        ENV_ASSERT_DBG(m_pValue);
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_STRING);

        return *(std::string*)(m_pValue->GetData());
    }


    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(const CBaseParam& rhs)
    {
        Free();

        if (rhs.m_Type == PARAMETER_TYPE_STRING)
        {
            *this = (std::string)rhs;
        }
        else if (rhs.m_Type == PARAMETER_TYPE_FLOAT)
        {
            *this = (float)(rhs);
        }
        else if (rhs.m_Type == PARAMETER_TYPE_VECTOR2)
        {
            Vec2 tmp = rhs;
            *this = tmp;
        }
        else if (rhs.m_Type == PARAMETER_TYPE_VECTOR3)
        {
            Vec3 tmp = rhs;
            *this = tmp;
        }
        else if (rhs.m_Type == PARAMETER_TYPE_VECTOR4)
        {
            Vec4 tmp = rhs;
            *this = tmp;
        }
        else if (rhs.m_Type == PARAMETER_TYPE_BOOL)
        {
            *this = (bool)rhs;
        }
        else if (rhs.m_Type == PARAMETER_TYPE_INT)
        {
            *this = (int)rhs;
        }
        else if (rhs.m_Type == PARAMETER_TYPE_VOID)
        {
            ENV_DELETE(m_pValue);
            m_Size = 0;
            m_Rows = 0;
            m_Columns = 0;
            m_Type = PARAMETER_TYPE_VOID;
            //ENV_ASSERT_DBG(false && "SHOULD NOT HAPPEN!");
        }
        else
        {
            ENV_ASSERT_DBG(false && "SHOULD NOT HAPPEN!");
        }

        return *this;
    }

    //-----------------------------------------------------------------------------------
    template< class T > 
    void CBaseParam::AssignValue(const T& val, CBaseParam::PARAMETER_TYPE paramType)
    {
        ENV_ASSERT_DBG(m_Type == PARAMETER_TYPE_VOID || 
                       m_Type == paramType);

        ENV_DELETE(m_pValue);

        m_Type = paramType;
        CParamStorage<T>* p = new CParamStorage<T>(val);
        m_pValue = p;
        m_Size = sizeof(T);
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(bool bVal)
    {
        AssignValue(bVal, PARAMETER_TYPE_BOOL);

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(float Val)
    {
        AssignValue(Val, PARAMETER_TYPE_FLOAT);

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(int Val)
    {
        AssignValue(Val, PARAMETER_TYPE_INT);

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(const Vec2& vVal)
    {
        AssignValue(vVal, PARAMETER_TYPE_VECTOR2);

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(const Vec3& vVal)
    {
        AssignValue(vVal, PARAMETER_TYPE_VECTOR3);

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(const Vec4& vVal)
    {
        paramStorage::Vec4 v;
        v.x = vVal.x;
        v.y = vVal.y;
        v.z = vVal.z;
        v.w = vVal.w;

        AssignValue(v, PARAMETER_TYPE_VECTOR4);

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator=(const std::string& str)
    {
        AssignValue(str, PARAMETER_TYPE_STRING);

        return *this;
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CBaseParam::operator[](int Index)
    {
        return *this;
    }

    const std::string& CBaseParam::AsString()
    {
        return operator std::string&();
    }
} // env