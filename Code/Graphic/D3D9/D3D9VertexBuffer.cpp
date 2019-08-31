#ifdef D3D9

#include <Graphic/D3D9/D3D9VertexBuffer.h>
#include <Graphic/D3D9/D3D9APITypeConversion.h>

namespace env
{
	CD3D9VertexBuffer::CD3D9VertexBuffer(IDirect3DDevice9* pDevice,
		IDirect3DVertexBuffer9* pVB)
		: m_pDevice(pDevice),
		m_pVertexBuffer(pVB),
		m_numVertices(0) {}

	CD3D9VertexBuffer::~CD3D9VertexBuffer()
	{
		ENV_RELEASE(m_pVertexBuffer);
	}

	void* CD3D9VertexBuffer::Lock(unsigned int offset, unsigned int size, unsigned long flags)
	{
		void* pData;
		m_pVertexBuffer->Lock(offset, size, &pData, ConvertToD3D9Lock(flags));

		return pData;
	}

	void CD3D9VertexBuffer::Unlock()
	{
		m_pVertexBuffer->Unlock();
	}

	void CD3D9VertexBuffer::Bind(unsigned int streamNumber, unsigned int offset, unsigned int stride)
	{
		assert(stride > 0 && stride < 1000);
		m_pDevice->SetStreamSource(streamNumber, m_pVertexBuffer, offset, stride);
	}


} // env

#endif // D3D9