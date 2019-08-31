#include <Common/Core/Script.h>
#include <Common/Core/Macros.h>

namespace env
{
    CScriptMgr* gScriptMgr = nullptr;

    //-----------------------------------------------------------------------------------
    CScript::CScript()
        : m_bScriptLoaded(true)
        , m_bModified(true)
    {
        m_Itr = m_Params.begin();
    }

    //-----------------------------------------------------------------------------------
    CScript::CScript(std::ifstream& File)
    {
        m_bModified = false;
        if (File.is_open())
        {
            m_bScriptLoaded = true;
            Load(File);
        }
        m_Itr = m_Params.begin();
    }

    //-----------------------------------------------------------------------------------
    bool CScript::InitResource(const std::string& strName, const std::string& str, FILETIME* pFt)
    {
        m_strFile = strName;

        bool bResult = LoadResource();
        if (!bResult)
            return false;

        GetFileLastWriteTime(m_strFile, pFt);
        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CScript::LoadResource()
    {
        m_bModified = false;
        m_bScriptLoaded = false;
        m_Params.clear();
        m_SubScripts.clear();
        m_Itr = m_Params.begin();

        // Open the file.
        {
            std::ifstream File(m_strFile.c_str());
            if (File.is_open())
            {
                m_bScriptLoaded = true;
                Load(File);
                File.close();
            }
            else
            {
                //testing! create file if it doesn't exist
                std::ofstream FileToCreate(m_strFile.c_str());
                if (FileToCreate.is_open())
                    m_bScriptLoaded = true;
                else
                    return false;
            }
            m_Itr = m_Params.begin();

        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CScript::Load(std::ifstream& File)
    {
        File >> std::setiosflags(std::ios_base::boolalpha);
        std::string strBuffer;

        // Read all variables
        //
        bool bReading = false;

        while (!File.fail())
        {
            // Search for { to start reading.
            if (!bReading)
            {
                File >> strBuffer;
                if (strBuffer == "{")
                    bReading = true;
            }
            else
            {
                std::string strNameBuffer, strTypeBuffer;
                File >> strTypeBuffer;
                if (strTypeBuffer != "}")
                    File >> strNameBuffer;
                else
                    break;

                if (m_SubScripts.find(strNameBuffer) == m_SubScripts.end()
                    && m_Params.find(strNameBuffer) == m_Params.end())
                {
                    if (strTypeBuffer == "bool")
                    {
                        bool buffer = false;
                        File >> buffer;
                        m_Params[strNameBuffer] = buffer;
                    }
                    else if (strTypeBuffer == "string")
                    {
                        std::string buffer = "";
                        //for the ' '
                        char c;
                        File.get(c);
                        getline(File, buffer);
                        m_Params[strNameBuffer] = buffer;
                    }
                    else if (strTypeBuffer == "float")
                    {
                        float buffer = 0.0f;
                        File >> buffer;
                        m_Params[strNameBuffer] = buffer;
                    }
                    else if (strTypeBuffer == "int")
                    {
                        int buffer = 0;
                        File >> buffer;
                        m_Params[strNameBuffer] = buffer;
                    }
                    else if (strTypeBuffer == "vector3")
                    {
                        Vec3 buffer = Vec3(0.0f, 0.0f, 0.0f);
                        File >> buffer.x >> buffer.y >> buffer.z;
                        m_Params[strNameBuffer] = buffer;
                    }
                    else if (strTypeBuffer == "vector4")
                    {
                        Vec4 buffer = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
                        File >> buffer.x >> buffer.y >> buffer.z >> buffer.w;
                        m_Params[strNameBuffer] = buffer;
                    }
                    else if (strTypeBuffer == "subscript")
                    {
                        m_SubScripts[strNameBuffer] = new CScript(File);
                    }
                }
            }
        }
        return true;
    }

    //-----------------------------------------------------------------------------------
    CScript* CScript::AddSubscript(const std::string& strName)
    {
        std::map<std::string, CScript*>::iterator it = m_SubScripts.find(strName);
        if (it == m_SubScripts.end())
        {
            std::ifstream ifstr;
            CScript* pScript = new CScript(ifstr);
            m_SubScripts[strName] = pScript;
            return pScript;
        }
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    void CScript::RemoveSubscript(const std::string& strName)
    {
        m_SubScripts.erase(strName);
    }

    //-----------------------------------------------------------------------------------
    void CScript::RemoveSubscripts()
    {
        for (mapSubscripts::iterator it = m_SubScripts.begin(); it != m_SubScripts.end(); ++it)
            ENV_DELETE(it->second);

        m_SubScripts.clear();
    }

    //-----------------------------------------------------------------------------------
    CScript* CScript::GetSubscript(const std::string& strName)
    {
        if (m_SubScripts.find(strName) != m_SubScripts.end())
            return m_SubScripts[strName];
        return 0;
    }

    //-----------------------------------------------------------------------------------
    bool CScript::SubscriptExists(const std::string& strName)
    {
        return m_SubScripts.find(strName) != m_SubScripts.end();
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CScript::operator[](const std::string& str)
    {
        return m_Params[str];
    }

    //-----------------------------------------------------------------------------------
    CBaseParam& CScript::operator[](const char* pstr)
    {
        return m_Params[std::string(pstr)];
    }

    //-----------------------------------------------------------------------------------
    std::string CScript::GetString(const std::string& strParam)
    {
        mapParams::iterator it = m_Params.find(strParam);
        if (it == m_Params.end())
            return std::string();
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    float CScript::GetFloat(const std::string& strParam)
    {
        mapParams::iterator it = m_Params.find(strParam);
        if (it == m_Params.end())
            return 0.0f;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    bool CScript::GetBool(const std::string& strParam)
    {
        mapParams::iterator it = m_Params.find(strParam);
        if (it == m_Params.end())
            return false;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    int CScript::GetInt(const std::string& strParam)
    {
        mapParams::iterator it = m_Params.find(strParam);
        if (it == m_Params.end())
            return 0;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    unsigned int CScript::GetUInt(const std::string& strParam)
    {
        mapParams::iterator it = m_Params.find(strParam);
        if (it == m_Params.end())
            return 0;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    Vec3 CScript::GetVec3(const std::string& strParam)
    {
        mapParams::iterator it = m_Params.find(strParam);
        if (it == m_Params.end())
            return Vec3(0.0f, 0.0f, 0.0f);
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    Vec4 CScript::GetVec4(const std::string& strParam)
    {
        mapParams::iterator it = m_Params.find(strParam);
        if (it == m_Params.end())
            return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    CScript::~CScript()
    {
        while (m_SubScripts.size() > 0)
        {
            ENV_DELETE(m_SubScripts.begin()->second);
            m_SubScripts.erase(m_SubScripts.begin());
        }
    }

    //-----------------------------------------------------------------------------------
    bool CScript::SaveScript(const std::string& strFilename)
    {
        m_strFile = strFilename;

        return SaveScript();
    }

    //-----------------------------------------------------------------------------------
    bool CScript::SaveScript()
    {
        std::ofstream ofstr(m_strFile.c_str());
        ofstr << std::setiosflags(std::ios_base::boolalpha);

        SaveScript(ofstr, 1);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CScript::SaveScript(std::ofstream& ofstr, int tabs)
    {
        {
            for (int i = 0; i < tabs - 1; ++i)
                ofstr << "\t";
            ofstr << "{\n";

            std::map< std::string, CBaseParam >::iterator it;
            for (it = m_Params.begin(); it != m_Params.end(); ++it)
            {
                CBaseParam& Param = it->second;
                for (unsigned int i = 0; i < tabs; ++i)
                    ofstr << "\t";

                switch (Param.GetType())
                {
                case CBaseParam::PARAMETER_TYPE_BOOL:
                {
                    bool bValue = Param;
                    ofstr << "bool " << it->first << " " << bValue << "\n";
                } break;

                case CBaseParam::PARAMETER_TYPE_INT:
                {
                    int Value = Param;
                    ofstr << "int " << it->first << " " << Value << "\n";
                } break;

                case CBaseParam::PARAMETER_TYPE_FLOAT:
                {
                    float Value = Param;
                    ofstr << "float " << it->first << " " << std::setprecision(4) << Value << "\n";
                } break;

                case CBaseParam::PARAMETER_TYPE_VECTOR3:
                {
                    Vec3 vVal = Param;
                    ofstr << "vector3 " << it->first << " " << std::setprecision(4) << vVal.x << " " << vVal.y << " " << vVal.z << "\n";
                } break;

                case CBaseParam::PARAMETER_TYPE_VECTOR4:
                {
                    Vec4 vVal = Param;
                    ofstr << "vector4 " << it->first << " " << std::setprecision(4) << vVal.x << " " << vVal.y << " " << vVal.z << " " << vVal.w << "\n";
                } break;

                case CBaseParam::PARAMETER_TYPE_STRING:
                {
                    std::string& str = Param;
                    ofstr << "string " << it->first << " " << str << "\n";
                } break;
                case CBaseParam::PARAMETER_TYPE_VOID:
                {    
                    ENV_ASSERT_DBG(false && "SHOULD NOT HAPPEN");
                } break;
                default:
                {
                    ENV_ASSERT_DBG(false && "SHOULD NOT HAPPEN");
                }
                }
            }

            for (std::map< std::string, CScript* >::iterator it = m_SubScripts.begin(); it != m_SubScripts.end(); ++it)
            {
                for (int i = 0; i < tabs; ++i)
                    ofstr << "\t";
                ofstr << "subscript " << it->first << "\n";
                it->second->SaveScript(ofstr, tabs + 1);
            }
            for (int i = 0; i < tabs - 1; ++i)
                ofstr << "\t";
            ofstr << "}\n";
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CScript::Exists(const std::string& strName)
    {
        std::map< std::string, CBaseParam >::iterator it = m_Params.find(strName);
        if (it == m_Params.end())
            return false;
        else
            return true;
    }

    //-----------------------------------------------------------------------------------
    bool CScript::GetNext(std::string& strVarName, CBaseParam::PARAMETER_TYPE& ParamType)
    {
        if (m_Itr == m_Params.end())
        {
            m_Itr = m_Params.begin();
            return false;
        }

        strVarName = m_Itr->first;
        CBaseParam& Param = m_Itr->second;
        ParamType = Param.GetType();

        m_Itr++;
        return true;
    }

    //-----------------------------------------------------------------------------------
    std::map< std::string, CScript* >& CScript::GetSubscripts()
    {
        return m_SubScripts;
    }

    //-----------------------------------------------------------------------------------
    const std::string& CScript::GetFilename()
    {
        return m_strFile;
    }
} // env