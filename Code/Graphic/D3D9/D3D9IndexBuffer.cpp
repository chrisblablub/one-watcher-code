#ifdef D3D9

#include <Graphic/D3D9/D3D9IndexBuffer.h>
#include <Graphic/D3D9/D3D9APITypeConversion.h>

namespace env
{
    CD3D9IndexBuffer::CD3D9IndexBuffer(IDirect3DDevice9* pDevice,
                                       IDirect3DIndexBuffer9* pIB)
                                       : m_pDevice(pDevice),
                                       m_pIndexBuffer(pIB)
    {
    }

    CD3D9IndexBuffer::~CD3D9IndexBuffer()
    {
        ENV_RELEASE(m_pIndexBuffer);
    }

    void CD3D9IndexBuffer::Bind()
    {
        m_pDevice->SetIndices(m_pIndexBuffer);
    }

    void* CD3D9IndexBuffer::Lock(unsigned int offset, unsigned int size, unsigned long flags)
    {
        void* pData;
        m_pIndexBuffer->Lock(offset, size, &pData, ConvertToD3D9Lock(flags));

        return pData;
    }

    void CD3D9IndexBuffer::Unlock()
    {
        m_pIndexBuffer->Unlock();
    }



} // env
#endif // D3D9