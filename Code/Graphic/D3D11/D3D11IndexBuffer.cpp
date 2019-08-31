#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11IndexBuffer.h>
#include <Graphic/D3D11/D3D11APITypeConversion.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CIndexBuffer::CIndexBuffer(ID3D11DeviceContext* pDeviceContext,
                               ID3D11Buffer* pIndexBuffer, TEXTUREFORMAT_TYPE format)
                               : m_pDeviceContext(pDeviceContext)
                               , m_pIndexBuffer(pIndexBuffer)
                               , m_format(format) {}

    //-----------------------------------------------------------------------------------
    CIndexBuffer::~CIndexBuffer()
    {
        ENV_RELEASE(m_pIndexBuffer);
    }

    //-----------------------------------------------------------------------------------
    void CIndexBuffer::Bind()
    {
        m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, GetFormat() == TEXTUREFORMAT_TYPE_INDEX16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
    }

    //-----------------------------------------------------------------------------------
    void* CIndexBuffer::Lock(unsigned int offset, unsigned int size, unsigned long flags)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresources;

        m_pDeviceContext->Map(m_pIndexBuffer, 0, ConvertToD3D11Map(flags), 0, &mappedSubresources);

        return mappedSubresources.pData;
    }

    //-----------------------------------------------------------------------------------
    void CIndexBuffer::Unlock()
    {
        m_pDeviceContext->Unmap(m_pIndexBuffer, 0);
    }



} // env
#endif // ENV_GRAPHIC_API_D3D11
