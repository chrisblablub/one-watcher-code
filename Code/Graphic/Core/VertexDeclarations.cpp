
#include <Graphic/Core/VertexDeclarations.h>


namespace env
{
    vecVertexDeclarationElements PosColorVertex::VertexElements;
    vecVertexDeclarationElements PosTexVertex::VertexElements;
    vecVertexDeclarationElements PosColorTexVertex::VertexElements;
    vecVertexDeclarationElements PosNormalMapTexVertex::VertexElements;
    vecVertexDeclarationElements CParticleColorGPU::VertexElements;
    vecVertexDeclarationElements CParticleDistortGPU::VertexElements;
    vecVertexDeclarationElements PolygonVertex::VertexElements;
    vecVertexDeclarationElements TerrainVertex::VertexElements;
    vecVertexDeclarationElements TerrainPainterVertex::VertexElements;
    vecVertexDeclarationElements TerrainBorderVertex::VertexElements;

    void InitVertexDeclarations(CRenderDevice* pDevice)
    {
        PosColorVertex::VertexElements.reserve(2);
        PosColorVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        PosColorVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_COLOR, 0));

        PosTexVertex::VertexElements.reserve(2);
        PosTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        PosTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));

        PosColorTexVertex::VertexElements.reserve(3);
        PosColorTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        PosColorTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 1));
        PosColorTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));

        PosNormalMapTexVertex::VertexElements.reserve(5);
        PosNormalMapTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        PosNormalMapTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_TANGENT, 0));
        PosNormalMapTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_BINORMAL, 0));
        PosNormalMapTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_NORMAL, 0));
        PosNormalMapTexVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));

        TerrainVertex::VertexElements.reserve(7);
        TerrainVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_COLOR, 0));
        TerrainVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_COLOR, 1));
        TerrainVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_COLOR, 2));
        TerrainVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        TerrainVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_TANGENT, 0));
        TerrainVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_BINORMAL, 0));
        TerrainVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));
        
        TerrainPainterVertex::VertexElements.reserve(6);
        TerrainPainterVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        TerrainPainterVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_TANGENT, 0));
        TerrainPainterVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_BINORMAL, 0));
        TerrainPainterVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));
        TerrainPainterVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_ULONG, VERTEX_DECLARATION_USAGE_TYPE_COLOR, 0));
        TerrainPainterVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 1));

        TerrainBorderVertex::VertexElements.reserve(4);
        TerrainBorderVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        TerrainBorderVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));
        TerrainBorderVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 1));
        TerrainBorderVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 2));
        

        PolygonVertex::VertexElements.reserve(2);
        PolygonVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        PolygonVertex::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT3, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));

        CParticleColorGPU::VertexElements.reserve(4);
        CParticleColorGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_COLOR, 0));
        CParticleColorGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_POSITION, 0));
        CParticleColorGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT2, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 0));
        CParticleColorGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 1));
        CParticleColorGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 5));

        CParticleDistortGPU::VertexElements.reserve(7);
        CParticleDistortGPU::VertexElements = CParticleColorGPU::VertexElements;
        CParticleDistortGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_TANGENT, 0));
        CParticleDistortGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT4, VERTEX_DECLARATION_USAGE_TYPE_BINORMAL, 0));
        CParticleDistortGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 2));
        CParticleDistortGPU::VertexElements.push_back(VertexDeclarationElement(VERTEX_DECLARATION_ELEMENTS_TYPE_FLOAT1, VERTEX_DECLARATION_USAGE_TYPE_TEXCOORD, 6));
    }

    void ReleaseVertexDeclarations()
    {
    }
} // env