#ifdef ENV_GRAPHIC_API_D3D11

#include <Graphic/D3D11/D3D11GPUProfiler.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CGPUProfiler::CGPUProfiler(const std::string& strGroup)
        : CProfiler(strGroup)
        , m_pDevice(nullptr)
        , m_pDeviceContext(nullptr)
        , m_pQueryBegin(nullptr)
        , m_pQueryEnd(nullptr)
        , m_pQueryDisjoint(nullptr)
        , m_bProfiling(false)
    {
        m_pDevice = nullptr;
        m_pDeviceContext = nullptr;
        ENV_ASSERT_DBG(false);
    }

    //-----------------------------------------------------------------------------------
    CGPUProfiler::~CGPUProfiler()
    {
        End();

        ENV_RELEASE(m_pQueryDisjoint);
        ENV_RELEASE(m_pQueryEnd);
        ENV_RELEASE(m_pQueryBegin);
    }

    //-----------------------------------------------------------------------------------
    void CGPUProfiler::Begin(const std::string& strName)
    {
        CProfilingMgr& profilingMgr = CProfilingMgr::GetInstance();

        m_bProfiling = profilingMgr.GetProfiling(m_strGroup);

        if (!m_bProfiling)
            return;
        
        m_pDevice = gRenderSystem->GetRenderDevice()->GetDevice();
        m_pDeviceContext = gRenderSystem->GetRenderDevice()->GetDeviceContext();

        profilingMgr.PushProfiler(m_strGroup, strName);

        D3D11_QUERY_DESC queryTimestampDesc;
        queryTimestampDesc.Query = D3D11_QUERY_TIMESTAMP;
        queryTimestampDesc.MiscFlags = 0;

        D3D11_QUERY_DESC queryDisjointDesc;
        queryDisjointDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
        queryDisjointDesc.MiscFlags = 0;


        ENV_DXCALL(m_pDevice->CreateQuery(&queryTimestampDesc, &m_pQueryBegin));
        ENV_DXCALL(m_pDevice->CreateQuery(&queryTimestampDesc, &m_pQueryEnd));
        ENV_DXCALL(m_pDevice->CreateQuery(&queryDisjointDesc, &m_pQueryDisjoint));

        // Flush the pipeline to avoid sideeffects due to previous rendering operations
        Flush();
        //m_beginTimestamp = CPUClock::GetTimestamp();

        m_pDeviceContext->Begin(m_pQueryDisjoint);
        m_pDeviceContext->End(m_pQueryBegin);

        m_strName = strName;
    }

    //-----------------------------------------------------------------------------------
    void CGPUProfiler::Flush()
    {
        D3D11_QUERY_DESC queryFlushDesc;
        queryFlushDesc.Query = D3D11_QUERY_EVENT;
        queryFlushDesc.MiscFlags = 0;

        ID3D11Query* pFlushQuery = nullptr;

        ENV_DXCALL(m_pDevice->CreateQuery(&queryFlushDesc, &pFlushQuery));

        m_pDeviceContext->Flush();
        m_pDeviceContext->End(pFlushQuery);

        BOOL flushFinished = FALSE;

        while (S_OK != m_pDeviceContext->GetData(pFlushQuery, &flushFinished, sizeof(BOOL), 0));

        ENV_RELEASE(pFlushQuery);
    }

    //-----------------------------------------------------------------------------------
    void CGPUProfiler::End()
    {
        if (!m_bProfiling)
            return;

        // Flush the pipeline to make sure all calls have actually been processed
        Flush();

        // Get timestamps
        m_pDeviceContext->End(m_pQueryEnd);
        m_pDeviceContext->End(m_pQueryDisjoint);

        D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointTimestampData;
        UINT64 beginTimestamp = 0, endTimestamp = 0;

        while (S_OK != m_pDeviceContext->GetData(m_pQueryBegin, &beginTimestamp, sizeof(UINT64), 0));
        while (S_OK != m_pDeviceContext->GetData(m_pQueryEnd, &endTimestamp, sizeof(UINT64), 0));
        while (S_OK != m_pDeviceContext->GetData(m_pQueryDisjoint, &disjointTimestampData, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT), 0));

        // in ms
        double diff = double(endTimestamp - beginTimestamp) / double(disjointTimestampData.Frequency) * 1000.0;



        //QueryPerformanceCounter((LARGE_INTEGER*)&m_stop);

        //m_time = (float)((m_stop - m_start) / (float)m_freq) * 1000.0f;

        ENV_RELEASE(m_pQueryDisjoint);
        ENV_RELEASE(m_pQueryEnd);
        ENV_RELEASE(m_pQueryBegin);

        m_time = (float)diff;

        CProfilingMgr::GetInstance().PopProfiler(m_strGroup, m_time);

        m_bProfiling = false;

        m_strName.clear();
    }
} // env


#endif // ENV_GRAPHIC_API_D3D11
