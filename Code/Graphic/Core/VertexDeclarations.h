/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef VERTEX_DECLARATIONS_H
#define VERTEX_DECLARATIONS_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/VertexDeclaration.h>

namespace env
{
    class PosColorVertex
    {
    public:
        PosColorVertex() {}

        PosColorVertex(float x, float y, float z, float w, 
                 float r, float g, float b, float a)
            : p(x, y, z, w)
            , color(r, g, b, a) {}

        Vec4 p;
        Vec4 color;

        static vecVertexDeclarationElements VertexElements;
    };

    class PosTexVertex
    {
    public:
        PosTexVertex() {}

        PosTexVertex(float x, float y, float z, 
            float u, float v)
            : p(x,y,z)
            , t(u,v) {}

        Vec3 p;
        Vec2 t;

        static vecVertexDeclarationElements VertexElements;
    };

    class PosColorTexVertex
    {
    public:
        PosColorTexVertex() {}

        PosColorTexVertex(float x, float y, float z,
            float u, float v)
            : p(x, y, z)
            , t(u, v) {}

        Vec3 p;
        Vec3 c;
        Vec2 t;

        static vecVertexDeclarationElements VertexElements;
    };


    struct TerrainVertex
    {
        TerrainVertex()
            : pos(0.0f, 0.0f, 0.0f)
            , texcoords(0.0f, 0.0f)
            , colorLighting(0.0f, 0.0f, 0.0f, 1.0f)
            , colorEmissive(1.0f, 1.0f, 1.0f, 1.0f) {}

        TerrainVertex(float x, float y, float z,
            float u, float v, float a)
            : pos(x,y,z)
            , texcoords(u, v)
            , colorLighting(0.0f, 0.0f, 0.0f, 1.0f) 
            , colorEmissive(1.0f, 1.0f, 1.0f, 1.0f) {}
        
        Vec4 color;
        Vec4 colorLighting;
        Vec4 colorEmissive;
        Vec3 pos;
        Vec3 tangent;
        Vec3 binormal;
        Vec2 texcoords;

        static vecVertexDeclarationElements VertexElements;
    };

    struct TerrainPainterVertex
    {
        TerrainPainterVertex()
            : pos(0.0f, 0.0f, 0.0f)
            , texcoords(0.0f, 0.0f)
            , alpha(0.0f) {}

        TerrainPainterVertex(float x, float y, float z,
                    float u, float v, float a)
                    : pos(x, y, z)
                    , texcoords(u, v)
                    , alpha(0.0f) {}

        Vec3 pos;
        Vec3 tangent;
        Vec3 binormal;
        Vec2 texcoords;

        unsigned long color;
        float alpha;

        static vecVertexDeclarationElements VertexElements;
    };

    struct TerrainBorderVertex
    {
        TerrainBorderVertex()
            : pos(0.0f, 0.0f, 0.0f, 0.0f)
            , texcoords(0.0f, 0.0f)
            , color(1.0f, 1.0f, 1.0f, 0.0f)
            , colorLighting(0.0f, 0.0f, 0.0f, 1.0f) {}

        Vec4 pos;
        Vec4 color;
        Vec4 colorLighting;
        Vec2 texcoords;

        static vecVertexDeclarationElements VertexElements;
    };

    struct PolygonVertex
    {
        PolygonVertex(const Vec3& pos, const Vec3& color)
            : pos(pos)
            , color(color) {}

        PolygonVertex()
            : pos(0.0f, 0.0f, 0.0f)
            , color(0.0f, 0.0f, 0.0f) {}

        Vec3 pos;
        Vec3 color;

        static vecVertexDeclarationElements VertexElements;
    };


    struct PosNormalMapTexVertex
    {
        PosNormalMapTexVertex()
            : pos(0.0f, 0.0f, 0.0f)
            , tangent(0.0f, 0.0f, 0.0f)
            , binormal(0.0f, 0.0f, 0.0f)
            , normal(0.0f, 0.0f, 0.0f)
            , tex0(0.0f, 0.0f) {}

        Vec3 pos;
        Vec3 tangent;
        Vec3 binormal;
        Vec3 normal;
        Vec2 tex0;

        static vecVertexDeclarationElements VertexElements;
    };



    struct CParticleColorGPU
    {
        CParticleColorGPU()
            : position(0.0f, 0.0f, 0.0f, 1.0f)
            , color(0.0f, 0.0f, 0.0f, 0.0f)
            , texCoords(0.0f, 0.0f)
            , blending(1.0f) {}

        Vec4 color;
        Vec4 position;
        Vec2 texCoords;
        float blending;
        float pad;
        static vecVertexDeclarationElements VertexElements;
    };

    // IMPORTANT: ALIGNMENT
    // The structs will be aligned to the largest single element. This means that
    // the vertex declaration can be wrong 
    struct CParticleDistortGPU : CParticleColorGPU
    {
        CParticleDistortGPU()
            : tangent(0.0f, 0.0f, 0.0f, 0.0f)
            , binormal(0.0f, 0.0f, 0.0f, 0.0f)
            , distortion(0.0f) {}


        Vec4 tangent;
        Vec4 binormal;
        float distortion;
        float pad;


        static vecVertexDeclarationElements VertexElements;
    };

    class CRenderDevice;

    void InitVertexDeclarations(CRenderDevice* pDevice);
    void ReleaseVertexDeclarations();

} // env

#endif // VERTEX_DECLARATIONS_H 



