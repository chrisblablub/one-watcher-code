/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_VERTEX_DECLARATION_H
#define D3D11_VERTEX_DECLARATION_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>
#include <Graphic/Core/VertexDeclaration.h>

namespace env
{
    class CVertexDeclaration
    {
    public:

        typedef std::vector<D3D11_INPUT_ELEMENT_DESC> vecD3D11VertexDeclaration;

        CVertexDeclaration(ID3D11Device* pDevice,
                           ID3D11DeviceContext* pDeviceContext,
                           ID3DBlob* pShaderSignature,
                           vecVertexDeclarationElements& vertexDeclarationElements,
                           unsigned int stride);

        ~CVertexDeclaration();

        void Bind();

        unsigned int GetStride() const { return m_stride; }

    private:
        DXGI_FORMAT ConvertToD3DFormat(VERTEX_DECLARATION_ELEMENTS_TYPE type);

        std::string ConvertToD3DSemantic(VERTEX_DECLARATION_USAGE_TYPE usage);

        unsigned int ConvertToD3DSize(VERTEX_DECLARATION_ELEMENTS_TYPE type);

    private:
        ID3D11Device* m_pDevice;
        ID3D11DeviceContext* m_pDeviceContext;
        ID3D11InputLayout* m_pInputLayout;

        vecD3D11VertexDeclaration    m_d3d11VertexDeclaration;
        vecVertexDeclarationElements m_vertexDeclarationElements;

        unsigned int m_stride;
    };
} // env

#endif // D3D11_VERTEX_DECLARATION_H
#endif // ENV_GRAPHIC_API_D3D11
