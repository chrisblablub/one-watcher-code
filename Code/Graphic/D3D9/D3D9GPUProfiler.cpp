#ifdef D3D9

#include <Graphic/D3D9/D3D9GPUProfiler.h>
#include <Graphic/RenderSystems/RenderSystem.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CD3D9GPUProfiler::CD3D9GPUProfiler(const std::string& strGroup)
        : CProfiler(strGroup)
        , m_pDevice(nullptr)
        , m_pEvent(nullptr)
        , m_time(0.0f)
        , m_start(0)
        , m_stop(0)
        , m_freq(0)
        , m_bProfiling(false)
    {
        m_pDevice = gRenderSystem->GetRenderDevice()->GetDevice();
    }

    //-----------------------------------------------------------------------------------
    CD3D9GPUProfiler::~CD3D9GPUProfiler()
    {
        End();

        ENV_RELEASE(m_pEvent);
    }

    //-----------------------------------------------------------------------------------
    void CD3D9GPUProfiler::Begin(const std::string& strName)
    {
//         CProfilingMgrSingleton profiler;
// 
//         m_bProfiling = (profiler->GetFrameProfilingGroup() == m_strGroup);
// 
//         if (!m_bProfiling)
//             return;
// 
//         ENV_DXCALL(m_pDevice->CreateQuery(D3DQUERYTYPE_EVENT, &m_pEvent));
//         ENV_DXCALL(m_pEvent->Issue(D3DISSUE_END));
// 
//         while (S_OK != m_pEvent->GetData(NULL, 0, D3DGETDATA_FLUSH));
// 
//         // Now start counting because the video card is ready
//         QueryPerformanceFrequency((LARGE_INTEGER*)&m_freq);
//         QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
// 
//         m_strName = strName;
    }

    //-----------------------------------------------------------------------------------
    void CD3D9GPUProfiler::End()
    {
//         gRenderSystem->GetRenderDevice()->EndPerformanceMarker();
// 
//         if (!m_bProfiling)
//             return;
// 
//         ENV_DXCALL(m_pEvent->Issue(D3DISSUE_END));
// 
//         while (S_FALSE == m_pEvent->GetData(NULL, 0, D3DGETDATA_FLUSH));
// 
//         ENV_RELEASE(m_pEvent);
// 
//         QueryPerformanceCounter((LARGE_INTEGER*)&m_stop);
// 
//         m_time = (float)((m_stop - m_start) / (float)m_freq) * 1000.0f;
// 
//         CProfilingMgrSingleton profiler;
// 
//         profiler->SetProfilingResult(m_strGroup, m_strName, m_time);
// 
//         m_bProfiling = false;
// 
//         m_strName.clear();
    }
} // env

#endif // D3D9