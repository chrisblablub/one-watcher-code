#include <Engine/Core/PolygonMousePicker.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Camera.h>
#include <Engine/Scene/Geometry.h>
#include <Graphic/RenderSystems/RenderSystem.h>


namespace env
{
    //-----------------------------------------------------------------------------------
    CPolygonMousePicker::CPolygonMousePicker()
    {

    }

    //-----------------------------------------------------------------------------------
    CPolygonMousePicker::~CPolygonMousePicker()
    {

    }

    //-----------------------------------------------------------------------------------
    void CPolygonMousePicker::AddPoint(const Vec3& point)
    {
        m_points.push_back(point);
    }

    //-----------------------------------------------------------------------------------
    bool CPolygonMousePicker::CheckPicked(const Matrix4x4& mtxTransform, CCamera* pCamera, float* pDistance)
    {
        std::vector< Vec3 > transformedPoints;
        transformedPoints.resize(m_points.size());

        for (int i = 0; i < m_points.size(); ++i)
        {
            transformedPoints[i] = Vec3(Vec4(m_points[i], 1.0f) * mtxTransform);
        }

        Matrix4x4 mtxViewProj = pCamera->GetViewProjectionMatrix();

        CPolygon polyA, polyB;
        polyA.SetNumPoints(transformedPoints.size());

        float width = (float)gRenderSystem->GetRenderQualityParams().screenWidth;
        float height = (float)gRenderSystem->GetRenderQualityParams().screenHeight;
        float znear = gRenderSystem->GetRenderQualityParams().zNear;
        float zfar = gRenderSystem->GetRenderQualityParams().zFar;
        for (int i = 0; i < transformedPoints.size(); ++i)
        {
            Vec4 ndc = Vec4(transformedPoints[i], 1.0f) * mtxViewProj;
            transformedPoints[i] = Vec3(ndc);
            transformedPoints[i] /= ndc.w;
            transformedPoints[i] += Vec3(1.0, 1.0, 1.0);
            transformedPoints[i] *= 0.5;
            transformedPoints[i].x *= width;
            transformedPoints[i].y *= height;
            transformedPoints[i].z = znear + transformedPoints[i].z * (zfar - znear);
            polyA.AddPoint(transformedPoints[i]);
        }


        Vec3 vMouse((float)gEngine->GetInput()->GetMouseX(), height - (float)gEngine->GetInput()->GetMouseY(), 0.0f);
        polyB.SetNumPoints(1);
        polyB.AddPoint(vMouse);
        if (pDistance)
            *pDistance = transformedPoints[0].z;
        return polyA.ComputeIntersection(polyB);
    }
} // env