/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef RESMGRBASE_H
#define RESMGRBASE_H

#include <Common/Common.h>

ENV_PLATFORM_ORBIS_ONLY(struct _FILETIME {};);
ENV_PLATFORM_ORBIS_ONLY(typedef _FILETIME FILETIME;);

namespace env
{
    template< class Type >
    struct DefaultAllocationPolicy
    {
        Type* Create() { return new Type(); }
    };

    template < class Key >
    struct DefaultReloadPolicy
    {
        bool Reload(const Key& key, FILETIME& fileTime)
        {
#ifdef ENV_PLATFORM_WINDOWS
            WIN32_FIND_DATA data;
            HANDLE hFile = FindFirstFile(key.c_str(), &data);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                FindClose(hFile);
                return false;
            }
          
            // Set up the current system time minus 0.5s
            //
            SYSTEMTIME st;
            GetSystemTime(&st);
            FILETIME stft;
            SystemTimeToFileTime(&st, &stft);

            unsigned long long tmp = (((ULONGLONG)stft.dwHighDateTime) << 32) + stft.dwLowDateTime;
            tmp -= (5000000);
            stft.dwLowDateTime = (DWORD)(tmp & 0xFFFFFFFF);
            stft.dwHighDateTime = (DWORD)(tmp >> 32);

            // Check if file's current lastWriteTime > it's stored lastWriteTime and
            // if it's current lastWriteTime < sysTime - 0.5s (i.e. avoid to reload the file,
            // while it is still being written).
            //
            bool bReload = (CompareFileTime(&data.ftLastWriteTime, &fileTime) == 1 && CompareFileTime(&stft, &data.ftLastWriteTime) == 1);

            fileTime = data.ftLastWriteTime;

            FindClose(hFile);

            return bReload;
#else
            return false;
#endif // ENV_PLATFORM_WINDOWS
        }
    };

    template < 
               class Key, 
               class Type, 
               class AllocationPolicy = DefaultAllocationPolicy<Type>,
               class ReloadPolicy = DefaultReloadPolicy< Key >
             >
    class CResMgrBase
    {
    public:

        typedef AllocationPolicy AllocPolicyType;
        typedef ReloadPolicy     ReloadPolicyType;

    private:
        template< class ResourceType > struct ResourceNode final
        {
            ResourceNode()
                : refCtr(0) {}

            ResourceNode(std::unique_ptr< ResourceType >&& p)
                : ptr(std::move(p)), refCtr(1) 
            {
                p.release();
            }

            ResourceNode(const ResourceNode< ResourceType >& rhs)
                : ptr(rhs.ptr), refCtr(rhs.refCtr) {}

            ResourceNode(ResourceNode&& n)
                : ptr(n.ptr), refCtr(n.refCtr), ft(n.ft) {}

            ResourceNode& operator=(const ResourceNode& n) = delete;
            
            ResourceNode& operator=(ResourceNode&& n)
            {
                ptr = std::move(n.ptr);
                refCtr = n.refCtr;
                ft = n.ft;

                n.ptr.release();

                return *this;
            }

            std::unique_ptr< ResourceType > ptr;
            int refCtr;

            _FILETIME ft;
        };

        AllocationPolicy m_creator;

    public:

        //-----------------------------------------------------------------------------------
        CResMgrBase() {}
        
        CResMgrBase(const Key& rootKey) 
            : m_rootKey(rootKey) {}

        CResMgrBase(const Key& rootKey, const AllocationPolicy& creator)
            : m_rootKey(rootKey), m_creator(creator) {}

        //-----------------------------------------------------------------------------------
        CResMgrBase(const CResMgrBase&) = delete;
        CResMgrBase& operator=(const CResMgrBase&) = delete;

        //-----------------------------------------------------------------------------------
        virtual ~CResMgrBase()
        {
            if (!m_Resources.empty())
            {
                ENV_LOG_ERROR("FOUND UNRELEASED RESOURCES:");
                for (const auto& resNodeKeyValue : m_Resources)
                {
                    ENV_LOG_INFO(resNodeKeyValue.first);
                }
            }
            ENV_ASSERT_DBG(m_Resources.empty());
        }

        void SetAllocPolicy(const AllocPolicyType& creator) { m_creator = creator; }
        AllocPolicyType& GetAllocPolicy() { return m_creator; }

        void SetResourceRoot(const Key& rootKey) { m_rootKey = rootKey; }

        //-----------------------------------------------------------------------------------
        template<class... Args>
        void OnLostDevice(const Args&... args) 
        {
            for (const auto& resource : m_Resources)
            {
                const ResourceNode< Type >& resourceNode = resource.second;

                ENV_ASSERT_DBG(resourceNode.ptr.get());
   
                resourceNode.ptr->OnLostDevice(args...);
            }
        }

        //-----------------------------------------------------------------------------------
        template<class... Args>
        void OnResetDevice(const Args&... args)
        {
            for (const auto& resource : m_Resources)
            {
                const ResourceNode< Type >& resourceNode = resource.second;

                ENV_ASSERT_DBG(resourceNode.ptr.get());

                resourceNode.ptr->OnResetDevice(args...);
            }
        }

        //-----------------------------------------------------------------------------------
        template<class Lambda>
        void CallForEach()
        {
            for (const auto& resource : m_Resources)
            {
                const ResourceNode< Type >& resourceNode = resource.second;

                Lambda(resourceNode.ptr.get());
            }
        }

        //-----------------------------------------------------------------------------------
        void Release(Type* p)
        {
            if (p != nullptr)
                Release(p->GetName());
        }

        //-----------------------------------------------------------------------------------
        void Release(Key key)
        {
            if(m_Resources.size() > 0 )
            {
                const auto& it = m_Resources.find(key);

                if(it == m_Resources.end())
                    return;

                ResourceNode< Type >& resNode = it->second;
                --resNode.refCtr;

                if(resNode.refCtr == 0)
                    m_Resources.erase(it);
            }
        }

        //-----------------------------------------------------------------------------------
        void ReleaseAll()
        {
            m_Resources.clear();
        }

        //-----------------------------------------------------------------------------------
        Type* GetPtr(const Key& key)
        {
            const auto& it = m_Resources.find(key);

            if(it == m_Resources.end())
                return nullptr;
            
            ResourceNode< Type >& resNode = it->second;
            ++resNode.refCtr;
            return resNode.ptr.get();
        }

        //-----------------------------------------------------------------------------------
        template<class... Args>
        Type* AddGetPtr(const Key& key, const Args&... args)
        {
            if (Type* ptr = GetPtr(key))
                return ptr;

            ResourceNode<Type> node(std::unique_ptr<Type>(m_creator.Create()));

            if (!node.ptr.get() || 
                !node.ptr->InitResource(m_rootKey + key, key, &node.ft, args...))
            {
                return nullptr;
            }

            return Insert(std::move(node), key);
        }

        //-----------------------------------------------------------------------------------
        void IncreaseRefCounter()
        {
            for (const auto& it : m_Resources)
            {
                ++(it.second.refCtr);
            }
        }

        //-----------------------------------------------------------------------------------
        void DecreaseRefCounter()
        {
            for (const auto& it : m_Resources)
            {
                --(it.second.refCtr);
            }
        }

        //-----------------------------------------------------------------------------------
        bool UpdateResources()
        {
            bool bReturn = false;
            for(auto& it : m_Resources)
            {
                ReloadPolicy reloadType;

                if (reloadType.Reload(m_rootKey + it.first, it.second.ft))
                {
                    it.second.ptr->LoadResource();
                }
            }
            return bReturn;
        }

        //-----------------------------------------------------------------------------------
        void Update(float dt)
        {
            for(const auto& it : m_Resources)
                it.second.ptr->Update(dt);
        }

    private:

        //-----------------------------------------------------------------------------------
        Type* Insert(ResourceNode<Type>&& node, Key key)
        {
            Type* pRet = node.ptr.get();
            m_Resources[ key ] = std::move(node);
            m_itrResources = m_Resources.begin();
            return pRet;
        }

    private:
        Key m_rootKey;

        std::map<Key, ResourceNode<Type>> m_Resources;
        typename std::map<Key, ResourceNode< Type > >::iterator m_itrResources;
    };

} // env
#endif // RESMGRBASE_H