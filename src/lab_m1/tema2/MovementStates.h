
#pragma once

namespace m1 {
    enum class MovementState
    {
        GoingForward,
        GoingBackward,
        InPlaceRotationLeft,
        InPlaceRotationRight
    };
    extern int GetRandomNumberInRange(const int minInclusive, const int maxInclusive);
    MovementState GetNextMovementState(const MovementState currentState);
    std::string GetMovementStateName(const MovementState state);
}