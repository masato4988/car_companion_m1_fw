#include "behavior_manager.h"
// #include <arduino.h>
#include <algorithm>

bool BehaviorManager::begin()
{
    boredom = 0.0f;
    excitement = 0.0f;
    sleepiness = 0.0f;
    mood = 0.0f;
    blinkDesire = 0.0f;

    currentExpression = Expression::Neutral;
    currentAction = Action::None;

    return true;
}

void BehaviorManager::update(
    VehicleState state,
    VehicleEvent event,
    float dt)
{
    updateInternalState(
        state,
        event,
        dt);

    decideExpression();

    decideAction();
}

Expression BehaviorManager::getExpression() const
{
    return currentExpression;
}

float BehaviorManager::getBoredom() const
{
    return boredom;
}

float BehaviorManager::getExcitement() const
{
    return excitement;
}

float BehaviorManager::getSleepiness() const
{
    return sleepiness;
}

float BehaviorManager::getBlinkDesire() const
{
    return blinkDesire;
}

Action BehaviorManager::getAction()
{
    Action action = currentAction;

    currentAction = Action::None;

    return action;
}

//=======================================================

void BehaviorManager::updateInternalState(
    VehicleState state,
    VehicleEvent event,
    float dt)
{
    //----------------------------------------
    // 興奮は自然に減衰
    //----------------------------------------

    excitement -= dt * 0.5f;

    if(excitement < 0.0f)
    {
        excitement = 0.0f;
    }

    //----------------------------------------
    // 暇
    //----------------------------------------

    if(state == VehicleState::Idle)
    {
        boredom += dt * 0.10f;
    }

    if(state == VehicleState::Cruising)
    {
        boredom += dt * 0.05f;
    }

    if(state == VehicleState::Accelerating)
    {
        boredom -= dt * 0.50f;
    }

    //----------------------------------------
    // 眠気
    //----------------------------------------

    if(state == VehicleState::IdleStop)
    {
        sleepiness += dt * 0.30f;
    }

    if(event == VehicleEvent::EngineStarted)
    {
        sleepiness = 0.0f;
    }

    
    //----------------------------------------
    // 瞬き
    //----------------------------------------
    blinkDesire += dt * 0.3f;
    
    if(state == VehicleState::Idle)
    {
        blinkDesire += dt * 0.6f;
    }
    if(state == VehicleState::Cruising)
    {
        blinkDesire += dt * 0.50f;
    }
    
    //----------------------------------------
    // Clamp
    //----------------------------------------
    
    boredom = 
    std::clamp(
        boredom,
        0.0f,
        1.0f);
            
    excitement = 
    std::clamp(
        excitement,
        0.0f,
        1.0f);
        
    sleepiness = 
    std::clamp(
        sleepiness,
        0.0f,
        1.0f);
        
    blinkDesire =
    std::clamp(
        blinkDesire,
        0.0f,
        1.0f);

        
    //----------------------------------------
    // 目覚め
    //----------------------------------------

    if(event == VehicleEvent::EngineStarted)
    {
        excitement += 0.7f;
    
    currentAction = Action::WakeUp;
    }
    
    //----------------------------------------
    // 寝入り
    //----------------------------------------
    if(event == VehicleEvent::EngineStopped)
    {
        currentAction = Action::FallAsleep;
    }
}
    
void BehaviorManager::decideExpression()
{
    //----------------------------------------
    // 興奮
    //----------------------------------------
    
    if(excitement > 0.7f)
    {
        currentExpression = Expression::Smile;
        return;
    }
    
    //----------------------------------------
    // 眠い
    //----------------------------------------
    
    if(sleepiness > 0.6f)
    {
        currentExpression = Expression::Sleepy;
        return;
    }
    
    //----------------------------------------
    // 暇
    //----------------------------------------
    
    if(boredom > 0.7f)
    {
        currentExpression = Expression::Curious;
        return;
    }
    
    //----------------------------------------
    // まばたき
    //----------------------------------------
    
    if(blinkDesire >= 1.0f)
    {
        blinkDesire = 0.0f;
        currentAction = Action::Blink;
        return;
    }
    
    //----------------------------------------
    
    currentExpression =
    Expression::Neutral;
}

void BehaviorManager::decideAction()
{
    //----------------------------------------
    // Actionは一回だけ出す
    //----------------------------------------
    
    if(currentAction != Action::None)
    {
        return;
    }

    

    //----------------------------------------
    // 将来
    //----------------------------------------

    // boredom > 0.8
    // ↓
    // ランダムで
    // Blink
    // LookAround
    // DoubleBlink
}
