#include <Graphic/Core/RenderTarget.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CBaseRenderTarget::CBaseRenderTarget(int width, int height, TEXTUREFORMAT_TYPE format, bool bMipMaps)
        : m_width(width)
        , m_height(height)
        , m_format(format)
        , m_bMipMaps(bMipMaps) {}

    //-----------------------------------------------------------------------------------
    CBaseRenderTarget::~CBaseRenderTarget()
    {
        OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderTarget::ApplyChanges()
    {
        OnLostDevice();
        OnResetDevice();
    }

    //-----------------------------------------------------------------------------------
    int CBaseRenderTarget::GetWidth()
    {
        return m_width;
    }

    //-----------------------------------------------------------------------------------
    int CBaseRenderTarget::GetHeight()
    {
        return m_height;
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderTarget::SetWidth(int width)
    {
        m_width = width;
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderTarget::SetHeight(int height)
    {
        m_height = height;
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderTarget::SetSize(unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;
    }

} // env