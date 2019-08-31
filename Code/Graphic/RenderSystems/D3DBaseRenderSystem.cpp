#include <Graphic/RenderSystems/D3DBaseRenderSystem.h>
#include <Graphic/Core/RenderDevice.h>
#include <vector>

namespace env
{

    //-----------------------------------------------------------------------------------
    CBaseRenderSystem::CBaseRenderSystem()
        : m_pRenderDevice(nullptr) {}

    //-----------------------------------------------------------------------------------
    CBaseRenderSystem::~CBaseRenderSystem()
    {
        ENV_DELETE(m_pRenderDevice);

        ENV_LOG_INFO("BaseRenderSystem destructed.");
    }

    //-----------------------------------------------------------------------------------
    bool CBaseRenderSystem::Initialize(const RenderQualityParams& videoParams, const RenderSystemParams& renderSysParams)
    {
        ENV_LOG_INFO("INITIALIZING BASERENDERSYSTEM");

        m_renderQualityParams = videoParams;
        m_renderSysParams = renderSysParams;

        if (!CreateDevice())
            return false;

        if (!CheckDeviceSettings())
            return false;


        ENV_LOG_INFO("INITIALIZING BASERENDERSYSTEM SUCCEEDED");

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CBaseRenderSystem::InitializeRenderers()
    {
        ENV_LOG_INFO("INITIALIZING RENDERERS");

        for (mapRenderers::iterator it = m_renderers.begin(); it != m_renderers.end(); ++it)
        {
            if (!it->second->Initialize())
            {
                ENV_LOG_ERROR("COULD NOT INITIALIZE RENDERER: " + it->first);
                return false;
            }
            else
            {
                ENV_LOG_INFO("INITIALIZED RENDERER: " + it->first);
            }
        }

        ENV_LOG_INFO("INITIALIZING RENDERERS SUCCEEDED");

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CBaseRenderSystem::ResetDevice(const RenderQualityParams& videoParams)
    {
        m_renderQualityParams = videoParams;

        if (!CheckDeviceSettings())
            return false;

        m_pRenderDevice->Reset(m_renderQualityParams.screenWidth,
                               m_renderQualityParams.screenHeight, 
                               m_renderQualityParams.bVSync, 
                               m_renderQualityParams.bWindowed);



        ENV_LOG_INFO("Device reset.");

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CBaseRenderSystem::ResetDevice()
    {
        ResetDevice(m_renderQualityParams);

        ENV_LOG_INFO("Device reset.");

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CBaseRenderSystem::ReadDeviceSettings()
    {
        ENV_LOG_INFO("READING DEVICE SETTINGS");
        ENV_LOG_INFO("READING DEVICE SETTINGS SUCCEEDED");

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CBaseRenderSystem::CheckDeviceSettings()
    {
        return m_pRenderDevice->CheckDeviceSettings();
    }

    //-----------------------------------------------------------------------------------
    bool CBaseRenderSystem::CreateDevice()
    {
        m_pRenderDevice = new CRenderDevice();

        return m_pRenderDevice->Initialize(m_renderSysParams,
                                           m_renderQualityParams.screenWidth,
                                           m_renderQualityParams.screenHeight,
                                           m_renderQualityParams.Adapter,
                                           m_renderQualityParams.bVSync, 
                                           m_renderQualityParams.bWindowed);
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::AddRenderer(const std::string& strName, CBaseRenderer* pRenderer)
    {
        m_renderers[strName] = pRenderer;
    }

    //-----------------------------------------------------------------------------------
    CBaseRenderer* CBaseRenderSystem::GetRenderer(const std::string& strName)
    {
        mapRenderers::iterator it = m_renderers.find(strName);
        if (it == m_renderers.end())
            return NULL;
        else
            return it->second;
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::RemoveRenderer(const std::string& strName)
    {
        m_renderers.erase(strName);
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::OnLevelChanged()
    {
        for (mapRenderers::iterator it = m_renderers.begin(); it != m_renderers.end(); ++it)
            it->second->OnLevelChanged();
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::SetRenderQueue(RenderQueue* pQueue)
    {
        m_pRenderQueue = pQueue;
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::OnLostDevice()
    {
        for (mapRenderers::iterator it = m_renderers.begin(); it != m_renderers.end(); ++it)
            it->second->OnLostDevice();

        m_pRenderDevice->OnLostDevice();
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::OnResetDevice()
    {
        for (mapRenderers::iterator it = m_renderers.begin(); it != m_renderers.end(); ++it)
        {
            // ENV_LOG_INFO("OnResetDevice: " + it->first);

            it->second->OnResetDevice(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight);

            // ENV_LOG_INFO("OnResetDevice: " + it->first + " SUCCEEDED");
        }

        m_pRenderDevice->OnResetDevice(m_renderQualityParams.screenWidth, m_renderQualityParams.screenHeight);
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::BeginScene()
    {
        m_pRenderDevice->BeginScene();
    }

    //-----------------------------------------------------------------------------------
    void CBaseRenderSystem::EndScene()
    {
        m_pRenderDevice->EndScene();
    }

} // env