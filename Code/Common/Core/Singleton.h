/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

#include <Common/Common.h>

//#include "Exceptions.h"

namespace env
{
    template< class Type > class CSingletonStatic
    {
    public:
        CSingletonStatic() {}
        ~CSingletonStatic() {}

        inline Type* operator->()
        {
            return GetInstance();
        }

        inline static Type& GetRef()
        {
            return *GetInstance();
        }

    protected:
        inline static Type* GetInstance()
        {
            static Type instance;
            if (!m_pInstance)
            {
                m_pInstance = &instance;
                std::atexit(Kill);
            }
            else if (m_bDestroyed)
            {
                new(&instance) Type;
                std::atexit(KillPhoenix);
                m_bDestroyed = false;
            }
            return &instance;
        }

        inline static void Kill()
        {
            m_bDestroyed = true;
        }
        inline static void KillPhoenix()
        {
            m_pInstance->~Type();
            m_bDestroyed = true;
        }

        static bool m_bDestroyed;
        static Type* m_pInstance;
    };

    template< class Type > bool CSingletonStatic< Type >::m_bDestroyed = false;
    template< class Type > Type* CSingletonStatic< Type >::m_pInstance = NULL;


    template< class KeyType, class ValueType >
    class CSingletonMap : public CSingletonStatic<std::map< KeyType, ValueType>>
    {
    public:
        typedef std::map< KeyType, ValueType > MapType;
        typename MapType::value_type::second_type& operator[](const typename MapType::key_type& param)
        {
            return CSingletonStatic<std::map< KeyType, ValueType>>::GetRef()[param];
        }
    };


    template< class T > 
    class CSingleton
    {
    private:
        CSingleton(const CSingleton<T>&);
        CSingleton& operator=(const CSingleton<T>&);

    protected:
        CSingleton() {}

    public:

        inline static T& GetInstance() 
        { 
            static T inst;
            return inst;
        }

    };


//     template< class T > class CSingleton
//     {
//     public:
//         inline static T& GetInstance()
//         {
//             assert(m_pSingleton);
//             if (!m_pSingleton)
//             {
//                 //ENV_EXCEPTION( CExcBase::EXC_OBJ_NOT_VALID, "Called GetInstance() on a non existing singleton." );
//             }
//             return *m_pSingleton;
//         }
// 
//         inline static T* GetInstancePtr()
//         {
//             assert(m_pSingleton);
//             return m_pSingleton;
//         }
// 
// 
//     protected:
//         CSingleton(void)
//         {
//             /*
//              * If this assertion failed, this means that there exists
//              * an instance already.
//              */
//             assert(!m_pSingleton);
//             m_pSingleton = static_cast<T*>(this);
//         }
// 
//         ~CSingleton(void)
//         {
//             assert(m_pSingleton);
//             m_pSingleton = NULL;
//         }
// 
//         static T* m_pSingleton;
// 
//     private:
//         /** Explicit private copy constructor. This is a forbidden operation.*/
//         CSingleton(const CSingleton<T> &);
// 
//         /** Private operator= . This is a forbidden operation. */
//         CSingleton& operator=(const CSingleton<T> &);
//     };

    //template< class T > T* CSingleton<T>::m_pSingleton = NULL;
}
#endif /* SINGLETON_H */