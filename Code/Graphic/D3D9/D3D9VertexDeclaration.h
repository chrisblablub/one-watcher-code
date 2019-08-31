/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_VERTEX_DECLARATION_H
#define D3D9_VERTEX_DECLARATION_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>
#include <Graphic/Core/VertexDeclaration.h>

namespace env
{
    class CD3D9VertexDeclaration
    {
    public:
        CD3D9VertexDeclaration(IDirect3DDevice9* pDevice, 
                               vecVertexDeclarationElements& vertexDeclarationElements,
                               unsigned int stride);

        virtual ~CD3D9VertexDeclaration();

        virtual void Bind();

        virtual unsigned int GetStride();

        virtual void GetDeclaration(D3DVERTEXELEMENT9* pElements, unsigned int& numElements);

    private:
        BYTE ConvertToD3DType(VERTEX_DECLARATION_ELEMENTS_TYPE type);

        BYTE ConvertToD3DUsage(VERTEX_DECLARATION_USAGE_TYPE usage);

        unsigned int ConvertToD3DSize(VERTEX_DECLARATION_ELEMENTS_TYPE type);

    private:
        IDirect3DVertexDeclaration9* m_pDecl;
        IDirect3DDevice9* m_pDevice;

        vecVertexDeclarationElements m_vertexDeclarationElements;

        unsigned int m_stride;
    };

    typedef CD3D9VertexDeclaration CVertexDeclaration;
} // env

#endif // D3D9_VERTEX_DECLARATION_H
#endif // D3D9