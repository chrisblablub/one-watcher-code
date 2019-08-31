
#include <Graphic/Core/MeshContainer.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/Material.h>
#include <Graphic/Core/RenderDevice.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace env
{
    CMeshMgr* gMeshMgr = nullptr;

    //-----------------------------------------------------------------------------------
    CMeshContainer::~CMeshContainer()
    {
        Unload();
    }

    //-----------------------------------------------------------------------------------
    void CMeshContainer::Unload()
    {
        for (mapSubMeshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
        {
            ENV_DELETE(it->second);
        }

        m_meshes.clear();

        m_bChanged = true;
    }

    //-----------------------------------------------------------------------------------
    bool CMeshContainer::InitResource(const std::string& strFilename, const std::string& strName, _FILETIME* pFt)
    {
        m_strFilename = strFilename;
        m_strName = strName;

        bool bResult = LoadResource();

        m_bChanged = true;

        if (!bResult)
            return false;

        if (pFt)
            GetFileLastWriteTime(m_strFilename, pFt);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CMeshContainer::LoadResource()
    {
        const aiScene* pScene = aiImportFile(m_strFilename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

        if (pScene == nullptr)
            return true;

        Unload();

        Matrix4x4 mtxParentTransform(1.0f);

        if (!LoadMeshesRecursive(pScene, pScene->mRootNode, mtxParentTransform))
            return false;

        Vec3 vMin = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        Vec3 vMax = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (mapSubMeshes::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
        {
            CBoundingVolume* pBoundingBox = it->second->GetBoundingVolume();

            Vec3 vMinTmp, vMaxTmp;
            pBoundingBox->GetMinMax(vMinTmp, vMaxTmp);

            if (vMinTmp.x < vMin.x)
                vMin.x = vMinTmp.x;
            if (vMinTmp.y < vMin.y)
                vMin.y = vMinTmp.y;
            if (vMinTmp.z < vMin.z)
                vMin.z = vMinTmp.z;

            if (vMaxTmp.x > vMax.x)
                vMax.x = vMaxTmp.x;
            if (vMaxTmp.y > vMax.y)
                vMax.y = vMaxTmp.y;
            if (vMaxTmp.z > vMax.z)
                vMax.z = vMaxTmp.z;
        }

        m_boundingVolume.ComputeBoundingVolume(vMin, vMax);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CMeshContainer::LoadMeshesRecursive(const aiScene* pScene, aiNode* pNode, const Matrix4x4& mtxParentTransform)
    {
        Matrix4x4 mtxTransform;
        mtxTransform[0][0] = pNode->mTransformation.a1;
        mtxTransform[0][1] = pNode->mTransformation.a2;
        mtxTransform[0][2] = pNode->mTransformation.a3;
        mtxTransform[0][3] = pNode->mTransformation.a4;
        mtxTransform[1][0] = pNode->mTransformation.b1;
        mtxTransform[1][1] = pNode->mTransformation.b2;
        mtxTransform[1][2] = pNode->mTransformation.b3;
        mtxTransform[1][3] = pNode->mTransformation.b4;
        mtxTransform[2][0] = pNode->mTransformation.c1;
        mtxTransform[2][1] = pNode->mTransformation.c2;
        mtxTransform[2][2] = pNode->mTransformation.c3;
        mtxTransform[2][3] = pNode->mTransformation.c4;
        mtxTransform[3][0] = pNode->mTransformation.d1;
        mtxTransform[3][1] = pNode->mTransformation.d2;
        mtxTransform[3][2] = pNode->mTransformation.d3;
        mtxTransform[3][3] = pNode->mTransformation.d4;

        mtxTransform = mtxTransform * mtxParentTransform;

        for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
        {
            unsigned int meshIndex = pNode->mMeshes[i];

            aiMesh* pMesh = pScene->mMeshes[meshIndex];



            std::vector< PosNormalMapTexVertex > vertices;
            vertices.reserve(pMesh->mNumVertices);

            std::vector< unsigned short > indices;
            indices.reserve(pMesh->mNumFaces * 3);

            for (unsigned int vertex = 0; vertex < pMesh->mNumVertices; ++vertex)
            {
                PosNormalMapTexVertex v;

                aiVector3D vertexPos(pMesh->mVertices[vertex]);

                v.pos = Vec3(vertexPos.x, vertexPos.y, vertexPos.z);

                if (pMesh->HasNormals())
                {
                    aiVector3D vertexNormal(pMesh->mNormals[vertex]);

                    v.normal = Vec3(vertexNormal.x, vertexNormal.y, vertexNormal.z);
                }

                if (pMesh->HasTangentsAndBitangents())
                {
                    aiVector3D vertexTangent(pMesh->mTangents[vertex]);
                    aiVector3D vertexBinormal(pMesh->mBitangents[vertex]);

                    v.tangent = Vec3(vertexTangent.x, vertexTangent.y, vertexTangent.z);
                    v.binormal = Vec3(vertexBinormal.x, vertexBinormal.y, vertexBinormal.z);
                }

                if (pMesh->HasTextureCoords(0))
                {
                    aiVector3D vertexTexCoords(pMesh->mTextureCoords[0][vertex]);

                    v.tex0 = Vec2(vertexTexCoords.x, vertexTexCoords.y);
                }


                vertices.push_back(v);
            }

            for (unsigned int face = 0; face < pMesh->mNumFaces; ++face)
            {
                aiFace f = pMesh->mFaces[face];

                assert(f.mNumIndices == 3);

                indices.push_back(f.mIndices[0]);
                indices.push_back(f.mIndices[1]);
                indices.push_back(f.mIndices[2]);
            }

            CVertexBuffer* pVB = gRenderSystem->GetRenderDevice()->CreateVertexBuffer(pMesh->mNumVertices * sizeof(PosNormalMapTexVertex), USAGE_TYPE_GPUREADACCESS, &vertices[0]);
            CIndexBuffer* pIB = gRenderSystem->GetRenderDevice()->CreateIndexBuffer(pMesh->mNumFaces * 3 * sizeof(unsigned short), USAGE_TYPE_GPUREADACCESS, TEXTUREFORMAT_TYPE_INDEX16, &indices[0]);

//             PosNormalMapTexVertex* pVertices = static_cast<PosNormalMapTexVertex*>(pVB->Lock(0, 0, 0));
//             for (unsigned int vertex = 0; vertex < vertices.size(); ++vertex)
//             {
//                 *pVertices = vertices[vertex];
//                 ++pVertices;
//             }
//             pVB->Unlock();
// 
//             unsigned short* pIndices = static_cast<unsigned short*>(pIB->Lock(0, 0, 0));
//             for (unsigned int index = 0; index < indices.size(); ++index)
//             {
//                 *pIndices = indices[index];
//                 ++pIndices;
//             }
//             pIB->Unlock();


            aiMaterial* pMat = pScene->mMaterials[pMesh->mMaterialIndex];

            aiString strTextureFilename;
            pMat->GetTexture(aiTextureType_DIFFUSE, 0, &strTextureFilename);

            CMaterial material;

            std::string strTex(strTextureFilename.C_Str());
            if (!strTex.empty())
            {
                material.LoadTexture(strTex, "LinearWrap", true);

                std::string strNormalMapName = CTextureManaged::GetNormalMapFilename(strTex);
                std::string strSpecularMapName = CTextureManaged::GetSpecularMapFilename(strTex);
                std::string strEmissiveMapName = CTextureManaged::GetEmissiveMapFilename(strTex);

                if (!strNormalMapName.empty())
                    material.LoadNormalMap(strNormalMapName, "LinearMipWrap", false);

                if (!strSpecularMapName.empty())
                    material.LoadSpecularMap(strSpecularMapName, "LinearMipWrap", true);

                if (!strEmissiveMapName.empty())
                    material.LoadEmissiveMap(strEmissiveMapName, "LinearMipWrap", true);
            }

            aiColor4D diff;
            aiColor3D specular;
            float shininess;
            pMat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
            pMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
            pMat->Get(AI_MATKEY_SHININESS, shininess);

            material.SetDiffuseColor(Vec3(diff.r, diff.g, diff.b));
            material.SetSpecularColor(Vec3(specular.r, specular.g, specular.b));
            material.SetShininess(shininess);
            material.SetOpacity(diff.a);
            
            Vec3 bvMin, bvMax;
            ComputeBoundingVolume(vertices, bvMin, bvMax);

            CBoundingVolume meshBoundingVolume;
            meshBoundingVolume.ComputeBoundingVolume(bvMin, bvMax);

            CMesh* pMeshInternal = new CMesh(pVB, pIB, material, mtxTransform, meshBoundingVolume, pMesh->mNumFaces, pMesh->mNumVertices);

            std::string strMeshName;
            mapSubMeshes::iterator it;
            unsigned int k = 0;

            do
            {
                std::stringstream strStrMeshName;
                strStrMeshName << std::string(pNode->mName.C_Str());

                if (k > 0)
                {
                    strStrMeshName << "##";
                    strStrMeshName << k;
                }

                strMeshName = strStrMeshName.str();

                it = m_meshes.find(strMeshName);

                ++k;

            } while (it != m_meshes.end());

            m_meshes[strMeshName] = pMeshInternal;
        }

        for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
        {
            LoadMeshesRecursive(pScene, pNode->mChildren[i], mtxTransform);
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CMeshContainer::ComputeBoundingVolume(const std::vector< PosNormalMapTexVertex >& vertices, Vec3& minPos, Vec3& maxPos)
    {
        minPos = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        maxPos = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (unsigned int i = 0; i < vertices.size(); ++i)
        {
            if (vertices[i].pos.x < minPos.x)
                minPos.x = vertices[i].pos.x;
            else if (vertices[i].pos.x > maxPos.x)
                maxPos.x = vertices[i].pos.x;

            if (vertices[i].pos.y < minPos.y)
                minPos.y = vertices[i].pos.y;
            else if (vertices[i].pos.y > maxPos.y)
                maxPos.y = vertices[i].pos.y;

            if (vertices[i].pos.z < minPos.z)
                minPos.z = vertices[i].pos.z;
            else if (vertices[i].pos.z > maxPos.z)
                maxPos.z = vertices[i].pos.z;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    CMesh* CMeshContainer::GetMesh(const std::string& strMesh)
    {
        if (m_meshes.empty())
            return NULL;
        else if (strMesh.empty())
            return m_meshes.begin()->second;
        else
        {
            mapSubMeshes::iterator it = m_meshes.find(strMesh);
            if (it == m_meshes.end())
                return NULL;
            else
                return it->second;
        }
    }

} // env