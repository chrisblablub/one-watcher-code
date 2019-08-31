/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef TEXTURE_SAMPLER_H
#define TEXTURE_SAMPLER_H

#include <Graphic/GraphicCommon.h>
#include <Graphic/Core/APITypes.h>

namespace env
{
    class CTextureSampler
    {
    public:
        CTextureSampler()
            : m_addressU(TEXTUREADDRESS_TYPE_WRAP)
            , m_addressV(TEXTUREADDRESS_TYPE_WRAP)
            , m_addressW(TEXTUREADDRESS_TYPE_WRAP)
            , m_magFilter(TEXTUREFILTER_TYPE_LINEAR)
            , m_minFilter(TEXTUREFILTER_TYPE_LINEAR)
            , m_mipFilter(TEXTUREFILTER_TYPE_NONE)
            , m_maxAnisotropy(0) {}

        CTextureSampler(TEXTUREADDRESS_TYPE addressU,
                        TEXTUREADDRESS_TYPE addressV,
                        TEXTUREFILTER_TYPE minFilter, 
                        TEXTUREFILTER_TYPE magFilter, 
                        TEXTUREFILTER_TYPE mipFilter,
                        int maxAnisotropy = 0)
                        : m_addressU(addressU)
                        , m_addressV(addressV)
                        , m_addressW(addressU)
                        , m_minFilter(minFilter)
                        , m_magFilter(magFilter)
                        , m_mipFilter(mipFilter)
                        , m_maxAnisotropy(maxAnisotropy) {}

        ~CTextureSampler() {}

        void SetAddressU(TEXTUREADDRESS_TYPE addressU) { m_addressU = addressU; }
        TEXTUREADDRESS_TYPE GetAddressU() const { return m_addressU; }

        void SetAddressV(TEXTUREADDRESS_TYPE addressV) { m_addressV = addressV; }
        TEXTUREADDRESS_TYPE GetAddressV() const { return m_addressV; }

        void SetAddressW(TEXTUREADDRESS_TYPE addressW) { m_addressW = addressW; }
        TEXTUREADDRESS_TYPE GetAddressW() const { return m_addressW; }

        void SetMagFilter(TEXTUREFILTER_TYPE filter) { m_magFilter = filter; }
        TEXTUREFILTER_TYPE GetMagFilter() const { return m_magFilter; }

        void SetMinFilter(TEXTUREFILTER_TYPE filter) { m_minFilter = filter; }
        TEXTUREFILTER_TYPE GetMinFilter() const { return m_minFilter; }

        void SetMipFilter(TEXTUREFILTER_TYPE filter) { m_mipFilter = filter; }
        TEXTUREFILTER_TYPE GetMipFilter() const { return m_mipFilter; }

        void SetMaxAnisotropy(int maxAni) { m_maxAnisotropy = maxAni; }
        int GetMaxAnisotropy() const { return m_maxAnisotropy; }

        bool operator==(const CTextureSampler& other) const
        {
            return  m_addressU == other.m_addressU &&
                m_addressV == other.m_addressV &&
                m_addressW == other.m_addressW &&
                m_minFilter == other.m_minFilter &&
                m_magFilter == other.m_magFilter &&
                m_mipFilter == other.m_mipFilter;
        }

    protected:

        TEXTUREADDRESS_TYPE m_addressU;
        TEXTUREADDRESS_TYPE m_addressV;
        TEXTUREADDRESS_TYPE m_addressW;

        TEXTUREFILTER_TYPE m_minFilter;
        TEXTUREFILTER_TYPE m_magFilter;
        TEXTUREFILTER_TYPE m_mipFilter;

        int m_maxAnisotropy;
    };

    class CDefaultTextureSampler
    {
    public:
        static const CTextureSampler& GetPointClampSampler() { return m_pointClampSampler; }
        static const CTextureSampler& GetPointWrapSampler() { return m_pointWrapSampler; }
        static const CTextureSampler& GetLinearClampSampler() { return m_linearClampSampler; }
        static const CTextureSampler& GetLinearWrapSampler() { return m_linearWrapSampler; }
        static const CTextureSampler& GetLinearMipClampSampler() { return m_linearMipClampSampler; }
        static const CTextureSampler& GetLinearMipWrapSampler() { return m_linearMipWrapSampler; }

        static const CTextureSampler& GetSampler(const std::string& strSampler);
        static const std::string GetSamplerName(const CTextureSampler& sampler);

    private:
        static CTextureSampler m_pointClampSampler;
        static CTextureSampler m_pointWrapSampler;
        static CTextureSampler m_linearClampSampler;
        static CTextureSampler m_linearWrapSampler;
        static CTextureSampler m_linearMipClampSampler;
        static CTextureSampler m_linearMipWrapSampler;
    };
} // env

#endif // TEXTURE_SAMPLER_H