/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_VERTEX_BUFFER_H
#define D3D11_VERTEX_BUFFER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D11/D3D11Config.h>

namespace env
{
    class CVertexBuffer
    {
    public:

        CVertexBuffer(ID3D11DeviceContext* pDevice,
                      ID3D11Buffer* pVertexBuffer);

        ~CVertexBuffer();

        void* Lock(unsigned int offset, unsigned int size, unsigned long flags);

        void Unlock();

        void Bind(unsigned int streamNumber, unsigned int offset, unsigned int stride);

    protected:
        ID3D11Buffer* m_pVertexBuffer;
        ID3D11DeviceContext* m_pDeviceContext;

        unsigned int m_numVertices;

        void* m_pLockedData;
    };

} // env

#endif // D3D11_VERTEX_BUFFER_H
#endif // ENV_GRAPHIC_API_D3D11
