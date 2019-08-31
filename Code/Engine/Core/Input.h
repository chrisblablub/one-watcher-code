/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef INPUT_H
#define INPUT_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/InputListener.h>

#ifdef ENV_PLATFORM_WINDOWS
#include "xinput.h"
#endif // ENV_PLATFORM_WINDOWS

namespace env
{
    enum KEY_STATUS
    {
        KEY_STATUS_DOWN,
        KEY_STATUS_JUST_DOWN,
        KEY_STATUS_JUST_UP,
        KEY_STATUS_WHEEL,
        KEY_STATUS_EVENT
    };

    enum ACTION
    {
        ACTION_LEFT = 0,
        ACTION_RIGHT,
        ACTION_UP,
        ACTION_DOWN,
        ACTION_PRIMARY,
        ACTION_SECONDARY,
        ACTION_START,
        ACTION_BACK,
        ACTION_END
    };

    struct MouseInputEvent
    {
        unsigned long btn;
        KEY_STATUS event;
    };

    typedef CInputListener< MouseInputEvent > CMouseListener;

    class CInput final : public CInputListenable< MouseInputEvent >
    {
    public:
        CInput();
        CInput(const CInput&) = delete;
        CInput& operator=(const CInput&) = delete;
        ~CInput();

    public:
        void SetHwnd(WindowHandle hWnd) { m_hWnd = hWnd; }

        void BeforeFrame();
        void AfterFrame();
        void UpdateObservers();

        void Reset();

        void ShowCursor(bool bShow);
        bool IsCursorVisible();

        void LockCursor(bool bHide);
        bool GetCursorLocked();
        void ReleaseCursor();


        bool IsActionJustTriggered(unsigned long action);
        bool IsActionTriggered(unsigned long action);
        bool IsActionJustReleased(unsigned long action);

        bool IsScrollEvent();
        bool IsKeyDown(unsigned long key);
        bool IsKeyJustDown(unsigned long key);
        bool IsKeyJustUp(unsigned long key);
        bool WasKeyDownEvent(unsigned long key);
        bool IsButtonDown(unsigned long button);
        bool IsButtonJustDown(unsigned long button);
        bool IsButtonJustUp(unsigned long button);
        long GetMouseX() { return m_MouseX; }
        long GetMouseDX() { return m_MouseDX; }
        long GetMouseY() { return m_MouseY; }
        long GetMouseDY() { return m_MouseDY; }
        long GetMouseDZ() { return m_MouseDZ; }
        void GetTextinput(std::string& strText) { strText = m_strTextInput; }

        void KeyDownEvent(unsigned long key);
        void KeyUpEvent(unsigned long key);
        void KeyDownEventController(unsigned long key);
        void KeyUpEventController(unsigned long key);

        void SetMousePosition(long x, long y);
        void MouseDownEvent(unsigned long button);
        void MouseUpEvent(unsigned long button);
        void ScrollEvent(long dz) { m_Buttons[2] = (1 << KEY_STATUS_WHEEL); m_MouseDZ += dz; }

        void TextInput(char Character) { m_strTextInput.append(1, Character); }

        // Has Input already been handled this frame?
        void SetMouseHandled(bool handled = true) { m_bMouseHandled = handled; }
        bool GetMouseHandled() { return m_bMouseHandled; }
        void SetKeyboardHandled(bool handled = true) { m_bKeyboardHandled = handled; }
        bool GetKeyboardHandled() { return m_bKeyboardHandled; }

        void AddMouseObserver(unsigned long button, KEY_STATUS event, CObserver* pObserver, unsigned long id);
        void RemoveMouseObserver(unsigned long button, KEY_STATUS event, CObserver* pObserver);
        void RemoveMouseObservers(unsigned long button);

        void AddKeyboardObserver(KEY_STATUS event, CObserver* pObserver, unsigned long id);

        void SetKeyboardActionMapping(unsigned long sourceKey, unsigned long targetAction);

        bool GetKeyboardInput() const { return m_bKeyboardActive; }

    private:
        typedef std::map< unsigned long, unsigned long > mapKeyAction;
        mapKeyAction m_keyboardActionMap;

        std::vector< unsigned char > m_actions;

        unsigned char m_Keys[256];
        unsigned char m_Buttons[16];

        long m_MouseX;
        long m_MouseDX;
        long m_MouseY;
        long m_MouseDY;
        long m_MouseDZ;

        bool m_bCursorLocked;
        bool m_bCursorHidden;
        bool m_bMouseHandled;
        bool m_bKeyboardHandled;

        bool m_bKeyboardActive;

        std::string m_strTextInput;

        WindowHandle m_hWnd;

        CObservable m_mouseObservers[3];
        CObservable m_keyboardObservers;

        ENV_PLATFORM_WINDOWS_ONLY(XINPUT_STATE m_prevState;);
    };
} // env

#endif // INPUT_H