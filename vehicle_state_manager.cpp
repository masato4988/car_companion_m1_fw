#include "vehicle_state_manager.h"

#include <Arduino.h>

bool VehicleStateManager::begin()
{
    currentState = VehicleState::EngineOff;
    previousState = VehicleState::EngineOff;

    currentEvent = VehicleEvent::None;

    stateStartTime = millis();

    return true;
}

void VehicleStateManager::update(
    float rpm,
    float speed,
    float accel,
    bool idleStop)
{
    VehicleState nextState =
        decideState(
            rpm,
            speed,
            accel,
            idleStop);

    if(nextState == currentState)
    {
        return;
    }

    previousState = currentState;
    currentState = nextState;

    stateStartTime = millis();

    generateEvent(
        previousState,
        currentState);
}

VehicleState VehicleStateManager::decideState(
    float rpm,
    float speed,
    float accel,
    bool idleStop)
{
    //----------------------------------------
    // Engine OFF
    //----------------------------------------

    if(rpm < 10)
    {
        return VehicleState::EngineOff;
    }

    //----------------------------------------
    // Idle Stop
    //----------------------------------------

    if(idleStop)
    {
        return VehicleState::IdleStop;
    }

    //----------------------------------------
    // Stop
    //----------------------------------------

    if(speed < 1.0f)
    {
        return VehicleState::Idle;
    }

    //----------------------------------------
    // Running
    //----------------------------------------

    if(accel > 0.3f)
    {
        return VehicleState::Accelerating;
    }

    if(accel < -0.3f)
    {
        return VehicleState::Decelerating;
    }

    return VehicleState::Cruising;
}

void VehicleStateManager::generateEvent(
    VehicleState previous,
    VehicleState current)
{
    Serial.print("prev=");
    Serial.print((int)previous);

    Serial.print(" current=");
    Serial.println((int)current);

    currentEvent = VehicleEvent::None;

    //----------------------------------------

    if(previous == VehicleState::EngineOff &&
       current != VehicleState::EngineOff)
    {
        currentEvent = VehicleEvent::EngineStarted;
        return;
    }

    //----------------------------------------

    if(previous != VehicleState::EngineOff &&
       current == VehicleState::EngineOff)
    {
        currentEvent = VehicleEvent::EngineStopped;
        return;
    }

    //----------------------------------------

    if(current == VehicleState::IdleStop)
    {
        currentEvent = VehicleEvent::IdleStopEntered;
        return;
    }

    //----------------------------------------

    if(previous == VehicleState::IdleStop &&
       current != VehicleState::IdleStop)
    {
        currentEvent = VehicleEvent::IdleStopExited;
        return;
    }

    //----------------------------------------

    if(current == VehicleState::Accelerating)
    {
        currentEvent = VehicleEvent::StartAccelerating;
        return;
    }

    //----------------------------------------

    if(current == VehicleState::Cruising)
    {
        currentEvent = VehicleEvent::StartCruising;
        return;
    }

    //----------------------------------------

    if(current == VehicleState::Decelerating)
    {
        currentEvent = VehicleEvent::StartDecelerating;
        return;
    }
}

VehicleState VehicleStateManager::getState() const
{
    return currentState;
}

VehicleEvent VehicleStateManager::consumeEvent()
{
    VehicleEvent event = currentEvent;

    currentEvent = VehicleEvent::None;

    return event;
}

uint32_t VehicleStateManager::getStateElapsedMs() const
{
    return millis() - stateStartTime;
}