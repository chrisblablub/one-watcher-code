/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#ifdef ENV_PLATFORM_WINDOWS
#pragma once
#ifndef WNDPROC_H
#define WNDPROC_H

#include <Engine/Core/Engine.h>
#include <Engine/Core/Input.h>
#include <Graphic/Core/RenderWindow.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    static bool ProcessWndMsg(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        static bool bRestored = false;

        bool bMsgProcessed = false;

        switch (msg)
        {            
        case WM_SIZE:
        {
            int nWidth = LOWORD(lParam);
            int nHeight = HIWORD(lParam);

            ENV_LOG_INFO("WM_SIZE");

            if (nWidth != 0 && nHeight != 0)
                gEngine->SetResetDeviceQueued(nWidth, nHeight);

            bMsgProcessed = true;
        }
        break;

        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* p = (MINMAXINFO*)lParam;
            bMsgProcessed = true;
        } break;

        case WM_KILLFOCUS:
            gEngine->GetInput()->Reset();
            bMsgProcessed = true;
            break;

        case WM_LBUTTONDOWN:
            gEngine->GetInput()->MouseDownEvent(0);
            bMsgProcessed = true;
            break;

        case WM_RBUTTONDOWN:
            gEngine->GetInput()->MouseDownEvent(1);
            bMsgProcessed = true;
            break;

        case WM_MBUTTONDOWN:
            gEngine->GetInput()->MouseDownEvent(2);
            bMsgProcessed = true;
            break;

        case WM_LBUTTONUP:
            gEngine->GetInput()->MouseUpEvent(0);
            bMsgProcessed = true;
            break;

        case WM_RBUTTONUP:
            gEngine->GetInput()->MouseUpEvent(1);
            bMsgProcessed = true;
            break;

        case WM_MBUTTONUP:
            gEngine->GetInput()->MouseUpEvent(2);
            bMsgProcessed = true;
            break;

        case WM_KEYUP:
            gEngine->GetInput()->KeyUpEvent(wParam);
            bMsgProcessed = true;
            break;

        case WM_KEYDOWN:
            gEngine->GetInput()->KeyDownEvent(wParam);
            bMsgProcessed = true;
            break;

        case WM_MOUSEWHEEL:
            gEngine->GetInput()->ScrollEvent(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
            bMsgProcessed = true;
            break;

        case WM_DESTROY:
            gEngine->ShutDown();
            bMsgProcessed = true;
            break;
        }

        return bMsgProcessed;
    }

    //-----------------------------------------------------------------------------------
    static LRESULT CALLBACK WndMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (ProcessWndMsg(msg, wParam, lParam))
            return 0;

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

} // env

#endif // WNDPROC_H
#endif // ENV_PLATFORM_WINDOWS