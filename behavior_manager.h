#pragma once

#include "vehicle_state_manager.h"
#include "face_animator.h"
#include "face_types.h"


class BehaviorManager
{
public:

    bool begin();

    void update(
        VehicleState state,
        VehicleEvent event,
        float dt);

    Expression getExpression() const;
    
    float getBoredom() const;
    float getExcitement() const;
    float getSleepiness() const;
    float getBlinkDesire() const;

    Action getAction();

private:

    //----------------------------------------
    // 内部状態
    //----------------------------------------

    float boredom = 0.0f;
    float excitement = 0.0f;
    float sleepiness = 0.0f;
    float mood = 0.0f;
    float blinkDesire = 0.0f;
    // float yawnDesire;
    // float lookAroundDesire;
    // float stretchDesire;

    //----------------------------------------

    Expression currentExpression =
        Expression::Neutral;

    Action currentAction =
        Action::None;

private:

    void updateInternalState(
        VehicleState state,
        VehicleEvent event,
        float dt);

    void decideExpression();

    void decideAction();
};