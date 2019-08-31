
#include <Graphic/Core/Mesh.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    CMesh::CMesh(CVertexBuffer* pVB, CIndexBuffer* pIB, const CMaterial& pMaterial,
                 const Matrix4x4& mtxTransform, CBoundingVolume& boundingVolume,
                 unsigned int numFaces, unsigned int numVertices)
        : m_pMaterial(pMaterial)
    {
        m_boundingVolume = boundingVolume;

        m_meshRenderObject.m_pVertexBuffer = pVB;
        m_meshRenderObject.m_pIndexBuffer = pIB;
        m_meshRenderObject.m_numFaces = numFaces;
        m_meshRenderObject.m_numVertices = numVertices;

        SetModelTransform(mtxTransform);
    }

    //-----------------------------------------------------------------------------------
    CMesh::~CMesh()
    {
        ENV_DELETE(m_meshRenderObject.m_pVertexBuffer);
        ENV_DELETE(m_meshRenderObject.m_pIndexBuffer);
    }

    //-----------------------------------------------------------------------------------
    void CMesh::SetModelTransform(const Matrix4x4& mtxModel)
    {
        m_meshRenderObject.m_mtxTransform = mtxModel;
        m_boundingVolume.Transform(m_meshRenderObject.m_mtxTransform);
    }
} // env