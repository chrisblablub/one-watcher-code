#include <Graphic/Renderer/PostProcessRenderer.h>
#include <Graphic/Renderer/FullscreenTextureRenderer.h>
#include <Graphic/Core/RenderDevice.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/Core/Texture.h>
#include <Graphic/Core/VertexDeclarations.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CPostProcessRenderer::CPostProcessRenderer(CRenderDevice* pDevice, CFullscreenTextureRenderer* pFullscreenRenderer)
        : m_pDevice(pDevice)
        , m_pFullscreenRenderer(pFullscreenRenderer)
        , m_pCartoonShader(nullptr)
        , m_pWaterNMap(nullptr)
        , m_pCombs(nullptr) {}

    //-----------------------------------------------------------------------------------
    CPostProcessRenderer::~CPostProcessRenderer()
    {
        gShaderMgr->Release(m_pPostProcessShader);
        gShaderMgr->Release(m_pCartoonShader);

        gTextureMgr->Release(m_pCombs);
        gTextureMgr->Release(m_pWaterNMap);
    }


    //-----------------------------------------------------------------------------------
    bool CPostProcessRenderer::Initialize()
    {
        m_pCartoonShader = gShaderMgr->AddGetPtr("Cartoon");
        m_pPostProcessShader = gShaderMgr->AddGetPtr("PostProcess");

        if (!m_pCartoonShader ||
            !m_pPostProcessShader)
        {
            return false;
        }
        
        m_pCartoonShader->LinkConstantBufferCPU("cb_CartoonVS", m_constBufCartoonVS);
        m_pCartoonShader->LinkConstantBufferCPU("cb_CartoonPS", m_constBufCartoonPS);

        m_pPostProcessShader->LinkConstantBufferCPU("cb_PostProcessVS", m_constBufPostProcessVS);
        m_pPostProcessShader->LinkConstantBufferCPU("cb_PostProcessPS", m_constBufPostProcessPS);

        m_pPostProcessVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pCartoonShader);

        m_screenQuad.Initialize(m_pDevice);

        m_pWaterNMap    = gTextureMgr->AddGetPtr("water_nmap01.png");
        m_pCombs        = gTextureMgr->AddGetPtr("Combs.png");

        return m_pCartoonShader && m_pPostProcessVertexDecl && m_pWaterNMap && m_pCombs;
    }

    //-----------------------------------------------------------------------------------
    void CPostProcessRenderer::RenderCartoon(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);

        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILWRITEMASK, 0xF0);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREADMASK, 0xF0);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILREF, 1);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_NOTEQUAL);

        m_pFullscreenRenderer->Render(renderSet.m_pRenderTargetColors, renderQueue, renderSet);

        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILFUNC, COMPARISON_TYPE_EQUAL);


        Matrix4x4 colorGrading = ComputeColorGrading(renderQueue);

        if (renderQueue.m_pCharacterRenderObject)
        {
            Vec3 charPos;
            charPos = Vec3(Vec4(renderQueue.m_pCameraRenderObject->m_position, 1.0f) * renderQueue.m_pCameraRenderObject->m_mtxView);
            
            m_constBufCartoonPS.GetDataForWriting().m_charPos = charPos;

            m_constBufCartoonPS.GetDataForWriting().m_silhouetteRadiusOpacity = Vec2(renderQueue.m_postProcessRenderObject.m_silhouetteCharacterRadius,
                                                                           renderQueue.m_postProcessRenderObject.m_silhouetteCharacterOpacity);
        }
        else
        {
            m_constBufCartoonPS.GetDataForWriting().m_silhouetteRadiusOpacity = Vec2(renderQueue.m_postProcessRenderObject.m_silhouetteCharacterRadius, 0.0f);
        }

        m_constBufCartoonVS.GetDataForWriting() = renderQueue.m_pCameraRenderObject->m_mtxInverseProjection;        
        m_constBufCartoonPS.GetDataForWriting().m_texCoordDXDY.x = 1.0f / ((float)renderQueue.m_pCameraRenderObject->m_screenWidth * 1.5f);
        m_constBufCartoonPS.GetDataForWriting().m_texCoordDXDY.y = 1.0f / ((float)renderQueue.m_pCameraRenderObject->m_screenHeight * 1.5f);
        m_constBufCartoonPS.GetDataForWriting().m_fogColor = renderQueue.m_pEnvironmentRenderObject->m_fogColor;
        m_constBufCartoonPS.GetDataForWriting().m_fogRange = renderQueue.m_pEnvironmentRenderObject->m_fogRange;
        m_constBufCartoonPS.GetDataForWriting().m_fogStart = renderQueue.m_pEnvironmentRenderObject->m_fogStart;
        m_constBufCartoonPS.GetDataForWriting().m_silhouetteColor = Vec3(renderQueue.m_postProcessRenderObject.m_silhouetteColor);
        m_constBufCartoonPS.GetDataForWriting().m_silhouetteIntensity = renderQueue.m_postProcessRenderObject.m_silhouetteIntensity;

        m_pDevice->SetTextureSampler(0, renderSet.m_pCurrentColorSource->GetTextureSampler());
        m_pDevice->SetTexture(0, renderSet.m_pCurrentColorSource->GetTexture());
        m_pDevice->SetTextureSampler(1, renderSet.m_pRenderTargetDepths->GetTextureSampler());
        m_pDevice->SetTexture(1, renderSet.m_pRenderTargetDepths->GetTexture());
        m_pDevice->SetTextureSampler(2, renderSet.m_pRenderTargetNormals->GetTextureSampler());
        m_pDevice->SetTexture(2, renderSet.m_pRenderTargetNormals->GetTexture());        

        m_pDevice->SetVertexDeclaration(m_pPostProcessVertexDecl.get());
        m_pDevice->SetShader(m_pCartoonShader);
        m_screenQuad.RenderQuad();

        m_pDevice->SetTexture(0, NULL);
        m_pDevice->SetTexture(1, NULL);
    }

    //-----------------------------------------------------------------------------------
    void CPostProcessRenderer::RenderPostProcessing(RenderQueue& renderQueue, RenderSet& renderSet)
    {
        m_pDevice->SetRenderTarget(0, renderSet.m_pCurrentColorTarget);
        m_pDevice->ResetToDefaultRenderStates();
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_STENCILENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZWRITEENABLE, false);
        m_pDevice->SetRenderState(RENDERSTATE_TYPE_ZENABLE, false);


        Matrix4x4 colorGrading = ComputeColorGrading(renderQueue);

        m_constBufPostProcessVS.GetDataForWriting() = renderQueue.m_pCameraRenderObject->m_mtxInverseProjection;

        m_constBufPostProcessPS.GetDataForWriting().m_colorGrading = colorGrading;
        m_constBufPostProcessPS.GetDataForWriting().m_posterizationAmount = renderQueue.m_postProcessRenderObject.m_posterizationAmount;

        m_pDevice->SetTextureSampler(0, renderSet.m_pCurrentColorSource->GetTextureSampler());
        m_pDevice->SetTexture(0, renderSet.m_pCurrentColorSource->GetTexture());
        m_pDevice->SetTextureSampler(1, renderSet.m_pRenderTargetDepths->GetTextureSampler());
        m_pDevice->SetTexture(1, renderSet.m_pRenderTargetDepths->GetTexture());

        m_pDevice->SetTextureSampler(2, CDefaultTextureSampler::GetLinearWrapSampler());
        if (renderSet.m_pBloom)
            m_pDevice->SetTexture(2, renderSet.m_pBloom->GetTexture());
        m_pDevice->SetTextureSampler(3, CDefaultTextureSampler::GetLinearWrapSampler());
        if (renderSet.m_pLightRays)
        m_pDevice->SetTexture(3, renderSet.m_pLightRays->GetTexture());

        m_pDevice->SetVertexDeclaration(m_pPostProcessVertexDecl.get());
        m_pDevice->SetShader(m_pPostProcessShader);
        m_screenQuad.RenderQuad();
        m_pDevice->SetTexture(0, nullptr);
        m_pDevice->SetTexture(1, nullptr);
    }

    //-----------------------------------------------------------------------------------
    void CPostProcessRenderer::OnLostDevice()
    {
        m_pCartoonShader->OnLostDevice();


    }

    //-----------------------------------------------------------------------------------
    void CPostProcessRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_pCartoonShader->OnResetDevice();

        RenderCoordRect renderCoords;

#ifdef D3D9
        Vec2 pixelsToTexels = CGPUQuad::GetPixelsToTexelsOffset(screenWidth, screenHeight);

        renderCoords.m_left += pixelsToTexels.x;
        renderCoords.m_right += pixelsToTexels.x;
        renderCoords.m_top += pixelsToTexels.y;
        renderCoords.m_bottom += pixelsToTexels.y;
#endif // D3D9

        m_screenQuad.SetRenderRect(renderCoords);
        m_screenQuad.UpdateVertexBuffer();
    }

    //-----------------------------------------------------------------------------------
    Matrix4x4 CPostProcessRenderer::ComputeColorGrading(RenderQueue& renderQueue)
    {
        Vec3 m_colorStandardWeights(0.299f, 0.587f, 0.114f);
        Vec3 m_colorScaling = renderQueue.m_postProcessRenderObject.m_colorScaling;
        Vec3 m_colorOffset = renderQueue.m_postProcessRenderObject.m_colorOffset;
        float m_contrast = renderQueue.m_postProcessRenderObject.m_contrast;
        float m_sepia = renderQueue.m_postProcessRenderObject.m_sepia;
        float m_hueRotation = renderQueue.m_postProcessRenderObject.m_hueRotation;
        float m_saturation = renderQueue.m_postProcessRenderObject.m_saturation;

        Matrix4x4 mtxColorSaturation(1.0f);

        float satR = (1.0f - m_saturation) * m_colorStandardWeights.x;
        float satG = (1.0f - m_saturation) * m_colorStandardWeights.y;
        float satB = (1.0f - m_saturation) * m_colorStandardWeights.z;
        mtxColorSaturation[0][0] = satR + m_saturation;
        mtxColorSaturation[1][0] = satR;
        mtxColorSaturation[2][0] = satR;
        mtxColorSaturation[0][1] = satG;
        mtxColorSaturation[1][1] = satG + m_saturation;
        mtxColorSaturation[2][1] = satG;
        mtxColorSaturation[0][2] = satB;
        mtxColorSaturation[1][2] = satB;
        mtxColorSaturation[2][2] = satB + m_saturation;

        Matrix4x4 mtxColorScaling(1.0f);
        mtxColorScaling = glm::scale(Matrix4x4(1.0f), m_colorScaling);

        Matrix4x4 mtxColorBrightness(1.0f);
        mtxColorBrightness = glm::transpose(glm::translate(Matrix4x4(1.0f), m_colorOffset));

        Matrix4x4 mtxColorContrast(1.0f);
        float contrastTranslation = (1.0f - m_contrast) / 2.0f;
        mtxColorContrast = glm::scale(Matrix4x4(1.0f), Vec3(m_contrast, m_contrast, m_contrast));
        mtxColorContrast[0][3] = contrastTranslation;
        mtxColorContrast[1][3] = contrastTranslation;
        mtxColorContrast[2][3] = contrastTranslation;


        Matrix4x4 mtxSepia(1.0f);
        mtxSepia[0] = Vec4(Vec3(0.3588f * m_sepia + 1.0f * (1.0f - m_sepia),
            0.299f * m_sepia,
            0.2392 * m_sepia), 0.0f);
        mtxSepia[1] = Vec4(Vec3(0.7044f * m_sepia,
            0.587f * m_sepia + 1.0f * (1.0f - m_sepia),
            0.4696f * m_sepia), 0.0f);
        mtxSepia[2] = Vec4(Vec3(0.1368f * m_sepia,
            0.114f * m_sepia,
            0.0913f * m_sepia + 1.0f * (1.0f - m_sepia)), 0.0f);

        mtxSepia = glm::transpose(mtxSepia);

        Matrix4x4 mtxHue(1.0f);

        Matrix4x4 mtxHue01(1.0f), mtxHue02(1.0f), mtxHue03(1.0f);
        mtxHue01[0] = Vec4(Vec3(0.213f, 0.715f, 0.072f), 0.0f);
        mtxHue01[1] = Vec4(Vec3(0.213f, 0.715f, 0.072f), 0.0f);
        mtxHue01[2] = Vec4(Vec3(0.213f, 0.715f, 0.072f), 0.0f);

        mtxHue02[0] = Vec4(Vec3(0.787f, -0.715f, -0.072f), 0.0f);
        mtxHue02[1] = Vec4(Vec3(-0.213f, 0.285f, -0.072f), 0.0f);
        mtxHue02[2] = Vec4(Vec3(-0.213f, -0.715f, 0.928f), 0.0f);

        mtxHue03[0] = Vec4(Vec3(-0.213f, -0.715f, 0.928f), 0.0f);
        mtxHue03[1] = Vec4(Vec3(0.143f, 0.140f, -0.283f), 0.0f);
        mtxHue03[2] = Vec4(Vec3(-0.787f, 0.715f, 0.072f), 0.0f);


        float radHueRotation = m_hueRotation / 360.0f * 2 * PI;
        mtxHue = mtxHue01 + cos(radHueRotation) * mtxHue02 + sin(radHueRotation) * mtxHue03;


        Matrix4x4 mtxColorTransformation = mtxColorSaturation * mtxColorScaling * mtxColorBrightness * mtxSepia * mtxHue * mtxColorContrast;

        return mtxColorTransformation;
    }
}
