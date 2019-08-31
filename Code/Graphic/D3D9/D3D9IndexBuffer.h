/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_INDEX_BUFFER_H
#define D3D9_INDEX_BUFFER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>

namespace env
{
	class CD3D9IndexBuffer
	{
	public:
		CD3D9IndexBuffer(IDirect3DDevice9* pDevice,
			IDirect3DIndexBuffer9* pIB);

		virtual ~CD3D9IndexBuffer();

		virtual void Bind();

		virtual void* Lock(unsigned int offset, unsigned int size, unsigned long flags);

		virtual void Unlock();

	protected:
		IDirect3DIndexBuffer9* m_pIndexBuffer;
		IDirect3DDevice9* m_pDevice;
	};

	typedef CD3D9IndexBuffer CIndexBuffer;
} // env

#endif // INDEX_BUFFER_H
#endif // D3D9