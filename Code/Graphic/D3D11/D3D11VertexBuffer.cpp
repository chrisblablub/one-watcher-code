#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11VertexBuffer.h>
#include <Graphic/D3D11/D3D11APITypeConversion.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CVertexBuffer::CVertexBuffer(ID3D11DeviceContext* pDeviceContext,
        ID3D11Buffer* pVertexBuffer)
        : m_pDeviceContext(pDeviceContext)
        , m_pVertexBuffer(pVertexBuffer)
        , m_numVertices(0)
        , m_pLockedData(nullptr) {}

    //-----------------------------------------------------------------------------------
    CVertexBuffer::~CVertexBuffer()
    {
        ENV_RELEASE(m_pVertexBuffer);
    }

    //-----------------------------------------------------------------------------------
    void* CVertexBuffer::Lock(unsigned int offset, unsigned int size, unsigned long flags)
    {
        if (m_pLockedData == nullptr)
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresources;

            m_pDeviceContext->Map(m_pVertexBuffer, 0, ConvertToD3D11Map(flags), 0, &mappedSubresources);
            m_pLockedData = mappedSubresources.pData;
        }

        return m_pLockedData;
    }

    //-----------------------------------------------------------------------------------
    void CVertexBuffer::Unlock()
    {
        if (m_pLockedData)
            m_pDeviceContext->Unmap(m_pVertexBuffer, 0);

        m_pLockedData = nullptr;
    }

    //-----------------------------------------------------------------------------------
    void CVertexBuffer::Bind(unsigned int streamNumber, unsigned int offset, unsigned int stride)
    {
        m_pDeviceContext->IASetVertexBuffers(streamNumber, 1, &m_pVertexBuffer, &stride, &offset);
    }
} // env

#endif // ENV_GRAPHIC_API_D3D11
