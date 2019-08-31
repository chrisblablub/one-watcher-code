/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifdef D3D9

#pragma once
#ifndef D3D9_VERTEX_BUFFER_H
#define D3D9_VERTEX_BUFFER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/D3D9/D3D9Config.h>

namespace env
{
	class CD3D9VertexBuffer
	{
	public:

		CD3D9VertexBuffer(IDirect3DDevice9* pDevice,
			IDirect3DVertexBuffer9* pVB);

		virtual ~CD3D9VertexBuffer();

		virtual void* Lock(unsigned int offset, unsigned int size, unsigned long flags);

		virtual void Unlock();

		virtual void Bind(unsigned int streamNumber, unsigned int offset, unsigned int stride);

	protected:
		IDirect3DVertexBuffer9* m_pVertexBuffer;
		IDirect3DDevice9* m_pDevice;

		unsigned int m_numVertices;
	};

	typedef CD3D9VertexBuffer CVertexBuffer;
} // env

#endif // D3D9_VERTEX_BUFFER_H
#endif // D3D9