#pragma once

#include "vehicle_state_manager.h"
#include "behavior_manager.h"
#include "face_animator.h"

class CompanionController
{
public:

    bool begin();

    void update(
        float rpm,
        float speed,
        float accel,
        bool idleStop,
        float dt);

    const EyeShape& getLeftEye();
    const EyeShape& getRightEye();

    const BehaviorManager& getBehaviorManager() const;

    void setDebugMode(bool enable);
    bool isDebugMode() const;

private:

    VehicleStateManager vehicle;
    BehaviorManager behavior;
    FaceAnimator face;

    bool debugMode = false;
};