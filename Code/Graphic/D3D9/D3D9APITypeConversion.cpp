#ifdef D3D9

#include "D3D9APITypeConversion.h"

namespace env
{
    D3DPRIMITIVETYPE ConvertToD3D9PrimitiveType(PRIMITIVE_TYPE type)
    {
        D3DPRIMITIVETYPE d3d9Type;

        switch (type)
        {
        case PRIMITIVE_TYPE_POINTLIST:		d3d9Type = D3DPT_POINTLIST;		break;
        case PRIMITIVE_TYPE_LINELIST:		d3d9Type = D3DPT_LINELIST;		break;
        case PRIMITIVE_TYPE_LINESTRIP:		d3d9Type = D3DPT_LINESTRIP;		break;
        case PRIMITIVE_TYPE_TRIANGLELIST:	d3d9Type = D3DPT_TRIANGLELIST;	break;
        case PRIMITIVE_TYPE_TRIANGLESTRIP:	d3d9Type = D3DPT_TRIANGLESTRIP;	break;
        case PRIMITIVE_TYPE_TRIANGLEFAN:	d3d9Type = D3DPT_TRIANGLEFAN;	break;
        }

        return d3d9Type;
    }

    void ConvertToD3D9SamplerStateType(SAMPLERSTATE_TYPE type, unsigned long value, D3DSAMPLERSTATETYPE& d3d9Sampler, unsigned long& d3d9Value)
    {

        switch (type)
        {
        case SAMPLERSTATE_TYPE_ADDRESSU:
        {
            d3d9Sampler = D3DSAMP_ADDRESSU;

            switch (value)
            {
            case TEXTUREADDRESS_TYPE_CLAMP:  d3d9Value = D3DTADDRESS_CLAMP;		break;
            case TEXTUREADDRESS_TYPE_MIRROR: d3d9Value = D3DTADDRESS_MIRROR;	break;
            case TEXTUREADDRESS_TYPE_WRAP:	 d3d9Value = D3DTADDRESS_WRAP;		break;
            default:						 assert(false);						break;
            }
        } break;

        case SAMPLERSTATE_TYPE_ADDRESSV:
        {
            d3d9Sampler = D3DSAMP_ADDRESSV;

            switch (value)
            {
            case TEXTUREADDRESS_TYPE_CLAMP:  d3d9Value = D3DTADDRESS_CLAMP;		break;
            case TEXTUREADDRESS_TYPE_MIRROR: d3d9Value = D3DTADDRESS_MIRROR;	break;
            case TEXTUREADDRESS_TYPE_WRAP:	 d3d9Value = D3DTADDRESS_WRAP;		break;
            default:						 assert(false);						break;
            }
        } break;

        case SAMPLERSTATE_TYPE_ADDRESSW:
        {
            d3d9Sampler = D3DSAMP_ADDRESSW;

            switch (value)
            {
            case TEXTUREADDRESS_TYPE_CLAMP:  d3d9Value = D3DTADDRESS_CLAMP;		break;
            case TEXTUREADDRESS_TYPE_MIRROR: d3d9Value = D3DTADDRESS_MIRROR;	break;
            case TEXTUREADDRESS_TYPE_WRAP:	 d3d9Value = D3DTADDRESS_WRAP;		break;
            default:						 assert(false);						break;
            }
        }	break;


        case SAMPLERSTATE_TYPE_MAGFILTER:
        {
            d3d9Sampler = D3DSAMP_MAGFILTER;

            switch (value)
            {
            case TEXTUREFILTER_TYPE_ANISOTROPIC:	d3d9Value = D3DTEXF_ANISOTROPIC;	break;
            case TEXTUREFILTER_TYPE_LINEAR:			d3d9Value = D3DTEXF_LINEAR;			break;
            case TEXTUREFILTER_TYPE_NONE:			d3d9Value = D3DTEXF_NONE;			break;
            case TEXTUREFILTER_TYPE_POINT:			d3d9Value = D3DTEXF_POINT;			break;
            default:								assert(false);						break;
            }
        } break;

        case SAMPLERSTATE_TYPE_MINFILTER:
        {
            d3d9Sampler = D3DSAMP_MINFILTER;

            switch (value)
            {
            case TEXTUREFILTER_TYPE_ANISOTROPIC:	d3d9Value = D3DTEXF_ANISOTROPIC;	break;
            case TEXTUREFILTER_TYPE_LINEAR:			d3d9Value = D3DTEXF_LINEAR;			break;
            case TEXTUREFILTER_TYPE_NONE:			d3d9Value = D3DTEXF_NONE;			break;
            case TEXTUREFILTER_TYPE_POINT:			d3d9Value = D3DTEXF_POINT;			break;
            default:								assert(false);						break;
            }
        } break;

        case SAMPLERSTATE_TYPE_MIPFILTER:
        {
            d3d9Sampler = D3DSAMP_MIPFILTER;

            switch (value)
            {
            case TEXTUREFILTER_TYPE_ANISOTROPIC:	d3d9Value = D3DTEXF_ANISOTROPIC;	break;
            case TEXTUREFILTER_TYPE_LINEAR:			d3d9Value = D3DTEXF_LINEAR;			break;
            case TEXTUREFILTER_TYPE_NONE:			d3d9Value = D3DTEXF_NONE;			break;
            case TEXTUREFILTER_TYPE_POINT:			d3d9Value = D3DTEXF_POINT;			break;
            default:								assert(false);						break;
            }
        } break;


        case SAMPLERSTATE_TYPE_MAXMIPLEVEL:
            d3d9Sampler = D3DSAMP_MAXMIPLEVEL;
            d3d9Value = value;
            break;

        case SAMPLERSTATE_TYPE_MAXANISOTROPY:
            d3d9Sampler = D3DSAMP_MAXANISOTROPY;
            d3d9Value = value;
            break;

        case SAMPLERSTATE_TYPE_SRGBTEXTURE:
            d3d9Sampler = D3DSAMP_SRGBTEXTURE;
            d3d9Value = value;
            break;
        }
    }


    D3DRENDERSTATETYPE ConvertToD3D9RenderStateType(RENDERSTATE_TYPE rs)
    {
        D3DRENDERSTATETYPE d3d9RenderStateType;

        switch (rs)
        {
        case RENDERSTATE_TYPE_ZENABLE:					d3d9RenderStateType = D3DRS_ZENABLE;					break;
        case RENDERSTATE_TYPE_ZWRITEENABLE:				d3d9RenderStateType = D3DRS_ZWRITEENABLE;				break;
        case RENDERSTATE_TYPE_FILLMODE:					d3d9RenderStateType = D3DRS_FILLMODE;					break;
        case RENDERSTATE_TYPE_ALPHATESTENABLE:			d3d9RenderStateType = D3DRS_ALPHATESTENABLE;			break;
        case RENDERSTATE_TYPE_SRCBLEND:					d3d9RenderStateType = D3DRS_SRCBLEND;					break;
        case RENDERSTATE_TYPE_DESTBLEND:				d3d9RenderStateType = D3DRS_DESTBLEND;					break;
        case RENDERSTATE_TYPE_CULLMODE:					d3d9RenderStateType = D3DRS_CULLMODE;					break;
        case RENDERSTATE_TYPE_ZFUNC:					d3d9RenderStateType = D3DRS_ZFUNC;						break;
        case RENDERSTATE_TYPE_ALPHAREF:					d3d9RenderStateType = D3DRS_ALPHAREF;					break;
        case RENDERSTATE_TYPE_ALPHAFUNC:				d3d9RenderStateType = D3DRS_ALPHAFUNC;					break;
        case RENDERSTATE_TYPE_ALPHABLENDENABLE:			d3d9RenderStateType = D3DRS_ALPHABLENDENABLE;			break;
        case RENDERSTATE_TYPE_STENCILENABLE:			d3d9RenderStateType = D3DRS_STENCILENABLE;				break;
        case RENDERSTATE_TYPE_STENCILFAIL:				d3d9RenderStateType = D3DRS_STENCILFAIL;				break;
        case RENDERSTATE_TYPE_STENCILZFAIL:				d3d9RenderStateType = D3DRS_STENCILZFAIL;				break;
        case RENDERSTATE_TYPE_STENCILPASS:				d3d9RenderStateType = D3DRS_STENCILPASS;				break;
        case RENDERSTATE_TYPE_STENCILFUNC:				d3d9RenderStateType = D3DRS_STENCILFUNC;				break;
        case RENDERSTATE_TYPE_STENCILREF:				d3d9RenderStateType = D3DRS_STENCILREF;					break;
        case RENDERSTATE_TYPE_STENCILREADMASK:			d3d9RenderStateType = D3DRS_STENCILMASK;				break;
        case RENDERSTATE_TYPE_STENCILWRITEMASK:			d3d9RenderStateType = D3DRS_STENCILWRITEMASK;			break;
        case RENDERSTATE_TYPE_COLORWRITEENABLE:			d3d9RenderStateType = D3DRS_COLORWRITEENABLE;			break;
        case RENDERSTATE_TYPE_BLENDOP:					d3d9RenderStateType = D3DRS_BLENDOP;					break;
        case RENDERSTATE_TYPE_SCISSORTESTENABLE:		d3d9RenderStateType = D3DRS_SCISSORTESTENABLE;			break;
            //		case RENDERSTATE_TYPE_BLENDFACTOR:				d3d9RenderStateType = D3DRS_BLENDFACTOR;				break;
        case RENDERSTATE_TYPE_SRGBWRITEENABLE:			d3d9RenderStateType = D3DRS_SRGBWRITEENABLE;			break;
        case RENDERSTATE_TYPE_SEPARATEALPHABLENDENABLE: d3d9RenderStateType = D3DRS_SEPARATEALPHABLENDENABLE;	break;
        case RENDERSTATE_TYPE_SRCBLENDALPHA:			d3d9RenderStateType = D3DRS_SRCBLENDALPHA;				break;
        case RENDERSTATE_TYPE_DESTBLENDALPHA:			d3d9RenderStateType = D3DRS_DESTBLENDALPHA;				break;
        case RENDERSTATE_TYPE_BLENDOPALPHA:				d3d9RenderStateType = D3DRS_BLENDOPALPHA;				break;
        default:										assert(false);											break;
        }

        return d3d9RenderStateType;
    }

    unsigned long ConvertToD3D9RenderStateValue(RENDERSTATE_TYPE renderStateType, unsigned long renderState)
    {
        unsigned long d3d11RenderState = renderState;

        if (renderStateType == RENDERSTATE_TYPE_BLENDOP ||
            renderStateType == RENDERSTATE_TYPE_BLENDOPALPHA)
        {
            switch (renderState)
            {
            case BLENDOP_TYPE_ADD:          d3d11RenderState = D3DBLENDOP_ADD;          break;
            case BLENDOP_TYPE_SUBTRACT:     d3d11RenderState = D3DBLENDOP_SUBTRACT;     break;
            case BLENDOP_TYPE_REVSUBTRACT:  d3d11RenderState = D3DBLENDOP_REVSUBTRACT;  break;
            case BLENDOP_TYPE_MIN:          d3d11RenderState = D3DBLENDOP_MIN;          break;
            case BLENDOP_TYPE_MAX:          d3d11RenderState = D3DBLENDOP_MAX;          break;
            default:                        assert(false);                              break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_SRCBLEND ||
                 renderStateType == RENDERSTATE_TYPE_DESTBLEND ||
                 renderStateType == RENDERSTATE_TYPE_SRCBLENDALPHA ||
                 renderStateType == RENDERSTATE_TYPE_DESTBLENDALPHA)
        {
            switch (renderState)
            {
            case BLEND_TYPE_ZERO:               d3d11RenderState = D3DBLEND_ZERO;          break;
            case BLEND_TYPE_ONE:                d3d11RenderState = D3DBLEND_ONE;           break;
            case BLEND_TYPE_SRC_COLOR:          d3d11RenderState = D3DBLEND_SRCCOLOR;      break;
            case BLEND_TYPE_INV_SRC_COLOR:      d3d11RenderState = D3DBLEND_INVSRCCOLOR;   break;
            case BLEND_TYPE_SRC_ALPHA:          d3d11RenderState = D3DBLEND_SRCALPHA;      break;
            case BLEND_TYPE_INV_SRC_ALPHA:      d3d11RenderState = D3DBLEND_INVSRCALPHA;   break;
            case BLEND_TYPE_DEST_ALPHA:         d3d11RenderState = D3DBLEND_DESTALPHA;     break;
            case BLEND_TYPE_INV_DEST_ALPHA:     d3d11RenderState = D3DBLEND_INVDESTALPHA;  break;
            case BLEND_TYPE_DEST_COLOR:         d3d11RenderState = D3DBLEND_DESTCOLOR;     break;
            case BLEND_TYPE_INV_DEST_COLOR:     d3d11RenderState = D3DBLEND_INVDESTCOLOR;  break;
            default:                            assert(false);                             break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_FILLMODE)
        {
            switch (renderState)
            {
            case FILLMODE_TYPE_SOLID:       d3d11RenderState = D3DFILL_SOLID;     break;
            case FILLMODE_TYPE_WIREFRAME:   d3d11RenderState = D3DFILL_WIREFRAME; break;
            default:                        assert(false);                        break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_CULLMODE)
        {
            switch (renderState)
            {
            case CULLMODE_TYPE_NONE:    d3d11RenderState = D3DCULL_NONE;  break;
            case CULLMODE_TYPE_BACK:    d3d11RenderState = D3DCULL_CCW;   break;
            case CULLMODE_TYPE_FRONT:   d3d11RenderState = D3DCULL_CW;    break;
            default:                    assert(false);                    break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_STENCILPASS ||
                 renderStateType == RENDERSTATE_TYPE_STENCILFAIL ||
                 renderStateType == RENDERSTATE_TYPE_STENCILZFAIL)
        {
            switch (renderState)
            {
            case STENCILOP_TYPE_KEEP:     d3d11RenderState = D3DSTENCILOP_KEEP;     break;
            case STENCILOP_TYPE_ZERO:     d3d11RenderState = D3DSTENCILOP_ZERO;     break;
            case STENCILOP_TYPE_REPLACE:  d3d11RenderState = D3DSTENCILOP_REPLACE;  break;
            case STENCILOP_TYPE_INCR_SAT: d3d11RenderState = D3DSTENCILOP_INCRSAT;  break;
            case STENCILOP_TYPE_DECR_SAT: d3d11RenderState = D3DSTENCILOP_DECRSAT;  break;
            case STENCILOP_TYPE_INVERT:   d3d11RenderState = D3DSTENCILOP_INVERT;   break;
            case STENCILOP_TYPE_INCR:     d3d11RenderState = D3DSTENCILOP_INCR;     break;
            case STENCILOP_TYPE_DECR:     d3d11RenderState = D3DSTENCILOP_DECR;     break;
            default:                      assert(false);                            break;
            }
        }
        else if (renderStateType == RENDERSTATE_TYPE_ZFUNC ||
                 renderStateType == RENDERSTATE_TYPE_ALPHAFUNC ||
                 renderStateType == RENDERSTATE_TYPE_STENCILFUNC)
        {
            switch (renderState)
            {
            case COMPARISON_TYPE_ALWAYS:        d3d11RenderState = D3DCMP_ALWAYS;        break;
            case COMPARISON_TYPE_EQUAL:         d3d11RenderState = D3DCMP_EQUAL;         break;
            case COMPARISON_TYPE_GREATER:       d3d11RenderState = D3DCMP_GREATER;       break;
            case COMPARISON_TYPE_GREATEREQUAL:  d3d11RenderState = D3DCMP_GREATEREQUAL;  break;
            case COMPARISON_TYPE_LESS:          d3d11RenderState = D3DCMP_LESS;          break;
            case COMPARISON_TYPE_LESSEQUAL:     d3d11RenderState = D3DCMP_LESSEQUAL;     break;
            case COMPARISON_TYPE_NOTEQUAL:      d3d11RenderState = D3DCMP_NOTEQUAL;      break;
            default:                            ENV_ASSERT_DBG(false);                   break;
            }
        }

        return d3d11RenderState;
    }

    D3DPOOL ConvertToD3D9Pool(MEMORY_TYPE mem)
    {
        D3DPOOL d3d9Pool;

        switch (mem)
        {
        case MEMORY_TYPE_DEFAULT:	d3d9Pool = D3DPOOL_DEFAULT;		break;
        case MEMORY_TYPE_MANAGED:	d3d9Pool = D3DPOOL_MANAGED;		break;
        case MEMORY_TYPE_SYSTEMMEM: d3d9Pool = D3DPOOL_SYSTEMMEM;	break;
        default:					assert(false);					break;
        }

        return d3d9Pool;
    }

    D3DFORMAT ConvertToD3D9Format(TEXTUREFORMAT_TYPE mem)
    {
        D3DFORMAT d3d9Format;

        switch (mem)
        {
        case TEXTUREFORMAT_TYPE_A8R8G8B8:		d3d9Format = D3DFMT_A8R8G8B8;		break;
        case TEXTUREFORMAT_TYPE_X8R8G8B8:		d3d9Format = D3DFMT_X8R8G8B8;		break;
        case TEXTUREFORMAT_TYPE_R16F:			d3d9Format = D3DFMT_R16F;			break;
        case TEXTUREFORMAT_TYPE_R32F:			d3d9Format = D3DFMT_R32F;			break;
        case TEXTUREFORMAT_TYPE_A16B16G16R16F:	d3d9Format = D3DFMT_A16B16G16R16F;	break;
        case TEXTUREFORMAT_TYPE_A32B32G32R32F:	d3d9Format = D3DFMT_A32B32G32R32F;	break;
        case TEXTUREFORMAT_TYPE_INDEX16:		d3d9Format = D3DFMT_INDEX16;		break;
        case TEXTUREFORMAT_TYPE_INDEX32:		d3d9Format = D3DFMT_INDEX32;		break;
        case TEXTUREFORMAT_TYPE_D24S8:			d3d9Format = D3DFMT_D24S8;			break;
        default:								assert(false);						break;
        }

        return d3d9Format;
    }

    D3DTRANSFORMSTATETYPE ConvertToD3D9Transform(TRANSFORM_TYPE type)
    {
        D3DTRANSFORMSTATETYPE d3d9Transform;

        switch (type)
        {
        case TRANSFORM_TYPE_VIEW:		d3d9Transform = D3DTS_VIEW;			break;
        case TRANSFORM_TYPE_PROJECTION: d3d9Transform = D3DTS_PROJECTION;	break;
        case TRANSFORM_TYPE_WORLD:      d3d9Transform = D3DTS_WORLD;		break;
        default:						assert(false);						break;
        }

        return d3d9Transform;
    }

    unsigned long ConvertToD3D9Usage(unsigned long usage)
    {
        unsigned long d3dUsage = 0;

        if (usage & USAGE_TYPE_DYNAMIC)
            d3dUsage |= D3DUSAGE_DYNAMIC;
        if (usage & USAGE_TYPE_WRITEONLY)
            d3dUsage |= D3DUSAGE_WRITEONLY;
        if (usage & USAGE_TYPE_RENDERTARGET)
            d3dUsage |= D3DUSAGE_RENDERTARGET;
        if (usage & USAGE_TYPE_AUTOGENMIPMAP)
            d3dUsage |= D3DUSAGE_AUTOGENMIPMAP;

        assert(d3dUsage != 0);

        return d3dUsage;
    }

    unsigned long ConvertToD3D9Lock(unsigned long usage)
    {
        unsigned long d3dLock = 0;

        if (usage & LOCK_TYPE_WRITEDISCARD)
            d3dLock |= D3DLOCK_DISCARD;
        if (usage & LOCK_TYPE_READ)
            d3dLock |= D3DLOCK_READONLY;

        return d3dLock;
    }

    unsigned long ConvertToD3D9ClearType(unsigned long type)
    {
        unsigned long d3dClear = 0;

        if (type & CLEAR_TYPE_STENCIL)
            d3dClear |= D3DCLEAR_STENCIL;
        if (type & CLEAR_TYPE_TARGET)
            d3dClear |= D3DCLEAR_TARGET;
        if (type & CLEAR_TYPE_ZBUFFER)
            d3dClear |= D3DCLEAR_ZBUFFER;

        return d3dClear;
    }



    ERROR_TYPE convertFromD3D9ErrorType(unsigned long d3d9Error)
    {
        ERROR_TYPE error;

        switch (d3d9Error)
        {
        case S_OK:							error = ERROR_TYPE_OK;						break;
        case D3DERR_DEVICELOST:				error = ERROR_TYPE_DEVICELOST;				break;
        case D3DERR_DRIVERINTERNALERROR:	error = ERROR_TYPE_DRIVERINTERNALERROR;		break;
        case D3DERR_DEVICENOTRESET:			error = ERROR_TYPE_DEVICENOTRESET;			break;
        default:							assert(false);								break;
        }

        return error;
    }

    D3DXMATRIX convertToD3D9Matrix(const Matrix4x4& m)
    {
        D3DXMATRIX mtx;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                mtx(i, j) = m[j][i];

        return mtx;
    }


    Matrix4x4 convertFromD3D9Matrix(const D3DXMATRIX& m)
    {
        Matrix4x4 mtx;

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                mtx[j][i] = m(i, j);

        return mtx;
    }
} // env

#endif // D3D9