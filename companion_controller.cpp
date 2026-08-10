#include "companion_controller.h"
#include <Arduino.h>

bool CompanionController::begin()
{
    vehicle.begin();

    behavior.begin();

    face.begin();

    return true;
}

void CompanionController::update(
    float rpm,
    float speed,
    float accel,
    bool idleStop,
    float dt)
{
    //----------------------------------------
    // 車状態
    //----------------------------------------

    vehicle.update(
        rpm,
        speed,
        accel,
        idleStop);

    //----------------------------------------
    // イベント取得
    //----------------------------------------

    VehicleEvent event =
        vehicle.consumeEvent();

    //----------------------------------------
    // 行動決定
    //----------------------------------------

    behavior.update(
        vehicle.getState(),
        event,
        dt);

    // Serial.print("Expression=");
    // Serial.print((int)behavior.getExpression());

    // Serial.print("  Action=");
    // Serial.println((int)behavior.getAction());
    //----------------------------------------
    // 表情変更
    //----------------------------------------

    face.requestExpression(
        behavior.getExpression(),
        1.0f);

    //----------------------------------------
    // Action
    //----------------------------------------

    Action action =
        behavior.getAction();

    if(action != Action::None)
    {
        face.playAction(action);
    }

    //----------------------------------------
    // アニメーション
    //----------------------------------------

    face.update(dt);

    // Serial.println((int)behavior.getExpression());
}

const EyeShape& CompanionController::getLeftEye()
{
    return face.getLeftEye();
}

const EyeShape& CompanionController::getRightEye()
{
    return face.getRightEye();
}

const BehaviorManager& CompanionController::getBehaviorManager() const
{
    return behavior;
}

void CompanionController::setDebugMode(bool enable)
{
    debugMode = enable;
}

bool CompanionController::isDebugMode() const
{
    return debugMode;
}