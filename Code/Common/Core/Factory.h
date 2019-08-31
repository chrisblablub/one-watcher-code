/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef FACTORY_H
#define FACTORY_H

#include <Common/Common.h>

namespace env
{
    template< class ReturnType > struct IFactory
    {
        virtual ~IFactory() {}
        virtual ReturnType* Create() = 0;
    };

    template< class ReturnType, class CreateType > 
    struct CFactory : IFactory< ReturnType >
    {
        virtual ~CFactory() {}
        virtual ReturnType* Create()
        {
            return new CreateType();
        }
    };

} // env 
#endif // FACTORY_H