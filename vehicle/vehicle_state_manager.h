#pragma once

#include <stdint.h>

enum class VehicleState
{
    EngineOff,

    Idle,

    IdleStop,

    Accelerating,

    Cruising,

    Decelerating,

    Reversing
};

enum class VehicleEvent
{
    None,

    EngineStarted,
    EngineStopped,

    IdleStopEntered,
    IdleStopExited,

    StartAccelerating,
    StartCruising,
    StartDecelerating,

    ReverseEntered
};

class VehicleStateManager
{
public:

    bool begin();

    void update(
        float rpm,
        float speed,
        float accel,
        bool idleStop);

    VehicleState getState() const;

    VehicleEvent consumeEvent();

    uint32_t getStateElapsedMs() const;

private:

    VehicleState currentState = VehicleState::EngineOff;
    VehicleState previousState = VehicleState::EngineOff;

    VehicleEvent currentEvent = VehicleEvent::None;

    uint32_t stateStartTime = 0;

private:

    VehicleState decideState(
        float rpm,
        float speed,
        float accel,
        bool idleStop);

    void generateEvent(
        VehicleState previous,
        VehicleState current);
};