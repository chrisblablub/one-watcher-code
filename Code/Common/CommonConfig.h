/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#define _WIN32_WINNT 0x0600

#include <Common/Core/Macros.h>

#ifdef ENV_PLATFORM_WINDOWS
#include <windows.h>
#endif // ENV_PLATFORM_WINDOWS

#include <Common/Math/MathConfig.h>

#ifdef ENV_PLATFORM_ORBIS

struct RECT
{
    long left;
    long top;
    long right;
    long bottom;
};

typedef int HWND;
typedef int HINSTANCE;
typedef int WPARAM;
typedef int LPARAM;
typedef int LRESULT;
typedef unsigned int UINT;
typedef bool(*WNDPROC)(HWND, unsigned int, WPARAM, LPARAM);
typedef void* HANDLE;

#define VK_CONTROL 0x11
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87
#define VK_SHIFT          0x10
#define VK_DELETE         0x2E
#define VK_SPACE          0x20
#define VK_RETURN         0x0D
#define VK_ESCAPE         0x1B
#define VK_CAPITAL        0x14
#define CALLBACK

#endif // ENV_PLATFORM_ORBIS

typedef HWND        WindowHandle;
typedef WNDPROC     WindowMsgProc;
typedef HINSTANCE   InstanceHandle;
typedef HANDLE      Handle;

#endif // COMMON_CONFIG_H