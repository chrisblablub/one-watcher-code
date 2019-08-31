#include <Graphic/Renderer/GaussBlurRenderer.h>
#include <Graphic/Core/Quad.h>
#include <Graphic/GraphicConfig.h>
#include <Graphic/Core/VertexDeclarations.h>
#include <Graphic/Core/RenderDevice.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CGaussBlurRenderer::CGaussBlurRenderer(CRenderDevice* pDevice)
        : m_pGaussBlurShader(nullptr)
        , m_pDevice(pDevice) {}

    //-----------------------------------------------------------------------------------
    CGaussBlurRenderer::~CGaussBlurRenderer()
    {
        gShaderMgr->Release(m_pGaussBlurShader);
    }

    //-----------------------------------------------------------------------------------
    bool CGaussBlurRenderer::Initialize()
    {
        m_quad.Initialize(m_pDevice);

        m_pGaussBlurShader = gShaderMgr->AddGetPtr("GaussBlur");

        if (!m_pGaussBlurShader)
            return false;

        m_pGaussBlurShader->LinkConstantBufferCPU("cb_Samples", m_constBufSamples);

        m_pVertexDecl = m_pDevice->CreateVertexDeclaration(PosTexVertex::VertexElements, sizeof(PosTexVertex), m_pGaussBlurShader);

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CGaussBlurRenderer::Render(int targetWidth, int targetHeight, float intensity)
    {
        ENV_SCOPED_GPU_PROFILING(Rendering, GaussBlur);

        std::vector< Vec4 > avSampleOffsets;
        std::vector< Vec4 > avSampleWeights;
        avSampleOffsets.resize(16);
        avSampleWeights.resize(16);

        ComputeGaussBlurSamples((unsigned long)(targetWidth),
                                (unsigned long)(targetHeight), avSampleOffsets, avSampleWeights, intensity);

        memcpy(m_constBufSamples.GetDataForWriting().m_sampleOffsets, &avSampleOffsets[0], sizeof(m_constBufSamples.GetDataForWriting().m_sampleOffsets));
        memcpy(m_constBufSamples.GetDataForWriting().m_sampleWeights, &avSampleWeights[0], sizeof(m_constBufSamples.GetDataForWriting().m_sampleWeights));

        m_pDevice->SetVertexDeclaration(m_pVertexDecl.get());
        m_pDevice->SetShader(m_pGaussBlurShader);
        m_quad.RenderQuad();
    }

    //-----------------------------------------------------------------------------------
    void CGaussBlurRenderer::OnLostDevice()
    {
        m_pGaussBlurShader->OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CGaussBlurRenderer::OnResetDevice(int screenWidth, int screenHeight)
    {
        m_pGaussBlurShader->OnResetDevice();
    }

    //-----------------------------------------------------------------------------------
    float CGaussBlurRenderer::ComputeGaussianDistribution(float x, float y, float rho)
    {
        float g = 1.0f / sqrtf(2.0f * PI * rho * rho);
        g *= expf(-(x*x + y*y) / (2 * rho*rho));

        return g;
    }

    //-----------------------------------------------------------------------------------
    void CGaussBlurRenderer::ComputeGaussBlurSamples(unsigned long dwD3DTexWidth,
                                                     unsigned long dwD3DTexHeight,
                                                     std::vector<Vec4>& avTexCoordOffset,
                                                     std::vector<Vec4>& avSampleWeight,
                                                     float fMultiplier)
    {
        float s = 1.0f / fMultiplier;
        float tu = 1.0f / (float)(dwD3DTexWidth * s);
        float tv = 1.0f / (float)(dwD3DTexHeight * s);

        Vec4 vWhite(1.0f, 1.0f, 1.0f, 1.0f);

        float totalWeight = 0.0f;
        int index = 0;
        for (int x = -2; x <= 2; x++)
        {
            for (int y = -2; y <= 2; y++)
            {
                // Exclude pixels with a block distance greater than 2. This will
                // create a kernel which approximates a 5x5 kernel using only 13
                // sample points instead of 25; this is necessary since 2.0 shaders
                // only support 16 texture grabs.
                if (abs(x) + abs(y) > 2)
                    continue;

                // Get the unscaled Gaussian intensity for this offset
                avTexCoordOffset[index] = Vec4(x * tu, y * tv, 0.0f, 0.0f);
                avSampleWeight[index] = vWhite * ComputeGaussianDistribution((float)x, (float)y, 1.0f);
                totalWeight += avSampleWeight[index].x;

                index++;
            }
        }

        // Divide the current weight by the total weight of all the samples; Gaussian
        // blur kernels add to 1.0f to ensure that the intensity of the image isn't
        // changed when the blur occurs. An optional multiplier variable is used to
        // add or remove image intensity during the blur.
        for (int i = 0; i < index; i++)
        {
            avSampleWeight[i] /= totalWeight;
        }
    }

} // env