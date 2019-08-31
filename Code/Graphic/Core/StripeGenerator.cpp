
#include <Graphic/Core/StripeGenerator.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    void CDefaultStripeGenerator::GetStripeDistributionParams(unsigned int stripeIndex, 
                                                              const StripeParameters& settings,
                                                              float& topCoordX, float& botCoordX,
                                                              float& topCoordY, float& botCoordY,
                                                              float& width) const
    {
        const float angle = 45.0f;

        // Convert angle from deg to rad
        float radAngle = angle / 180.0f * PI;

        // Stripes are craeted in NDC [-1, 1]
        float xStripeOffset = tan(radAngle) * 2.0f * m_settings.m_aspectRatio;

        float xStripeMin = -1.0f - xStripeOffset;
        float xStripeMax = 1.0f + xStripeOffset;
        float yStripeMin = -1.0f;
        float yStripeMax = 1.0f;

        float xStartInterp = static_cast<float>(stripeIndex) / m_settings.m_numSlices;
        float xEndInterp = static_cast<float>(stripeIndex + 1) / m_settings.m_numSlices;
        float xMidInterp = xStartInterp + (xEndInterp - xStartInterp) * 0.5f;

        topCoordX = Lerp(-1.0f, xStripeMax, xMidInterp);
        botCoordX = Lerp(xStripeMin, 1.0f, xMidInterp);

        topCoordY = yStripeMax;
        botCoordY = yStripeMin;

        width = (xStripeMax + 1.0f) / settings.m_numSlices * 0.5f;
    }

    //-----------------------------------------------------------------------------------
    float CDefaultStripeGenerator::Generate(vecStripes& stripes) const
    {
        stripes.clear();
        stripes.resize(m_settings.m_numSlices);

        if (stripes.empty())
            return 0.0f;

        for (size_t i = 0; i < stripes.size(); ++i)
        {
            Stripe& stripe = stripes[i];

            float topCoordX = 0.0f, botCoordX = 0.0f;
            float topCoordY = 0.0f, botCoordY = 0.0f;
            float width = 0.0f;

            GetStripeDistributionParams(i, m_settings, topCoordX, botCoordX, topCoordY, botCoordY, width);

            // top center
            stripe.m_startTopPos.x = topCoordX;
            stripe.m_startTopPos.y = topCoordY;
            stripe.m_endTopPos = stripe.m_startTopPos;

            // bot center
            stripe.m_startBotPos = stripe.m_startTopPos;
            stripe.m_endBotPos.x = botCoordX;
            stripe.m_endBotPos.y = botCoordY;

            if (!m_settings.m_bGrowVertically)
                stripe.m_startBotPos = stripe.m_endBotPos;

            if (m_settings.m_bGrowAlternatingly && i % 2 != 0)
            {
                stripe.m_startBotPos = stripe.m_endBotPos;
                stripe.m_endTopPos = stripe.m_startTopPos;
                stripe.m_startTopPos = stripe.m_startBotPos;
            }

            // start + end width
            stripe.m_startWidth = m_settings.m_startWidth * width;
            stripe.m_endWidth = m_settings.m_endWidth * width;

            stripe.m_startTopColor = m_settings.m_startTopColor;
            stripe.m_startBotColor = m_settings.m_startBotColor;
            stripe.m_endTopColor = m_settings.m_endTopColor;
            stripe.m_endBotColor = m_settings.m_endBotColor;

//             float random = 0.0025f;
//             stripe.m_startBotColor = Clamp(m_settings.m_startBotColor + Vec4(1.0f, 1.0f, 1.0f, 0.0f) * GetRandomFloat(-random, random), Vec4(0, 0, 0, 0), Vec4(1, 1, 1, 1));
//             stripe.m_startTopColor = Clamp(m_settings.m_startTopColor + Vec4(1.0f, 1.0f, 1.0f, 0.0f) * GetRandomFloat(-random, random), Vec4(0), Vec4(1));
//             stripe.m_endBotColor = Clamp(m_settings.m_endBotColor + Vec4(1.0f, 1.0f, 1.0f, 0.0f) * GetRandomFloat(-random, random), Vec4(0), Vec4(1));
//             stripe.m_endTopColor = Clamp(m_settings.m_endTopColor + Vec4(1.0f, 1.0f, 1.0f, 0.0f) * GetRandomFloat(-random, random), Vec4(0), Vec4(1));

            if (m_settings.m_bFadeOut)
            {
                std::swap(stripe.m_startWidth, stripe.m_endWidth);
                std::swap(stripe.m_startTopPos, stripe.m_endTopPos);
                std::swap(stripe.m_startBotPos, stripe.m_endBotPos);
                std::swap(stripe.m_startBotColor, stripe.m_endBotColor);
                std::swap(stripe.m_startTopColor, stripe.m_endTopColor);
            }

            // time + life time
            stripe.m_timeSpawnStart = m_settings.m_timeSpawnOffset * i;
            stripe.m_timeSpawnDuration = m_settings.m_timeSpawnDuration;
        }

        Stripe& lastStripe = stripes[stripes.size() - 1];
        return lastStripe.m_timeSpawnStart + lastStripe.m_timeSpawnDuration;
    }

    //-----------------------------------------------------------------------------------
    float CStripeManager::Generate(const CStripeGenerator& generator)
    {
        return generator.Generate(m_stripeRenderObject.m_stripes);
    }

    //-----------------------------------------------------------------------------------
    void CStripeManager::AddToRenderQueue(RenderQueue& renderQueue)
    {
        renderQueue.m_stripeRenderObject = m_stripeRenderObject;
    }
} // env