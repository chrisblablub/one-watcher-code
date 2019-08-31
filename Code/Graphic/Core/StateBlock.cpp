
#include <Graphic/Core/StateBlock.h>
#include <Graphic/Core/RenderDevice.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CStateBlock::CStateBlock()
        : m_pDevice(NULL) {}

    //-----------------------------------------------------------------------------------
    void CStateBlock::SetNumStates(CRenderDevice* pDevice, int numRS, int numSS, int numTS)
    {
        m_renderStates.reserve(numRS);
        m_samplerStates.reserve(numSS);

        m_pDevice = pDevice;
    }

    //-----------------------------------------------------------------------------------
    void CStateBlock::OnResetDevice()
    {
        if (!m_pDevice)
            return;

        for (int i = 0; i < m_renderStates.size(); ++i)
            m_pDevice->SetRenderState(m_renderStates[i].first, m_renderStates[i].second);

        for (int i = 0; i < m_samplerStates.size(); ++i)
            m_pDevice->SetSamplerState(std::get<0>(m_samplerStates[i]),
            std::get<1>(m_samplerStates[i]),
            std::get<2>(m_samplerStates[i]));
    }

    //-----------------------------------------------------------------------------------
    void CStateBlock::Apply()
    {
        for (int i = 0; i < m_renderStates.size(); ++i)
            m_pDevice->SetRenderState(m_renderStates[i].first, m_renderStates[i].second);

        for (int i = 0; i < m_samplerStates.size(); ++i)
            m_pDevice->SetSamplerState(std::get<0>(m_samplerStates[i]),
            std::get<1>(m_samplerStates[i]),
            std::get<2>(m_samplerStates[i]));
    }

} // env