#pragma once

enum InputAction {
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    Jump,
    Crouch,
    Sprint,
    Count // Keep this last to track the number of actions
};

class Input {
public:
    Input();
    ~Input();

    void update(); // Call this every frame to update input states
    bool isActionPressed(InputAction action) const;
    bool isActionReleased(InputAction action) const;
    bool isActionHeld(InputAction action) const;
private:
    bool m_CurrentState[static_cast<int>(InputAction::Count)];
    bool m_PreviousState[static_cast<int>(InputAction::Count)];

    void updateKeyStates();
    void updateMouseStates();
};
