
#ifdef ENV_PLATFORM_WINDOWS

#include <Graphic/Core/RenderWindow.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CRenderWindow::CRenderWindow()
        : m_windowHandle(0)
        , m_bWindowed(true) {}

    //-----------------------------------------------------------------------------------
    CRenderWindow::~CRenderWindow()
    {
        DestroyWindow( m_windowHandle );
    }


//     void CRenderWindow::SetWindowMode(WINDOW_MODE mode)
//     {
//         if (mode == WINDOW_MODE_WINDOWED_FULLSCREEN)
//         {
//             unsigned long dwStyle = WS_VISIBLE | WS_POPUP;
// 
//             SetWindowLong(m_windowHandle, GWL_STYLE, dwStyle);
//             SetWindowPos(m_windowHandle, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOR)
//         }
//         else if(mode == WINDOW_MODE_WINDOWED)
//         {
//             unsigned long dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
// 
//             SetWindowLong(m_windowHandle, GWL_STYLE, dwStyle);
//         }
//     }


    //-----------------------------------------------------------------------------------
    unsigned long CRenderWindow::GetWindowStyle(bool bWindowed)
    {
        return GetWindowLong(m_windowHandle, GWL_STYLE);
    }

    //-----------------------------------------------------------------------------------
    void CRenderWindow::CenterWindow(int width, int height)
    {
        if (!m_bWindowed)
            return;

        long posX = 0;
        long posY = 0;

        GetCenterWindowPos(posX, posY, width, height);

        SetWindowPos(m_windowHandle, HWND_TOP, posX, 0, 0, 0, SWP_NOSIZE);
    }

    //-----------------------------------------------------------------------------------
    void CRenderWindow::GetCenterWindowPos(long& x, long& y, int width, int height)
    {
        RECT r = { 0, 0, width, height };
        RECT rDesktop;

        unsigned long dwStyle = GetWindowStyle(true);

        AdjustWindowRect(&r, dwStyle, FALSE);

        GetWindowRect(GetDesktopWindow(), &rDesktop);
        GetWindowRect(m_windowHandle, &r);

        long desktopWidth = rDesktop.right - rDesktop.left;
        long desktopHeight = rDesktop.bottom - rDesktop.top;

        long screenWidth = r.right - r.left;
        long screenHeight = r.bottom - r.top;

        x = (desktopWidth - screenWidth) / 2;
        y = (desktopHeight - screenHeight) / 2;
    }

    //-----------------------------------------------------------------------------------
    bool CRenderWindow::Create(HINSTANCE hInst, int width, int height, bool bWindowed, WindowMsgProc wndMsgProc)
    {
        m_bWindowed = bWindowed;

        WNDCLASS wc = { 0, (WNDPROC)wndMsgProc, 0, 0, hInst,
            LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
            (HBRUSH)GetStockObject(BLACK_BRUSH), 0, "EngineWindow" };
        RegisterClass(&wc);


        unsigned long dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX); 

        RECT r = { 0, 0, width, height };
        AdjustWindowRect(&r, dwStyle, FALSE);

        long windowPosX = 0;
        long windowPosY = 0;
        long windowWidth = r.right - r.left;
        long windowHeight = r.bottom - r.top;

        if (bWindowed)
        {
            GetCenterWindowPos(windowPosX, windowPosY, windowWidth, windowHeight);
        }

        m_windowHandle = CreateWindow("EngineWindow", "One Watcher", dwStyle, windowPosX, 0, windowWidth, windowHeight, 0, 0, hInst, 0);

        if(!m_windowHandle)
        {
            ENV_LOG_ERROR( "CREATING TARGET WINDOW FAILED!" );
            return false;
        }

        CenterWindow(windowWidth, windowHeight);
        return true;
    }

    //-----------------------------------------------------------------------------------
    void CRenderWindow::Show(bool bShow)
    {
#ifdef ENV_PLATFORM_WINDOWS
        if (bShow)
            ShowWindow(m_windowHandle, SW_SHOW);
        else
            ShowWindow(m_windowHandle, SW_HIDE);
#endif // ENV_PLATFORM_WINDOWS
    }

    //-----------------------------------------------------------------------------------
    void CRenderWindow::GetClientSize(const WindowHandle& windowHandle, int& width, int& height)
    {
        RECT WindowRect = { 0 };
        GetClientRect(windowHandle, &WindowRect);

        width  = static_cast<int>(WindowRect.right - WindowRect.left);
        height = static_cast<int>(WindowRect.bottom - WindowRect.top);
    }

} // env
#endif // ENV_PLATFORM_WINDOWS