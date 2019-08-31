#include "../Math/MathUtility.h"

namespace env
{
    //-----------------------------------------------------------------------------------
    float GetRandomFloat(float a, float b)
    {
        if (a >= b) // bad input
            std::swap(a, b);

        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_real_distribution<> dist(a, b);

        return dist(eng);
    }

    //-----------------------------------------------------------------------------------
    Vec3& GetRandomPoint(Vec3& vOut, const Vec3& vMin, const Vec3& vMax)
    {
        vOut.x = GetRandomFloat(vMin.x, vMax.x);
        vOut.y = GetRandomFloat(vMin.y, vMax.y);
        vOut.z = GetRandomFloat(vMin.z, vMax.z);

        return vOut;
    }

    //-----------------------------------------------------------------------------------
    Vec3& GetRandomVec(Vec3& vOut)
    {
        GetRandomPoint(vOut, Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 1.0f));

        glm::normalize(vOut);

        return vOut;
    }

    //-----------------------------------------------------------------------------------
    Vec3& GetRandomVec(Vec3& vOut, const Vec3& vMin, const Vec3& vMax)
    {
        GetRandomPoint(vOut, vMin, vMax);

        glm::normalize(vOut);

        return vOut;
    }

    //-----------------------------------------------------------------------------------
    Vec4& GetRandomPoint(Vec4& vOut, const Vec4& vMin, const Vec4& vMax)
    {
        vOut.x = GetRandomFloat(vMin.x, vMax.x);
        vOut.y = GetRandomFloat(vMin.y, vMax.y);
        vOut.z = GetRandomFloat(vMin.z, vMax.z);
        vOut.w = GetRandomFloat(vMin.w, vMax.w);

        return vOut;
    }

    //-----------------------------------------------------------------------------------
    Vec4& GetRandomVec(Vec4& vOut)
    {
        GetRandomPoint(vOut, Vec4(-1.0f, -1.0f, -1.0f, -1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));

        glm::normalize(vOut);

        return vOut;
    }

    //-----------------------------------------------------------------------------------
    Vec4& GetRandomVec(Vec4& vOut, const Vec4& vMin, const Vec4& vMax)
    {
        GetRandomPoint(vOut, vMin, vMax);

        glm::normalize(vOut);

        return vOut;
    }

    //-----------------------------------------------------------------------------------
    bool Equals(float a, float b)
    {
        return fabsf(a - b) < 0.00001f/*std::numeric_limits<float>::epsilon()*/;
    }

    //-----------------------------------------------------------------------------------
    void ComputeViewingRayDirection(const Matrix4x4& mtxProjection, const Matrix4x4& mtxView, unsigned long backbufferWidth,
                                    unsigned long backbufferHeight, float mouseX, float mouseY, Vec3& vRayDirection)
    {
        Vec3 vMouseRayDir;
        Vec3 vOrigin(0.0f, 0.0f, 0.0f);
        Vec2 vMouseScreenPos;
        vMouseScreenPos.x = (float)mouseX;
        vMouseScreenPos.y = (float)mouseY;

        float ScreenWidth = (float)backbufferWidth;
        float ScreenHeight = (float)backbufferHeight;
        float x = (2.0f * vMouseScreenPos.x / ScreenWidth - 1.0f) / mtxProjection[0][0];
        float y = (-2.0f * vMouseScreenPos.y / ScreenHeight + 1.0f) / mtxProjection[1][1];
        vMouseRayDir.x = x;
        vMouseRayDir.y = y;
        vMouseRayDir.z = 1.0f;


        Matrix4x4 ViewInv = glm::inverse(mtxView);
        Vec3 vOriginW;
        Vec3 vMouseRayDirW;
        vOriginW = Vec3(Vec4(vOrigin, 1.0f) * ViewInv);
        vMouseRayDirW = glm::normalize(Vec3(Vec4(vMouseRayDir, 0.0f) * ViewInv));

        vRayDirection = vMouseRayDirW;
    }


    //-----------------------------------------------------------------------------------
    float ComputePointOnLineProjection(const Vec3& point, const Vec3& linePos, const Vec3& lineDir)
    {
        Vec3 v = point - linePos;
        float param = glm::dot(v, lineDir) / glm::dot(lineDir, lineDir);
        return param;
    }

    //-----------------------------------------------------------------------------------
    void ComputePointOnLineProjection(Vec3& projPos, const Vec3& point, const Vec3& linePos, const Vec3& lineEndPos)
    {
        Vec3 v = point - linePos;
        Vec3 lineDir = lineEndPos - linePos;
        float param = Clamp(glm::dot(v, lineDir) / glm::dot(lineDir, lineDir), 0.0f, 1.0f);
        projPos = linePos + param * lineDir;
    }

    //-----------------------------------------------------------------------------------
    void ComputeReflectedVector(Vec3& reflected, const Vec3& incidentRay, const Vec3& normal)
    {
        reflected = incidentRay - 2.0f * normal * glm::dot(incidentRay, normal);
    }

    //-----------------------------------------------------------------------------------
    void ComputeReflectedVector(Vec2& reflected, const Vec2& incidentRay, const Vec2& normal)
    {
        reflected = incidentRay - 2.0f * normal * glm::dot(incidentRay, normal);
    }

    //-----------------------------------------------------------------------------------
    float ComputeRoll(const Vec3& v, const Vec3& baseAxis)
    {
        float targetRoll = std::acos(glm::dot(v, baseAxis));

        if (v.x > 0.0f)
            targetRoll = -targetRoll;

        return targetRoll;
    }

    //-----------------------------------------------------------------------------------
    float ComputeRoll(const Vec2& v, const Vec2& baseAxis)
    {
        float targetRoll = std::acos(glm::dot(v, baseAxis));

        if (v.x > 0.0f)
            targetRoll = -targetRoll;

        return targetRoll;
    }

    //-----------------------------------------------------------------------------------
    bool ComputeRollRotationDirection(float* pRollDiff, float targetRoll, float initialRoll)
    {
        targetRoll = targetRoll + PI;
        initialRoll = initialRoll + PI;

        // assume targetRoll > currentRoll
        float largerRoll = targetRoll;
        float smallerRoll = initialRoll;

        bool bSwitched = false;
        if (largerRoll < smallerRoll)
        {
            std::swap(largerRoll, smallerRoll);
            bSwitched = true;
        }

        float diff1 = largerRoll - smallerRoll;
        float diff2 = smallerRoll + 2 * PI - largerRoll;

        bool bLeft;
        if (diff1 < diff2)
            bLeft = true;
        else
            bLeft = false;

        if (bLeft && pRollDiff)
            *pRollDiff = diff1;

        if (!bLeft && pRollDiff)
            *pRollDiff = diff2;

        if (bSwitched)
            bLeft = !bLeft;

        return bLeft;
    }


    //-----------------------------------------------------------------------------------
    float sgn(float x)
    {
        if (x < 0)
            return -1.0;
        else
            return 1.0;
    }

    //-----------------------------------------------------------------------------------
    float RadToDeg(float rad)
    {
        return rad / (2 * PI) * 360.0f;
    }

    //-----------------------------------------------------------------------------------
    float DegToRad(float deg)
    {
        return deg / 360.0f * 2 * PI;
    }

    //-----------------------------------------------------------------------------------
    void ComputeProjectionMatrix(Matrix4x4& mtxProjection, float l, float r, float t, float b, float n, float f)
    {
        float xScale = 2.0f * n / (r - l);
        float yScale = 2.0f * n / (t - b);

        mtxProjection[0] = Vec4(xScale, 0, -(r + l) / (l - r), 0);
        mtxProjection[1] = Vec4(0, yScale, -(t + b) / (b - t), 0);
        mtxProjection[2] = Vec4(0, 0, f / (f - n), -n * f / (f - n));
        mtxProjection[3] = Vec4(0, 0, 1, 0);
    }

    //-----------------------------------------------------------------------------------
    void ComputeProjectionMatrixOrtho(Matrix4x4& mtxProjection, float l, float r, float t, float b, float n, float f)
    {
        float xScale = 2.0f / (r - l);
        float yScale = 2.0f / (t - b);

        mtxProjection[0] = Vec4(xScale, 0, 0, -0.5f * (r + l) / (r - l));
        mtxProjection[1] = Vec4(0, yScale, 0, -0.5f * (t + b) / (t - b));
        mtxProjection[2] = Vec4(0, 0, 2.0f / (f - n), 0.5f * (f + n) / (f - n));
        mtxProjection[3] = Vec4(0, 0, 0, 1);
    }

} // env