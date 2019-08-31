/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <Common/Common.h>
#include <Common/Core/ResMgrBase.h>
#include <Common/Core/BaseParam.h>

namespace env
{
    class CScript 
    {
    public:

        typedef std::map< std::string, CScript* > mapSubscripts;
        typedef std::map< std::string, CBaseParam > mapParams;

        typedef CResMgrBase< std::string, CScript > CScriptMgr;

        enum SCRIPT_EVENTS { SCRIPT_RELOADED = 0 };

        CScript();
        ~CScript();

        bool SaveScript(const std::string& strFilename);
        bool SaveScript();

        CBaseParam& operator[]( const std::string& str );
        CBaseParam& operator[]( const char* pstr );

        CScript* AddSubscript(const std::string& strName);
        void RemoveSubscript(const std::string& strName);
        void RemoveSubscripts();
        CScript* GetSubscript(const std::string& strName);
        bool SubscriptExists(const std::string& strName);

        bool Exists( const std::string& strName );
        bool GetNext(std::string& strVarName, CBaseParam::PARAMETER_TYPE& ParamType);
        bool IsLoaded()	{ return m_bScriptLoaded; }

        mapSubscripts& GetSubscripts();
        const std::string& GetFilename();

        std::string GetString(const std::string& strParam);
        float GetFloat(const std::string& strParam);
        bool GetBool(const std::string& strParam);
        int GetInt(const std::string& strParam);
        unsigned int GetUInt(const std::string& strParam);
        Vec3 GetVec3(const std::string& strParam);
        Vec4 GetVec4(const std::string& strParam);


        void Reload() { LoadResource(); }

    private:
        

        friend class CResMgrBase< std::string, CScript >;
        friend struct DefaultAllocationPolicy< CScript >;

        bool InitResource(const std::string& strName, const std::string& str, FILETIME* pFt);
        bool LoadResource();

        //for subscripts
        CScript(std::ifstream& File);
        bool SaveScript( std::ofstream& ofstr, int tabs );

        bool Load(std::ifstream& File);

        bool m_bScriptLoaded;
        bool m_bModified;

        mapParams::iterator m_Itr;
        mapParams m_Params;
        std::map< std::string, CScript* > m_SubScripts;

        std::string m_strFile;
    };
    
    typedef CScript::CScriptMgr CScriptMgr;

    extern CScriptMgr* gScriptMgr;

} // env
#endif // SCRIPTSYSTEM_H