/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#ifdef ENV_PLATFORM_WINDOWS

#pragma once
#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include <Graphic/GraphicCommon.h>

namespace env
{
    class CRenderWindow
    {
    public:
        CRenderWindow();
        ~CRenderWindow();

        bool Create(HINSTANCE hInst, int width, int height, bool bWindowed, WindowMsgProc wndMsgProc);

        WindowHandle GetHandle() const { return m_windowHandle; }

        void CenterWindow(int width, int height);
//      void SetWindowMode(WINDOW_MODE mode);
        inline bool GetWindowed() const { return m_bWindowed; }

        void GetCenterWindowPos(long& x, long& y, int width, int height);

        void Show(bool bShow = true);

        static void GetClientSize(const WindowHandle& windowHandle, int& width, int& height);

    private:
        unsigned long GetWindowStyle(bool bWindowed);

    private:
        WindowHandle m_windowHandle;
        bool m_bWindowed;

        int m_windowWidth;
        int m_windowHeight;
    };
} // env


#endif // RENDER_WINDOW_H
#endif // ENV_PLATFORM_WINDOWS