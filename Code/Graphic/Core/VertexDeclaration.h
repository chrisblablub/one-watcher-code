/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef VERTEX_DECLARATION_H
#define VERTEX_DECLARATION_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/GraphicConfig.h>

namespace env
{
    enum VERTEX_DECLARATION_ELEMENTS_TYPE
    {
        VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1 = 0,
        VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2,
        VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3,
        VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4,
        VERTEX_DECLARATION_ELEMENTS_TYPE_ULONG
    };

    enum VERTEX_DECLARATION_USAGE_TYPE
    {
        VERTEX_DECLARATION_USAGE_TYPE_POSITION = 0,
        VERTEX_DECLARATION_USAGE_TYPE_COLOR,
        VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD,
        VERTEX_DECLARATION_USAGE_TYPE_NORMAL,
        VERTEX_DECLARATION_USAGE_TYPE_TANGENT,
        VERTEX_DECLARATION_USAGE_TYPE_BINORMAL
    };

    struct VertexDeclarationElement
    {
        VertexDeclarationElement() {}

        VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE type,
                                 VERTEX_DECLARATION_USAGE_TYPE usage, unsigned int usageIndex)
                                 : m_type(type),
                                 m_usage(usage),
                                 m_usageIndex(usageIndex) {}

        VERTEX_DECLARATION_ELEMENTS_TYPE m_type;
        VERTEX_DECLARATION_USAGE_TYPE m_usage;

        unsigned int m_usageIndex;
    };

    typedef std::vector< VertexDeclarationElement > vecVertexDeclarationElements;

} // env

#endif // VERTEX_DECLARATION_H 

#include GRAPHIC_API_VERTEXDECLARATION

