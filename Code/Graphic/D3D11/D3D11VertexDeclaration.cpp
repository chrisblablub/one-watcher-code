#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11VertexDeclaration.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CVertexDeclaration::CVertexDeclaration(ID3D11Device* pDevice,
                                           ID3D11DeviceContext* pDeviceContext,
                                           ID3DBlob* pShaderSignature,
                                           vecVertexDeclarationElements& vertexDeclarationElements,
                                           unsigned int stride)
                                           : m_pDevice(pDevice)
                                           , m_pDeviceContext(pDeviceContext)
                                           , m_stride(stride)
                                           , m_vertexDeclarationElements(vertexDeclarationElements)
    {
        m_d3d11VertexDeclaration.resize(vertexDeclarationElements.size());

        for (unsigned int i = 0; i < vertexDeclarationElements.size(); ++i)
        {
            const VertexDeclarationElement& element = vertexDeclarationElements[i];

            DXGI_FORMAT d3d11Format = ConvertToD3DFormat(element.m_type);
            std::string d3d11Semantic = ConvertToD3DSemantic(element.m_usage);

            m_d3d11VertexDeclaration[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
            m_d3d11VertexDeclaration[i].Format = d3d11Format;
            m_d3d11VertexDeclaration[i].InputSlot = 0;
            m_d3d11VertexDeclaration[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            m_d3d11VertexDeclaration[i].InstanceDataStepRate = 0;
            m_d3d11VertexDeclaration[i].SemanticIndex = element.m_usageIndex;
            m_d3d11VertexDeclaration[i].SemanticName = new char[d3d11Semantic.length() + 1];
            memcpy((char*)m_d3d11VertexDeclaration[i].SemanticName, d3d11Semantic.c_str(), d3d11Semantic.length() + 1);

            //m_stride += ConvertToD3DSize(element.m_type);
        }

        ENV_DXCALL_DBG(m_pDevice->CreateInputLayout(&m_d3d11VertexDeclaration[0],
            vertexDeclarationElements.size(),
            pShaderSignature->GetBufferPointer(),
            pShaderSignature->GetBufferSize(),
            &m_pInputLayout));


        for (unsigned int i = 0; i < m_d3d11VertexDeclaration.size(); ++i)
            ENV_DELETE_ARRAY(m_d3d11VertexDeclaration[i].SemanticName);

    }

    //-----------------------------------------------------------------------------------
    CVertexDeclaration::~CVertexDeclaration()
    {
        ENV_RELEASE(m_pInputLayout);
    }

    //-----------------------------------------------------------------------------------
    void CVertexDeclaration::Bind()
    {
        m_pDeviceContext->IASetInputLayout(m_pInputLayout);
    }

    //-----------------------------------------------------------------------------------
    DXGI_FORMAT CVertexDeclaration::ConvertToD3DFormat(VERTEX_DECLARATION_ELEMENTS_TYPE type)
    {
        DXGI_FORMAT d3dFormat;

        switch (type)
        {
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1: d3dFormat = DXGI_FORMAT_R32_FLOAT;            break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2: d3dFormat = DXGI_FORMAT_R32G32_FLOAT;         break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3: d3dFormat = DXGI_FORMAT_R32G32B32_FLOAT;      break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4: d3dFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;   break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_ULONG:  d3dFormat = DXGI_FORMAT_R8G8B8A8_UNORM;       break;
        default: assert(false); break;
        }

        return d3dFormat;
    }

    //-----------------------------------------------------------------------------------
    std::string CVertexDeclaration::ConvertToD3DSemantic(VERTEX_DECLARATION_USAGE_TYPE usage)
    {
        std::string d3dSemanticName;

        switch (usage)
        {
        case VERTEX_DECLARATION_USAGE_TYPE_POSITION:    d3dSemanticName = "POSITION";   break;
        case VERTEX_DECLARATION_USAGE_TYPE_BINORMAL:    d3dSemanticName = "BINORMAL";   break;
        case VERTEX_DECLARATION_USAGE_TYPE_COLOR:       d3dSemanticName = "COLOR";      break;
        case VERTEX_DECLARATION_USAGE_TYPE_NORMAL:      d3dSemanticName = "NORMAL";     break;
        case VERTEX_DECLARATION_USAGE_TYPE_TANGENT:     d3dSemanticName = "TANGENT";    break;
        case VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD:    d3dSemanticName = "TEXCOORD";   break;
        default: assert(false); break;
        }

        return d3dSemanticName;
    }

    //-----------------------------------------------------------------------------------
    unsigned int CVertexDeclaration::ConvertToD3DSize(VERTEX_DECLARATION_ELEMENTS_TYPE type)
    {
        unsigned int size = 0;

        switch (type)
        {
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1: size += 4 * 1;        break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2: size += 4 * 2;        break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3: size += 4 * 3;        break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4: size += 4 * 4;        break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_ULONG:  size += 4;            break;
        default: assert(false); break;
        }

        return size;
    }
} // env

#endif // ENV_GRAPHIC_API_D3D11
