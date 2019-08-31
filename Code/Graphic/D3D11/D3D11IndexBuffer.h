/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef ENV_GRAPHIC_API_D3D11

#pragma once
#ifndef D3D11_INDEX_BUFFER_H
#define D3D11_INDEX_BUFFER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/APITypes.h>
#include <Graphic/D3D11/D3D11Config.h>

namespace env
{
    class CIndexBuffer
    {
    public:
        CIndexBuffer(ID3D11DeviceContext* pDeviceContext,
                          ID3D11Buffer* pIndexBuffer, 
                          TEXTUREFORMAT_TYPE format);

        ~CIndexBuffer();

        void Bind();

        void* Lock(unsigned int offset, unsigned int size, unsigned long flags);

        void Unlock();

        TEXTUREFORMAT_TYPE GetFormat() const { return m_format; }

    protected:
        ID3D11Buffer* m_pIndexBuffer;
        ID3D11DeviceContext* m_pDeviceContext;
        TEXTUREFORMAT_TYPE m_format;
    };

} // env

#endif // D3D11_INDEX_BUFFER_H
#endif // ENV_GRAPHIC_API_D3D11
