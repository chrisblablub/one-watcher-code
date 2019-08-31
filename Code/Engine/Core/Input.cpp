#

#include <Engine/Core/Input.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    CInput::CInput()
        : m_MouseX(0)
        , m_MouseDX(0)
        , m_MouseY(0)
        , m_MouseDY(0)
        , m_MouseDZ(0)
        , m_bMouseHandled(false)
        , m_bKeyboardHandled(false)
        , m_bCursorLocked(false)
        , m_bCursorHidden(false)
    {
#ifdef ENV_PLATFORM_WINDOWS
        ZeroMemory(m_Keys, 256);
        ZeroMemory(m_Buttons, 16);

        m_actions.resize(10);

        SetKeyboardActionMapping(VK_SPACE, ACTION_PRIMARY);
        SetKeyboardActionMapping(VK_LEFT, ACTION_LEFT);
        SetKeyboardActionMapping(VK_RIGHT, ACTION_RIGHT);
        SetKeyboardActionMapping(VK_UP, ACTION_UP);
        SetKeyboardActionMapping(VK_DOWN, ACTION_DOWN);
        SetKeyboardActionMapping(VK_ESCAPE, ACTION_BACK);
        SetKeyboardActionMapping(VK_RETURN, ACTION_START);
        SetKeyboardActionMapping(VK_SHIFT, ACTION_SECONDARY);
#endif // ENV_PLATFORM_WINDOWS
    }

    //-----------------------------------------------------------------------------------
    CInput::~CInput()
    {

    }

    //-----------------------------------------------------------------------------------
    void CInput::SetKeyboardActionMapping(unsigned long sourceKey, unsigned long targetAction)
    {
        m_actions.resize(std::max((int)m_actions.size(), (int)(targetAction + 1)));
        m_keyboardActionMap[sourceKey] = targetAction;
    }

    //-----------------------------------------------------------------------------------
    void CInput::BeforeFrame()
    {
        m_bKeyboardActive = true;

#ifdef ENV_PLATFORM_WINDOWS

        SetMouseHandled(false);
        SetKeyboardHandled(false);

        POINT p;
        GetCursorPos(&p);
        ScreenToClient(m_hWnd, &p);

        m_MouseDX = p.x - m_MouseX;
        m_MouseDY = p.y - m_MouseY;
        
        if (!m_bCursorLocked)
        {
            m_MouseX = p.x;
            m_MouseY = p.y;
        }
        else
        {
            p.x = m_MouseX;
            p.y = m_MouseY;
            ClientToScreen(m_hWnd, &p);
            SetCursorPos(p.x, p.y);
        }


        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        unsigned long res = XInputGetState(0, &state);

        if (res == ERROR_SUCCESS)
        {
            float magnitude = static_cast<float>(abs(state.Gamepad.sThumbLX));
            if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                if (magnitude > 32767.0f)
                    magnitude = 32767.0f;

                magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

                magnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

                if (magnitude > 0.4f)
                {
                    if (state.Gamepad.sThumbLX > 0)
                    {
                        KeyUpEventController(VK_LEFT);
                        KeyDownEventController(VK_RIGHT);
                    }
                    else if (state.Gamepad.sThumbLX < 0)
                    {
                        KeyUpEventController(VK_RIGHT);
                        KeyDownEventController(VK_LEFT);
                    }
                }
                else
                {
                    KeyUpEventController(VK_RIGHT);
                    KeyUpEventController(VK_LEFT);
                }
            }
            else if (abs(m_prevState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                KeyUpEventController(VK_RIGHT);
                KeyUpEventController(VK_LEFT);
            }

            magnitude = static_cast<float>(abs(state.Gamepad.sThumbLY));
            if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                if (magnitude > 32767)
                    magnitude = 32767;

                magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

                magnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

                if (magnitude > 0.4f)
                {
                    if (state.Gamepad.sThumbLY > 0)
                    {
                        KeyUpEventController(VK_DOWN);
                        KeyDownEventController(VK_UP);
                    }
                    else if (state.Gamepad.sThumbLY < 0)
                    {
                        KeyUpEventController(VK_UP);
                        KeyDownEventController(VK_DOWN);
                    }
                }
                else
                {
                    KeyUpEventController(VK_UP);
                    KeyUpEventController(VK_DOWN);
                }
            }
            else if (abs(m_prevState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
            {
                KeyUpEventController(VK_UP);
                KeyUpEventController(VK_DOWN);
            }


            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
                KeyDownEventController(VK_UP);
            else if (m_prevState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
                KeyUpEventController(VK_UP);

            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
                KeyDownEventController(VK_DOWN);
            else if (m_prevState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
                KeyUpEventController(VK_DOWN);


            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
                KeyDownEventController(VK_LEFT);
            else if (m_prevState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
                KeyUpEventController(VK_LEFT);

            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
                KeyDownEventController(VK_RIGHT);
            else if (m_prevState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
                KeyUpEventController(VK_RIGHT);

            if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) > 0)
                KeyDownEventController(VK_SPACE);
            else if ((m_prevState.Gamepad.wButtons & XINPUT_GAMEPAD_A) > 0)
                KeyUpEventController(VK_SPACE);

            if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) > 0)
                KeyDownEventController(VK_ESCAPE);
            else if ((m_prevState.Gamepad.wButtons & XINPUT_GAMEPAD_B) > 0)
                KeyUpEventController(VK_ESCAPE);

            if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START) > 0)
                KeyDownEventController(VK_RETURN);
            else if ((m_prevState.Gamepad.wButtons & XINPUT_GAMEPAD_START) > 0)
                KeyUpEventController(VK_RETURN);

            if (state.Gamepad.bLeftTrigger > 10 || state.Gamepad.bRightTrigger > 10)
                KeyDownEventController(VK_SHIFT);
            else if (m_prevState.Gamepad.bLeftTrigger > 10 || m_prevState.Gamepad.bRightTrigger > 10)
                KeyUpEventController(VK_SHIFT);

            m_prevState = state;
        }

#endif // ENV_PLATFORM_WINDOWS
    }

    //-----------------------------------------------------------------------------------
    void CInput::KeyDownEventController(unsigned long key)
    {
        KeyDownEvent(key);

        m_bKeyboardActive = false;
    }

    //-----------------------------------------------------------------------------------
    void CInput::KeyUpEventController(unsigned long key)
    {
        KeyUpEvent(key);

        m_bKeyboardActive = false;
    }

    //-----------------------------------------------------------------------------------
    void CInput::AfterFrame()
    {
        // clear one-time-flags
        for (int i = 0; i < m_actions.size(); ++i)
            m_actions[i] &= ~((1 << KEY_STATUS_EVENT) | (1 << KEY_STATUS_JUST_DOWN) | (1 << KEY_STATUS_JUST_UP));

        for (int i = 0; i < 256; ++i)
            m_Keys[i] &= ~((1 << KEY_STATUS_EVENT) | (1 << KEY_STATUS_JUST_DOWN) | (1 << KEY_STATUS_JUST_UP));

        for (int i = 0; i < 16; ++i)
            m_Buttons[i] &= ~((1 << KEY_STATUS_JUST_DOWN) | (1 << KEY_STATUS_JUST_UP) | (1 << KEY_STATUS_WHEEL));

        // clear cumulative values
        m_strTextInput.clear();
        m_MouseDX = 0;
        m_MouseDY = 0;
        m_MouseDZ = 0;
    }

    //-----------------------------------------------------------------------------------
    void CInput::Reset()
    {
        for (int i = 0; i < m_actions.size(); ++i)
        {
            m_actions[i] &= ~(1 << KEY_STATUS_EVENT);
            m_actions[i] &= ~(1 << KEY_STATUS_DOWN);
            m_actions[i] &= ~(1 << KEY_STATUS_JUST_DOWN);
            m_actions[i] &= ~(1 << KEY_STATUS_JUST_UP);
        }

        for (int i = 0; i < 256; ++i)
        {
            m_Keys[i] &= ~(1 << KEY_STATUS_EVENT);
            m_Keys[i] &= ~(1 << KEY_STATUS_DOWN);
            m_Keys[i] &= ~(1 << KEY_STATUS_JUST_DOWN);
            m_Keys[i] &= ~(1 << KEY_STATUS_JUST_UP);
        }

        for (int i = 0; i < 16; ++i)
        {
            m_Buttons[i] &= ~(1 << KEY_STATUS_WHEEL);
            m_Buttons[i] &= ~(1 << KEY_STATUS_DOWN);
            m_Buttons[i] &= ~(1 << KEY_STATUS_JUST_DOWN);
            m_Buttons[i] &= ~(1 << KEY_STATUS_JUST_UP);
            m_Buttons[i] &= ~(1 << KEY_STATUS_WHEEL);
        }
    }

    //-----------------------------------------------------------------------------------
    void CInput::UpdateObservers()
    {
        for (int i = 0; i < 16; ++i)
        {
            MouseInputEvent mouseEvent;
            mouseEvent.btn = i;
            for (int j = 0; j < 4; ++j)
            {
                if (m_Buttons[i] & (1 << j))
                {
                    mouseEvent.event = (KEY_STATUS)j;
                    NotifyListeners(mouseEvent);
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CInput::ShowCursor(bool bShow)
    {
        m_bCursorHidden = !bShow;
        ENV_PLATFORM_WINDOWS_ONLY(::ShowCursor(bShow););
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsCursorVisible()
    {
        return !m_bCursorHidden;
    }

    //-----------------------------------------------------------------------------------
    void CInput::LockCursor(bool bHide)
    {
        m_bCursorLocked = true;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::GetCursorLocked()
    {
        return m_bCursorLocked;
    }

    //-----------------------------------------------------------------------------------
    void CInput::ReleaseCursor()
    {
        m_bCursorLocked = false;
        if (m_bCursorHidden)
        {
            m_bCursorHidden = false;
            ENV_PLATFORM_WINDOWS_ONLY(::ShowCursor(true););
        }
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsActionTriggered(unsigned long action)
    {
        return (m_actions[action] & (1 << KEY_STATUS_DOWN)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsActionJustTriggered(unsigned long action)
    {
        return (m_actions[action] & (1 << KEY_STATUS_JUST_DOWN)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsActionJustReleased(unsigned long action)
    {
        return (m_actions[action] & (1 << KEY_STATUS_JUST_UP)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsScrollEvent()
    {
        return (m_Buttons[2] & (1 << KEY_STATUS_WHEEL)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsKeyDown(unsigned long key)
    {
        assert(key < 256);
        return (m_Keys[key] & (1 << KEY_STATUS_DOWN)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsKeyJustDown(unsigned long key)
    {
        assert(key < 256);
        return (m_Keys[key] & (1 << KEY_STATUS_JUST_DOWN)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsKeyJustUp(unsigned long key)
    {
        assert(key < 256);
        return (m_Keys[key] & (1 << KEY_STATUS_JUST_UP)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::WasKeyDownEvent(unsigned long key)
    {
        assert(key < 256);
        return (m_Keys[key] & (1 << KEY_STATUS_EVENT)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsButtonDown(unsigned long button)
    {
        assert(button < 16);
        return (m_Buttons[button] & (1 << KEY_STATUS_DOWN)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsButtonJustDown(unsigned long button)
    {
        assert(button < 16);
        return (m_Buttons[button] & (1 << KEY_STATUS_JUST_DOWN)) > 0;
    }

    //-----------------------------------------------------------------------------------
    bool CInput::IsButtonJustUp(unsigned long button)
    {
        assert(button < 16);
        return (m_Buttons[button] & (1 << KEY_STATUS_JUST_UP)) > 0;
    }

    //-----------------------------------------------------------------------------------
    void CInput::KeyDownEvent(unsigned long key)
    {
        assert(key < 256);
        if (m_Keys[key] & (1 << KEY_STATUS_DOWN))
        {
            m_Keys[key] |= (1 << KEY_STATUS_EVENT);

            mapKeyAction::iterator it = m_keyboardActionMap.find(key);
            if (it != m_keyboardActionMap.end())
                m_actions[it->second] |= (1 << KEY_STATUS_EVENT);
        }
        else
        {
            m_Keys[key] = (1 << KEY_STATUS_DOWN) | (1 << KEY_STATUS_JUST_DOWN) | (1 << KEY_STATUS_EVENT);

            mapKeyAction::iterator it = m_keyboardActionMap.find(key);
            if (it != m_keyboardActionMap.end())
                m_actions[it->second] = (1 << KEY_STATUS_DOWN) | (1 << KEY_STATUS_JUST_DOWN) | (1 << KEY_STATUS_EVENT);
        }
    }

    //-----------------------------------------------------------------------------------
    void CInput::KeyUpEvent(unsigned long key)
    {
        assert(key < 256);
        m_Keys[key] = (1 << KEY_STATUS_JUST_UP);

        mapKeyAction::iterator it = m_keyboardActionMap.find(key);
        if (it != m_keyboardActionMap.end())
            m_actions[it->second] = (1 << KEY_STATUS_JUST_UP);
    }

    //-----------------------------------------------------------------------------------
    void CInput::SetMousePosition(long x, long y)
    {

    }

    //-----------------------------------------------------------------------------------
    void CInput::MouseDownEvent(unsigned long button)
    {
        assert(button < 16);
        m_Buttons[button] = (1 << KEY_STATUS_DOWN) | (1 << KEY_STATUS_JUST_DOWN);
    }

    //-----------------------------------------------------------------------------------
    void CInput::MouseUpEvent(unsigned long button)
    {
        assert(button < 16);
        m_Buttons[button] = (1 << KEY_STATUS_JUST_UP);
    }

    //-----------------------------------------------------------------------------------
    void CInput::AddMouseObserver(unsigned long button, KEY_STATUS event, CObserver* pObserver, unsigned long id)
    {
        m_mouseObservers[event].AddObserver(button, pObserver, id);
    }

    //-----------------------------------------------------------------------------------
    void CInput::RemoveMouseObserver(unsigned long button, KEY_STATUS event, CObserver* pObserver)
    {
        m_mouseObservers[event].RemoveObserver(button, pObserver);
    }

    //-----------------------------------------------------------------------------------
    void CInput::RemoveMouseObservers(unsigned long button)
    {
        m_mouseObservers[button].RemoveObservers();
    }

    //-----------------------------------------------------------------------------------
    void CInput::AddKeyboardObserver(KEY_STATUS event, CObserver* pObserver, unsigned long id)
    {
        assert(false);
        m_keyboardObservers.AddObserver((unsigned long)event, pObserver, id);
    }

}