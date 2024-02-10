#include <random>
#include <map>
#include <string>
#include "MovementStates.h"
using namespace std;

namespace m1
{
    int GetRandomNumberInRange(const int minInclusive, const int maxInclusive)
    {
        static std::random_device randomDevice;
        static std::default_random_engine randomEngine(randomDevice());

        std::uniform_int_distribution<int> uniformDist(minInclusive, maxInclusive);

        return uniformDist(randomEngine);
    }
    MovementState GetNextMovementState(const MovementState currentState)
    {
        int randomChange = GetRandomNumberInRange(0, 1);
        MovementState nextState = currentState;

        switch (currentState)
        {
        case MovementState::GoingForward:
        case MovementState::GoingBackward:
            nextState = (randomChange == 1)
                ? MovementState::InPlaceRotationLeft
                : MovementState::InPlaceRotationRight;
            break;

        case MovementState::InPlaceRotationLeft:
        case MovementState::InPlaceRotationRight:
            nextState = (randomChange == 1)
                ? MovementState::GoingForward
                : MovementState::GoingBackward;
            break;

        default:
            break;
        }

        return nextState;
    }
    std::string GetMovementStateName(const MovementState state)
    {
        static const std::map<MovementState, std::string> kStateNames
        {
            { MovementState::GoingForward, "GoingForward" },
            { MovementState::GoingBackward, "GoingBackward" },
            { MovementState::InPlaceRotationLeft, "InPlaceRotationLeft" },
            { MovementState::InPlaceRotationRight, "InPlaceRotationRight" },
        };

        std::string s = "";
        auto search = kStateNames.find(state);
        if (search != kStateNames.end())
        {
            s = kStateNames.at(state);
        }

        return s;
    }
}