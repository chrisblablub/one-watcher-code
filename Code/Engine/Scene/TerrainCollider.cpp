#include <Engine/Scene/TerrainCollider.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CTerrainColliderCircle::CTerrainColliderCircle(const VECTOR2FIXED& position, const VECTOR2FIXED& move, int radius)
        : m_radius(radius)
    {
        SetPosition(position.x, position.y);
        SetMove(move.x, move.y);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainColliderCircle::SetPosition(int x, int y)
    {
        SetPositionX(x);
        SetPositionY(y);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainColliderCircle::SetPositionX(int x)
    {
        m_pos.x = x;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainColliderCircle::SetPositionY(int y)
    {
        m_pos.y = y;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainColliderCircle::SetMove(int x, int y)
    {
        SetMoveX(x);
        SetMoveY(y);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainColliderCircle::SetMoveX(int x)
    {
        m_move.x = x;

        if (x < 0.0)
            m_dirX = -1;
        else if (x > 0.0)
            m_dirX = 1;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainColliderCircle::SetMoveY(int y)
    {
        m_move.y = y;

        if (y < 0.0)
            m_dirY = -1;
        else if (y > 0.0)
            m_dirY = 1;
    }

    //-----------------------------------------------------------------------------------
    const VECTOR2FIXED& CTerrainColliderCircle::GetMove()
    {
        return m_move;
    }

    //-----------------------------------------------------------------------------------
    const VECTOR2FIXED& CTerrainColliderCircle::GetPosition()
    {
        return m_pos;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainColliderCircle::GetRadius()
    {
        return m_radius;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainColliderCircle::GetDirX()
    {
        return m_dirX;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainColliderCircle::GetDirY()
    {
        return m_dirY;
    }







    //-----------------------------------------------------------------------------------
    CTerrainCollider::CTerrainCollider(const VECTOR2FIXED& vPosition, int width, int height, const VECTOR2FIXED& vMove,
                                       const VECTOR2FIXED& vMoveOriginal, bool bOnFloor, bool bHitWallFromLeft,
                                       bool bHitWallFromRight, bool bHitCeiling, bool bOnRamp)
    {
        SetSize(width, height);
        SetPosition(vPosition.x, vPosition.y);

        m_dirX = 0;
        m_dirY = 0;

        SetMove(vMove.x, vMove.y);

        m_originalDirX = 0;
        if (vMoveOriginal.x < 0.0)
            m_originalDirX = -1;
        else if (vMoveOriginal.x > 0.0)
            m_originalDirX = 1;

        m_originalDirY = 0;
        if (vMoveOriginal.y < 0.0)
            m_originalDirY = -1;
        else if (vMoveOriginal.y > 0.0)
            m_originalDirY = 1;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetDirX()
    {
        return m_dirX;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetDirY()
    {
        return m_dirY;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetBaseDirX()
    {
        return m_originalDirX;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetBaseDirY()
    {
        return m_originalDirY;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetMove(int x, int y)
    {
        SetMoveX(x);
        SetMoveY(y);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetMoveX(int x)
    {
        m_move.x = x;
        m_newPos.x = m_pos.x + m_move.x;
        if (x < 0.0)
            m_dirX = -1;
        else if (x > 0.0)
            m_dirX = 1;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetMoveY(int y)
    {
        m_move.y = y;
        m_newPos.y = m_pos.y + m_move.y;

        if (y < 0.0)
            m_dirY = -1;
        else if (y > 0.0)
            m_dirY = 1;
    }

    //-----------------------------------------------------------------------------------
    const VECTOR2FIXED& CTerrainCollider::GetMove()
    {
        return m_move;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetPosition(int x, int y)
    {
        SetPositionX(x);
        SetPositionY(y);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetPositionX(int x)
    {
        m_pos.x = x;
        m_box.cellXMin = (int)((m_pos.x - m_halfWidth) / CELL_SIZE_SCALED);
        m_box.cellXMax = (int)((m_pos.x + m_halfWidth) / CELL_SIZE_SCALED);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetPositionY(int y)
    {
        m_pos.y = y;
        m_box.cellYMin = (int)((m_pos.y - m_halfHeight) / CELL_SIZE_SCALED);
        m_box.cellYMax = (int)((m_pos.y + m_halfHeight) / CELL_SIZE_SCALED);
    }

    //-----------------------------------------------------------------------------------
    const VECTOR2FIXED& CTerrainCollider::GetPosition()
    {
        return m_pos;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetNewPositionX(int x)
    {
        SetMoveX(x - m_pos.x);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetNewPositionY(int y)
    {
        SetMoveY(y - m_pos.y);
    }

    //-----------------------------------------------------------------------------------
    const VECTOR2FIXED& CTerrainCollider::GetNewPosition()
    {
        return m_newPos;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::SetSize(int width, int height)
    {
        m_width = width;
        m_height = height;

        m_halfWidth = (int)(m_width * 0.5f);
        m_halfHeight = (int)(m_height * 0.5f);
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetWidth()
    {
        return m_width;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetHeight()
    {
        return m_height;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetHalfWidth()
    {
        return m_halfWidth;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetHalfHeight()
    {
        return m_halfHeight;
    }

    //-----------------------------------------------------------------------------------
    const ColliderBBox& CTerrainCollider::GetBBox()
    {
        return m_box;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::DoXTransition(bool bXMin)
    {
        if (bXMin)
            m_box.cellXMin -= 1;
        else
            m_box.cellXMax += 1;
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::DoYTransition(bool bYMin)
    {
        if (bYMin)
            m_box.cellYMin -= 1;
        else
            m_box.cellYMax += 1;
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetCellDistanceX(int x, bool bXMin)
    {
        if (bXMin)
            return (x - m_box.cellXMin * CELL_SIZE_SCALED);
        else
            return ((m_box.cellXMax + 1) * CELL_SIZE_SCALED - x);
    }

    //-----------------------------------------------------------------------------------
    int CTerrainCollider::GetCellDistanceY(int y, bool bYMin)
    {
        if (bYMin)
            return (y - m_box.cellYMin * CELL_SIZE_SCALED);
        else
            return ((m_box.cellYMax + 1) * CELL_SIZE_SCALED - y);
    }

    //-----------------------------------------------------------------------------------
    void CTerrainCollider::GetCorners(VECTOR2FIXED& v1, VECTOR2FIXED& v2, VECTOR2FIXED& v3, VECTOR2FIXED& v4)
    {
        v1 = VECTOR2FIXED(m_pos.x - m_halfWidth, m_pos.y + m_halfHeight);
        v2 = VECTOR2FIXED(m_pos.x - m_halfWidth, m_pos.y - m_halfHeight);
        v3 = VECTOR2FIXED(m_pos.x + m_halfWidth, m_pos.y + m_halfHeight);
        v4 = VECTOR2FIXED(m_pos.x + m_halfWidth, m_pos.y - m_halfHeight);
    }
} // env
