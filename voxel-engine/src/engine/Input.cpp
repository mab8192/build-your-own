#include "Input.h"

#include "raylib.h"

Input::Input() {
    // Initialize current and previous states to false
    for (int i = 0; i < static_cast<int>(InputAction::Count); ++i) {
        m_CurrentState[i] = false;
        m_PreviousState[i] = false;
    }
}

Input::~Input() {}

void Input::update() {
    updateKeyStates();
    updateMouseStates();
}

bool Input::isActionPressed(InputAction action) const {
    return m_CurrentState[static_cast<int>(action)] && !m_PreviousState[static_cast<int>(action)];
}

bool Input::isActionReleased(InputAction action) const {
    return !m_CurrentState[static_cast<int>(action)] && m_PreviousState[static_cast<int>(action)];
}

bool Input::isActionHeld(InputAction action) const {
    return m_CurrentState[static_cast<int>(action)];
}

void Input::updateKeyStates() {
    // Update key states based on raylib input functions
    // Store previous states
    for (int i = 0; i < static_cast<int>(InputAction::Count); ++i) {
        m_PreviousState[i] = m_CurrentState[i];
    }

    m_CurrentState[static_cast<int>(InputAction::MoveForward)] = IsKeyDown(KEY_W);
    m_CurrentState[static_cast<int>(InputAction::MoveBackward)] = IsKeyDown(KEY_S);
    m_CurrentState[static_cast<int>(InputAction::MoveLeft)] = IsKeyDown(KEY_A);
    m_CurrentState[static_cast<int>(InputAction::MoveRight)] = IsKeyDown(KEY_D);
    m_CurrentState[static_cast<int>(InputAction::Jump)] = IsKeyPressed(KEY_SPACE);
    m_CurrentState[static_cast<int>(InputAction::Crouch)] = IsKeyPressed(KEY_LEFT_CONTROL);
    m_CurrentState[static_cast<int>(InputAction::Sprint)] = IsKeyDown(KEY_LEFT_SHIFT);
}

void Input::updateMouseStates() {
    // Update mouse states if needed
    // For example, you can check for mouse button presses or mouse movement
    // m_CurrentState[static_cast<int>(InputAction::MouseLeft)] = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    // m_CurrentState[static_cast<int>(InputAction::MouseRight)] = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
}