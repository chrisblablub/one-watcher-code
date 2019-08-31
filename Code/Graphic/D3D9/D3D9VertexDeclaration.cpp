#ifdef D3D9

#include <Graphic/D3D9/D3D9VertexDeclaration.h>

namespace env
{
    CD3D9VertexDeclaration::CD3D9VertexDeclaration(IDirect3DDevice9* pDevice,
                                                   vecVertexDeclarationElements& vertexDeclarationElements,
                                                   unsigned int stride)
                                                   : m_pDevice(pDevice)
                                                   , m_vertexDeclarationElements(vertexDeclarationElements)
                                                   , m_stride(stride)
    {
        D3DVERTEXELEMENT9* vertexElements = new D3DVERTEXELEMENT9[vertexDeclarationElements.size() + 1];

        //m_stride = 0;
        unsigned int offset = 0;

        for (unsigned int i = 0; i < vertexDeclarationElements.size(); ++i)
        {
            VertexDeclarationElement element = vertexDeclarationElements[i];

            BYTE type = ConvertToD3DType(element.m_type);
            BYTE usage = ConvertToD3DUsage(element.m_usage);
            BYTE usageIndex = element.m_usageIndex;

            vertexElements[i].Offset = offset;
            vertexElements[i].Stream = 0;
            vertexElements[i].Type = type;
            vertexElements[i].Method = D3DDECLMETHOD_DEFAULT;
            vertexElements[i].Usage = usage;
            vertexElements[i].UsageIndex = usageIndex;

            offset += ConvertToD3DSize(element.m_type);
        }

        vertexElements[vertexDeclarationElements.size()].Offset = 0;
        vertexElements[vertexDeclarationElements.size()].Stream = 0xFF;
        vertexElements[vertexDeclarationElements.size()].Type = D3DDECLTYPE_UNUSED;
        vertexElements[vertexDeclarationElements.size()].Method = 0;
        vertexElements[vertexDeclarationElements.size()].Usage = 0;
        vertexElements[vertexDeclarationElements.size()].UsageIndex = 0;

        m_pDevice->CreateVertexDeclaration(vertexElements, &m_pDecl);

        ENV_DELETE_ARRAY(vertexElements);

    }

    CD3D9VertexDeclaration::~CD3D9VertexDeclaration()
    {
        ENV_RELEASE(m_pDecl);
    }

    void CD3D9VertexDeclaration::Bind()
    {
        m_pDevice->SetVertexDeclaration(m_pDecl);
    }

    unsigned int CD3D9VertexDeclaration::GetStride()
    {
        if (m_stride == 0)
        {
            bool bBreak = true;
        }
        assert(m_stride > 0);
        return m_stride;
    }

    void CD3D9VertexDeclaration::GetDeclaration(D3DVERTEXELEMENT9* pElements, unsigned int& numElements)
    {
        numElements = m_vertexDeclarationElements.size() + 1;

        int offset = 0;
        for (unsigned int i = 0; i < m_vertexDeclarationElements.size(); ++i)
        {
            VertexDeclarationElement element = m_vertexDeclarationElements[i];
            BYTE type = ConvertToD3DType(element.m_type);
            BYTE usage = ConvertToD3DUsage(element.m_usage);
            BYTE usageIndex = element.m_usageIndex;

            pElements[i].Offset = m_stride;
            pElements[i].Stream = 0;
            pElements[i].Type = type;
            pElements[i].Method = D3DDECLMETHOD_DEFAULT;
            pElements[i].Usage = usage;
            pElements[i].UsageIndex = usageIndex;


            D3DVERTEXELEMENT9 ve = pElements[i];

            offset += ConvertToD3DSize(element.m_type);
        }

        pElements[numElements - 1].Offset = 0;
        pElements[numElements - 1].Stream = 0xFF;
        pElements[numElements - 1].Type = D3DDECLTYPE_UNUSED;
        pElements[numElements - 1].Method = 0;
        pElements[numElements - 1].Usage = 0;
        pElements[numElements - 1].UsageIndex = 0;
    }


    BYTE CD3D9VertexDeclaration::ConvertToD3DType(VERTEX_DECLARATION_ELEMENTS_TYPE type)
    {
        BYTE d3dType = 0;

        switch (type)
        {
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1: d3dType = D3DDECLTYPE_FLOAT1; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2: d3dType = D3DDECLTYPE_FLOAT2; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3: d3dType = D3DDECLTYPE_FLOAT3; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4: d3dType = D3DDECLTYPE_FLOAT4; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_ULONG: d3dType = D3DDECLTYPE_D3DCOLOR; break;
        default: assert(false); break;
        }

        return d3dType;
    }

    BYTE CD3D9VertexDeclaration::ConvertToD3DUsage(VERTEX_DECLARATION_USAGE_TYPE usage)
    {
        BYTE d3dUsage = 0;

        switch (usage)
        {
        case VERTEX_DECLARATION_USAGE_TYPE_POSITION: d3dUsage = D3DDECLUSAGE_POSITION; break;
        case VERTEX_DECLARATION_USAGE_TYPE_BINORMAL: d3dUsage = D3DDECLUSAGE_BINORMAL; break;
        case VERTEX_DECLARATION_USAGE_TYPE_COLOR: d3dUsage = D3DDECLUSAGE_COLOR; break;
        case VERTEX_DECLARATION_USAGE_TYPE_NORMAL: d3dUsage = D3DDECLUSAGE_NORMAL; break;
        case VERTEX_DECLARATION_USAGE_TYPE_TANGENT: d3dUsage = D3DDECLUSAGE_TANGENT; break;
        case VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD: d3dUsage = D3DDECLUSAGE_TEXCOORD; break;
        default: assert(false); break;
        }

        return d3dUsage;
    }

    unsigned int CD3D9VertexDeclaration::ConvertToD3DSize(VERTEX_DECLARATION_ELEMENTS_TYPE type)
    {
        unsigned int size = 0;

        switch (type)
        {
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1: size += sizeof(float) * 1; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2: size += sizeof(float) * 2; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3: size += sizeof(float) * 3; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4: size += sizeof(float) * 4; break;
        case VERTEX_DECLARATION_ELEMENTS_TYPE_ULONG:  size += sizeof(unsigned long); break;
        default: assert(false); break;
        }

        return size;
    }
} // env

#endif // D3D9
