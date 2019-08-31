
#include <Graphic/Core/TextureSampler.h>

namespace env
{
    // Default samplers
    //-----------------------------------------------------------------------------------
    CTextureSampler CDefaultTextureSampler::m_pointClampSampler = CTextureSampler(TEXTUREADDRESS_TYPE_CLAMP,
                                                                                  TEXTUREADDRESS_TYPE_CLAMP,
                                                                                  TEXTUREFILTER_TYPE_POINT,
                                                                                  TEXTUREFILTER_TYPE_POINT,
                                                                                  TEXTUREFILTER_TYPE_NONE);

    //-----------------------------------------------------------------------------------
    CTextureSampler CDefaultTextureSampler::m_pointWrapSampler = CTextureSampler(TEXTUREADDRESS_TYPE_WRAP,
                                                                                 TEXTUREADDRESS_TYPE_WRAP,
                                                                                 TEXTUREFILTER_TYPE_POINT,
                                                                                 TEXTUREFILTER_TYPE_POINT,
                                                                                 TEXTUREFILTER_TYPE_NONE);

    //-----------------------------------------------------------------------------------
    CTextureSampler CDefaultTextureSampler::m_linearClampSampler = CTextureSampler(TEXTUREADDRESS_TYPE_CLAMP,
                                                                                   TEXTUREADDRESS_TYPE_CLAMP,
                                                                                   TEXTUREFILTER_TYPE_LINEAR,
                                                                                   TEXTUREFILTER_TYPE_LINEAR,
                                                                                   TEXTUREFILTER_TYPE_NONE);

    //-----------------------------------------------------------------------------------
    CTextureSampler CDefaultTextureSampler::m_linearWrapSampler = CTextureSampler(TEXTUREADDRESS_TYPE_WRAP,
                                                                                  TEXTUREADDRESS_TYPE_WRAP,
                                                                                  TEXTUREFILTER_TYPE_LINEAR,
                                                                                  TEXTUREFILTER_TYPE_LINEAR,
                                                                                  TEXTUREFILTER_TYPE_NONE);

    //-----------------------------------------------------------------------------------
    CTextureSampler CDefaultTextureSampler::m_linearMipClampSampler = CTextureSampler(TEXTUREADDRESS_TYPE_CLAMP,
                                                                                      TEXTUREADDRESS_TYPE_CLAMP,
                                                                                      TEXTUREFILTER_TYPE_LINEAR,
                                                                                      TEXTUREFILTER_TYPE_LINEAR,
                                                                                      TEXTUREFILTER_TYPE_LINEAR);

    //-----------------------------------------------------------------------------------
    CTextureSampler CDefaultTextureSampler::m_linearMipWrapSampler = CTextureSampler(TEXTUREADDRESS_TYPE_WRAP,
                                                                                     TEXTUREADDRESS_TYPE_WRAP,
                                                                                     TEXTUREFILTER_TYPE_LINEAR,
                                                                                     TEXTUREFILTER_TYPE_LINEAR,
                                                                                     TEXTUREFILTER_TYPE_LINEAR);

    //-----------------------------------------------------------------------------------
    const CTextureSampler& CDefaultTextureSampler::GetSampler(const std::string& strSampler)
    {
        if (strSampler == "PointClamp")
            return GetPointClampSampler();
        else if (strSampler == "PointWrap")
            return GetPointWrapSampler();
        else if (strSampler == "LinearClamp")
            return GetLinearClampSampler();
        else if (strSampler == "LinearWrap")
            return GetLinearWrapSampler();
        else if (strSampler == "LinearMipClamp")
            return GetLinearMipClampSampler();
        else if (strSampler == "LinearMipWrap")
            return GetLinearMipWrapSampler();
        
        ENV_ASSERT_DBG(false && "No valid sampler name specified");

        return GetLinearWrapSampler();
    }

    //-----------------------------------------------------------------------------------
    const std::string CDefaultTextureSampler::GetSamplerName(const CTextureSampler& sampler)
    {
        if (sampler == GetPointClampSampler())
            return "PointClamp";
        else if (sampler == GetPointWrapSampler())
            return "PointWrap";
        else if (sampler == GetLinearClampSampler())
            return "LinearClamp";
        else if (sampler == GetLinearWrapSampler())
            return "LinearWrap";
        else if (sampler == GetLinearMipClampSampler())
            return "LinearMipClamp";
        else if (sampler == GetLinearMipWrapSampler())
            return "LinearMipWrap";
        
        return std::string();
    }

}