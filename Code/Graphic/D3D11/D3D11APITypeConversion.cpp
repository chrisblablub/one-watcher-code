#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11APITypeConversion.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    unsigned long ConvertToD3D11RenderState(RENDERSTATE_TYPE renderStateType, unsigned long renderState)
    {
        unsigned long d3d11RenderState = renderState;

        if (renderStateType == RENDERSTATE_TYPE_BLENDOP ||
            renderStateType == RENDERSTATE_TYPE_BLENDOPALPHA)
        {
            switch (renderState)
            {
            case BLENDOP_TYPE_ADD:          d3d11RenderState = D3D11_BLEND_OP_ADD;          break;
            case BLENDOP_TYPE_SUBTRACT:     d3d11RenderState = D3D11_BLEND_OP_SUBTRACT;     break;
            case BLENDOP_TYPE_REVSUBTRACT:  d3d11RenderState = D3D11_BLEND_OP_REV_SUBTRACT; break;
            case BLENDOP_TYPE_MIN:          d3d11RenderState = D3D11_BLEND_OP_MIN;          break;
            case BLENDOP_TYPE_MAX:          d3d11RenderState = D3D11_BLEND_OP_MAX;          break;
            default:                        assert(false);                                  break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_SRCBLEND ||
                 renderStateType == RENDERSTATE_TYPE_DESTBLEND ||
                 renderStateType == RENDERSTATE_TYPE_SRCBLENDALPHA ||
                 renderStateType == RENDERSTATE_TYPE_DESTBLENDALPHA)
        {
            switch (renderState)
            {
            case BLEND_TYPE_ZERO:               d3d11RenderState = D3D11_BLEND_ZERO;            break;
            case BLEND_TYPE_ONE:                d3d11RenderState = D3D11_BLEND_ONE;             break;
            case BLEND_TYPE_SRC_COLOR:          d3d11RenderState = D3D11_BLEND_SRC_COLOR;       break;
            case BLEND_TYPE_INV_SRC_COLOR:      d3d11RenderState = D3D11_BLEND_INV_SRC_COLOR;   break;
            case BLEND_TYPE_SRC_ALPHA:          d3d11RenderState = D3D11_BLEND_SRC_ALPHA;       break;
            case BLEND_TYPE_INV_SRC_ALPHA:      d3d11RenderState = D3D11_BLEND_INV_SRC_ALPHA;   break;
            case BLEND_TYPE_DEST_ALPHA:         d3d11RenderState = D3D11_BLEND_DEST_ALPHA;      break;
            case BLEND_TYPE_INV_DEST_ALPHA:     d3d11RenderState = D3D11_BLEND_INV_DEST_ALPHA;  break;
            case BLEND_TYPE_DEST_COLOR:         d3d11RenderState = D3D11_BLEND_DEST_COLOR;      break;
            case BLEND_TYPE_INV_DEST_COLOR:     d3d11RenderState = D3D11_BLEND_INV_DEST_COLOR;  break;
            default:                            assert(false);                                  break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_FILLMODE)
        {
            switch (renderState)
            {
            case FILLMODE_TYPE_SOLID:       d3d11RenderState = D3D11_FILL_SOLID;     break;
            case FILLMODE_TYPE_WIREFRAME:   d3d11RenderState = D3D11_FILL_WIREFRAME; break;
            default:                        assert(false);                           break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_CULLMODE)
        {
            switch (renderState)
            {
            case CULLMODE_TYPE_NONE:    d3d11RenderState = D3D11_CULL_NONE;  break;
            case CULLMODE_TYPE_BACK:    d3d11RenderState = D3D11_CULL_BACK;  break;
            case CULLMODE_TYPE_FRONT:   d3d11RenderState = D3D11_CULL_FRONT; break;
            default:                    assert(false);                       break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_STENCILPASS ||
                 renderStateType == RENDERSTATE_TYPE_STENCILFAIL ||
                 renderStateType == RENDERSTATE_TYPE_STENCILZFAIL)
        {
            switch (renderState)
            {
            case STENCILOP_TYPE_KEEP:     d3d11RenderState = D3D11_STENCIL_OP_KEEP;     break;
            case STENCILOP_TYPE_ZERO:     d3d11RenderState = D3D11_STENCIL_OP_ZERO;     break;
            case STENCILOP_TYPE_REPLACE:  d3d11RenderState = D3D11_STENCIL_OP_REPLACE;  break;
            case STENCILOP_TYPE_INCR_SAT: d3d11RenderState = D3D11_STENCIL_OP_INCR_SAT; break;
            case STENCILOP_TYPE_DECR_SAT: d3d11RenderState = D3D11_STENCIL_OP_DECR_SAT; break;
            case STENCILOP_TYPE_INVERT:   d3d11RenderState = D3D11_STENCIL_OP_INVERT;   break;
            case STENCILOP_TYPE_INCR:     d3d11RenderState = D3D11_STENCIL_OP_INCR;     break;
            case STENCILOP_TYPE_DECR:     d3d11RenderState = D3D11_STENCIL_OP_DECR;     break;
            default:                      assert(false);                                break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_ZFUNC ||
                 renderStateType == RENDERSTATE_TYPE_ALPHAFUNC ||
                 renderStateType == RENDERSTATE_TYPE_STENCILFUNC)
        {
            switch (renderState)
            {
            case COMPARISON_TYPE_ALWAYS:        d3d11RenderState = D3D11_COMPARISON_ALWAYS;         break;
            case COMPARISON_TYPE_EQUAL:         d3d11RenderState = D3D11_COMPARISON_EQUAL;          break;
            case COMPARISON_TYPE_GREATER:       d3d11RenderState = D3D11_COMPARISON_GREATER;        break;
            case COMPARISON_TYPE_GREATEREQUAL:  d3d11RenderState = D3D11_COMPARISON_GREATER_EQUAL;  break;
            case COMPARISON_TYPE_LESS:          d3d11RenderState = D3D11_COMPARISON_LESS;           break;
            case COMPARISON_TYPE_LESSEQUAL:     d3d11RenderState = D3D11_COMPARISON_LESS_EQUAL;     break;
            case COMPARISON_TYPE_NOTEQUAL:      d3d11RenderState = D3D11_COMPARISON_NOT_EQUAL;      break;
            default:                            ENV_ASSERT_DBG(false);                              break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_ZWRITEENABLE)
        {
            d3d11RenderState = renderState ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO; 
        }

        return d3d11RenderState;
    }

    //-----------------------------------------------------------------------------------
    unsigned long ConvertToD3D11SamplerState(SAMPLERSTATE_TYPE samplerStateType, unsigned long samplerState)
    {
        unsigned long d3d11SamplerState = 0;

        if (samplerStateType == SAMPLERSTATE_TYPE_ADDRESSU ||
            samplerStateType == SAMPLERSTATE_TYPE_ADDRESSV ||
            samplerStateType == SAMPLERSTATE_TYPE_ADDRESSW)
        {
            switch (samplerState)
            {
            case TEXTUREADDRESS_TYPE_CLAMP:     d3d11SamplerState = D3D11_TEXTURE_ADDRESS_CLAMP;    break;
            case TEXTUREADDRESS_TYPE_WRAP:      d3d11SamplerState = D3D11_TEXTURE_ADDRESS_WRAP;     break;
            case TEXTUREADDRESS_TYPE_MIRROR:    d3d11SamplerState = D3D11_TEXTURE_ADDRESS_MIRROR;   break;
            default:                            assert(false);                                      break;
            }
        }
        
        return d3d11SamplerState;
    }

    //-----------------------------------------------------------------------------------
    unsigned long ConvertToD3D11SamplerStateFilter(unsigned long minFilter, unsigned long magFilter, unsigned long mipFilter)
    {
        unsigned long d3d11SamplerFilter = 0;

        if (mipFilter == TEXTUREFILTER_TYPE_NONE)
            mipFilter = TEXTUREFILTER_TYPE_POINT;

        if (minFilter == TEXTUREFILTER_TYPE_POINT &&
            magFilter == TEXTUREFILTER_TYPE_POINT &&
            mipFilter == TEXTUREFILTER_TYPE_POINT)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        }
        else if (minFilter == TEXTUREFILTER_TYPE_POINT &&
                 magFilter == TEXTUREFILTER_TYPE_POINT &&
                 mipFilter == TEXTUREFILTER_TYPE_LINEAR)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        }
        else if (minFilter == TEXTUREFILTER_TYPE_POINT &&
                 magFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 mipFilter == TEXTUREFILTER_TYPE_POINT)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        }
        else if (minFilter == TEXTUREFILTER_TYPE_POINT &&
                 magFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 mipFilter == TEXTUREFILTER_TYPE_LINEAR)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
        }
        else if (minFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 magFilter == TEXTUREFILTER_TYPE_POINT &&
                 mipFilter == TEXTUREFILTER_TYPE_POINT)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        }
        else if (minFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 magFilter == TEXTUREFILTER_TYPE_POINT &&
                 mipFilter == TEXTUREFILTER_TYPE_LINEAR)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
        }
        else if (minFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 magFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 mipFilter == TEXTUREFILTER_TYPE_POINT)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        }
        else if (minFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 magFilter == TEXTUREFILTER_TYPE_LINEAR &&
                 mipFilter == TEXTUREFILTER_TYPE_LINEAR)
        {
            d3d11SamplerFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }
        else if (mipFilter == TEXTUREFILTER_TYPE_ANISOTROPIC)
        {
            d3d11SamplerFilter = D3D11_FILTER_ANISOTROPIC;
        }
        
        return d3d11SamplerFilter;
    }

    //-----------------------------------------------------------------------------------
    unsigned long ConvertFromD3D11(const D3D11_FILL_MODE& d3d11FillMode)
    {
        unsigned long renderStateType = 0;
       
        switch (d3d11FillMode)
        {
        case D3D11_FILL_SOLID:          renderStateType = FILLMODE_TYPE_SOLID;      break;
        case D3D11_FILL_WIREFRAME:      renderStateType = FILLMODE_TYPE_WIREFRAME;  break;
        default:                        assert(false);                              break;
        }

        return renderStateType;
    }

    //-----------------------------------------------------------------------------------
    unsigned long ConvertFromD3D11(const D3D11_CULL_MODE& d3d11CullMode)
    {
        unsigned long renderStateType = 0;

        switch (d3d11CullMode)
        {
        case D3D11_CULL_BACK:           renderStateType = CULLMODE_TYPE_BACK;       break;
        case D3D11_CULL_FRONT:          renderStateType = CULLMODE_TYPE_FRONT;      break;
        default:                        assert(false);                              break;
        }

        return renderStateType;
    }

    //-----------------------------------------------------------------------------------
    unsigned long ConvertToD3D11ClearType(unsigned long type)
    {
        unsigned long d3dClear = 0;

        if (type & CLEAR_TYPE_STENCIL)
            d3dClear |= D3D11_CLEAR_STENCIL;
        if (type & CLEAR_TYPE_ZBUFFER)
            d3dClear |= D3D11_CLEAR_DEPTH;

        return d3dClear;
    }

    //-----------------------------------------------------------------------------------
    TEXTUREFORMAT_TYPE ConvertFromD3D11(DXGI_FORMAT mem)
    {
        TEXTUREFORMAT_TYPE d3d11Format;

        switch (mem)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM:        d3d11Format = TEXTUREFORMAT_TYPE_A8R8G8B8;      break;
        case DXGI_FORMAT_B8G8R8X8_UNORM:        d3d11Format = TEXTUREFORMAT_TYPE_X8R8G8B8;      break;
        case DXGI_FORMAT_R16_FLOAT:             d3d11Format = TEXTUREFORMAT_TYPE_R16F;          break;
        case DXGI_FORMAT_R32_FLOAT:             d3d11Format = TEXTUREFORMAT_TYPE_R32F;          break;
        case DXGI_FORMAT_R16G16B16A16_FLOAT:    d3d11Format = TEXTUREFORMAT_TYPE_A16B16G16R16F; break;
        case DXGI_FORMAT_R32G32B32A32_FLOAT:    d3d11Format = TEXTUREFORMAT_TYPE_A32B32G32R32F; break;
        case DXGI_FORMAT_R10G10B10A2_UINT:      d3d11Format = TEXTUREFORMAT_TYPE_R10G10B10A2;   break;
        case DXGI_FORMAT_R11G11B10_FLOAT:       d3d11Format = TEXTUREFORMAT_TYPE_R11G11B10;     break;
        case DXGI_FORMAT_D16_UNORM:             d3d11Format = TEXTUREFORMAT_TYPE_INDEX16;       break;
        case DXGI_FORMAT_D32_FLOAT:             d3d11Format = TEXTUREFORMAT_TYPE_INDEX32;       break;
        case DXGI_FORMAT_D24_UNORM_S8_UINT:     d3d11Format = TEXTUREFORMAT_TYPE_D24S8;         break;
        default:                                assert(false);                                  break;
        }

        return d3d11Format;
    }

    //-----------------------------------------------------------------------------------
    DXGI_FORMAT ConvertToD3D11Format(TEXTUREFORMAT_TYPE mem)
    {
        DXGI_FORMAT d3d11Format;

        switch (mem)
        {
        case TEXTUREFORMAT_TYPE_A8R8G8B8:       d3d11Format = DXGI_FORMAT_R8G8B8A8_UNORM;     break;
        case TEXTUREFORMAT_TYPE_X8R8G8B8:       d3d11Format = DXGI_FORMAT_B8G8R8X8_UNORM;     break;
        case TEXTUREFORMAT_TYPE_R16F:           d3d11Format = DXGI_FORMAT_R16_FLOAT;          break;
        case TEXTUREFORMAT_TYPE_R32F:           d3d11Format = DXGI_FORMAT_R32_FLOAT;          break;
        case TEXTUREFORMAT_TYPE_A16B16G16R16F:  d3d11Format = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
        case TEXTUREFORMAT_TYPE_A32B32G32R32F:  d3d11Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
        case TEXTUREFORMAT_TYPE_R10G10B10A2:    d3d11Format = DXGI_FORMAT_R10G10B10A2_UINT;   break;
        case TEXTUREFORMAT_TYPE_R11G11B10:      d3d11Format = DXGI_FORMAT_R11G11B10_FLOAT;   break;
        case TEXTUREFORMAT_TYPE_INDEX16:        d3d11Format = DXGI_FORMAT_D16_UNORM;          break;
        case TEXTUREFORMAT_TYPE_INDEX32:        d3d11Format = DXGI_FORMAT_D32_FLOAT;          break;
        case TEXTUREFORMAT_TYPE_D24S8:          d3d11Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  break;
        default:                                assert(false);                                break;
        }

        return d3d11Format;
    }

    //-----------------------------------------------------------------------------------
    D3D11_USAGE ConvertToD3D11Usage(unsigned long usage)
    {
        D3D11_USAGE d3d11Usage = D3D11_USAGE_DEFAULT;

        if ((usage & USAGE_TYPE_CPUWRITEACCESS) &&
            (usage & USAGE_TYPE_GPUREADACCESS))
        {
            d3d11Usage = D3D11_USAGE_DYNAMIC;
        }
        else if ((usage & ~USAGE_TYPE_CPUREADACCESS) &&
                 (usage & ~USAGE_TYPE_CPUWRITEACCESS) &&
                 (usage & USAGE_TYPE_GPUREADACCESS))
        {
            d3d11Usage = D3D11_USAGE_IMMUTABLE;
        }
        else
        {
            d3d11Usage = D3D11_USAGE_DEFAULT;
        }

        return d3d11Usage;
    }

    //-----------------------------------------------------------------------------------
    D3D11_MAP ConvertToD3D11Map(unsigned long lockFlags)
    {
        D3D11_MAP d3d11Map = D3D11_MAP_WRITE_DISCARD;

        if (lockFlags & LOCK_TYPE_WRITE &&
            lockFlags & LOCK_TYPE_READ)
        {
            d3d11Map = D3D11_MAP_READ_WRITE;
        }
        else if (lockFlags & LOCK_TYPE_READ)
        {
            d3d11Map = D3D11_MAP_READ;
        }
        else if (lockFlags & LOCK_TYPE_WRITE)
        {
            d3d11Map = D3D11_MAP_WRITE;
        }
        else if (lockFlags & LOCK_TYPE_WRITEDISCARD)
        {
            d3d11Map = D3D11_MAP_WRITE_DISCARD;
        }

        ENV_ASSERT_DBG(d3d11Map != 0);

        return d3d11Map;
    }

} // env
#endif // ENV_GRAPHIC_API_D3D11