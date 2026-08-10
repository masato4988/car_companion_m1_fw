#include "face_animator.h"
#include <arduino.h>

bool FaceAnimator::begin()
{
    createExpression(
        Expression::Neutral,
        1.0f,
        targetLeft,
        targetRight);

    currentLeft = targetLeft;
    currentRight = targetRight;

    startLeft = targetLeft;
    startRight = targetRight;

    currentExpression = Expression::Neutral;
    requestedExpression = Expression::Neutral;

    return true;
}

void FaceAnimator::update(float dt)
{
   //----------------------------------------
    // 再生中でなければ新しいExpressionを見る
    //----------------------------------------

    if(!playing)
    {
        if(currentExpression != requestedExpression ||
        fabs(currentAmount - requestedAmount) > 0.01f)
        {
            currentExpression = requestedExpression;
            currentAmount = requestedAmount;

            startLeft = currentLeft;
            startRight = currentRight;

            createExpression(
                currentExpression,
                currentAmount,
                targetLeft,
                targetRight);

            keyFrameCount = 1;
            currentKeyFrame = 0;

            keyFrames[0].left = targetLeft;
            keyFrames[0].right = targetRight;
            keyFrames[0].duration = 0.30f;

            elapsed = 0.0f;
            playing = true;
        }
        else
        {
            // 再生するものが無い
            return;
        }
    }

    elapsed += dt;

    float duration =
        keyFrames[currentKeyFrame].duration;

    float t;

    if(duration <= 0.0f)
    {
        t = 1.0f;
    }
    else
    {
        t = elapsed / duration;

        if(t > 1.0f)
        {
            t = 1.0f;
        }
    }

    //----------------------------------------
    // 補間
    //----------------------------------------

    currentLeft =
        lerp(
            startLeft,
            keyFrames[currentKeyFrame].left,
            t);

    currentRight =
        lerp(
            startRight,
            keyFrames[currentKeyFrame].right,
            t);

    //----------------------------------------
    // 次のフレームへ
    //----------------------------------------

    if(t >= 1.0f)
    {
        currentLeft =
            keyFrames[currentKeyFrame].left;

        currentRight =
            keyFrames[currentKeyFrame].right;

        startLeft = currentLeft;
        startRight = currentRight;

        currentKeyFrame++;

        elapsed = 0.0f;

        //----------------------------------------
        // 全部終わった
        //----------------------------------------

        if(currentKeyFrame >= keyFrameCount)
        {
            playing = false;
        }
    }
}

void FaceAnimator::requestExpression(
    Expression expression,
    float amount)
{
    requestedExpression = expression;
    requestedAmount = amount;
}

void FaceAnimator::playAction(Action action)
{
    switch(action)
    {
        case Action::Blink:
            buildBlinkScenario();
            Serial.println("[FACE][EVENT]Blink");
            break;
            
        case Action::WakeUp:
            buildWakeUpScenario();
            Serial.println("[FACE][EVENT]Wakeup");
            break;

        case Action::FallAsleep:
            buildFallAsleepScenario();
            Serial.println("[FACE][EVENT]FallAsleep");
            break;

        default:
            break;
    }
}


const EyeShape& FaceAnimator::getLeftEye() const
{
    return currentLeft;
}

const EyeShape& FaceAnimator::getRightEye() const
{
    return currentRight;
}

EyeShape FaceAnimator::lerp(
    const EyeShape& a,
    const EyeShape& b,
    float t)
{
    EyeShape e;

    e.cx = a.cx + (b.cx - a.cx) * t;
    e.cy = a.cy + (b.cy - a.cy) * t;

    e.width = a.width + (b.width - a.width) * t;
    e.height = a.height + (b.height - a.height) * t;

    e.radius = a.radius + (b.radius - a.radius) * t;

    e.upperArc = a.upperArc + (b.upperArc - a.upperArc) * t;
    e.lowerArc = a.lowerArc + (b.lowerArc - a.lowerArc) * t;

    e.upperSlope = a.upperSlope + (b.upperSlope - a.upperSlope) * t;
    e.lowerSlope = a.lowerSlope + (b.lowerSlope - a.lowerSlope) * t;

    return e;
}


void FaceAnimator::createExpression(
    Expression expression,
    float amount,
    EyeShape& left,
    EyeShape& right)
{
    //----------------------------------------
    // Default
    //----------------------------------------

    EyeShape defaultLeft{};
    defaultLeft.cx = -30;
    defaultLeft.cy = 0;

    defaultLeft.width = 40;
    defaultLeft.height = 34;

    defaultLeft.radius = 10;

    defaultLeft.upperArc = 0;
    defaultLeft.lowerArc = 0;

    defaultLeft.upperSlope = 0;
    defaultLeft.lowerSlope = 0;

    EyeShape defaultRight = defaultLeft;
    defaultRight.cx = 30;

    //----------------------------------------
    // 初期状態
    //----------------------------------------

    left = defaultLeft;
    right = defaultRight;

    //----------------------------------------
    // Expression
    //----------------------------------------

    switch(expression)
    {
        default:

        case Expression::Neutral:
        {
            // 何もしない
            break;
        }

        case Expression::Smile:
        {
            EyeShape smileLeft = defaultLeft;
            EyeShape smileRight = defaultRight;

            smileLeft.height = 18;
            smileRight.height = 18;

            smileLeft.upperArc = 6;
            smileRight.upperArc = 6;

            smileLeft.lowerArc = -6;
            smileRight.lowerArc = -6;

            left = lerp(
                defaultLeft,
                smileLeft,
                amount);

            right = lerp(
                defaultRight,
                smileRight,
                amount);

            break;
        }

        case Expression::Happy:
        {
            EyeShape happyLeft = defaultLeft;
            EyeShape happyRight = defaultRight;

            happyLeft.height = 12;
            happyRight.height = 12;

            happyLeft.upperArc = 10;
            happyRight.upperArc = 10;

            happyLeft.lowerArc = -10;
            happyRight.lowerArc = -10;

            left = lerp(defaultLeft, happyLeft, amount);
            right = lerp(defaultRight, happyRight, amount);

            break;
        }

        case Expression::Angry:
        {
            EyeShape angryLeft = defaultLeft;
            EyeShape angryRight = defaultRight;

            angryLeft.upperSlope = 10;
            angryLeft.lowerSlope = -10;

            angryRight.upperSlope = -10;
            angryRight.lowerSlope = 10;

            angryLeft.height = 26;
            angryRight.height = 26;

            left = lerp(defaultLeft, angryLeft, amount);
            right = lerp(defaultRight, angryRight, amount);

            break;
        }

        case Expression::Sleepy:
        {
            EyeShape sleepyLeft = defaultLeft;
            EyeShape sleepyRight = defaultRight;

            sleepyLeft.cy = 15;
            sleepyRight.cy = 15;

            sleepyLeft.height = 5;
            sleepyRight.height = 5;

            sleepyLeft.radius = 1;
            sleepyRight.radius = 1;


            sleepyLeft.upperArc = 2;
            sleepyRight.upperArc = 2;

            left = lerp(defaultLeft, sleepyLeft, amount);
            right = lerp(defaultRight, sleepyRight, amount);

            break;
        }

        case Expression::Surprise:
        {
            EyeShape surpriseLeft = defaultLeft;
            EyeShape surpriseRight = defaultRight;

            surpriseLeft.width = 30;
            surpriseRight.width = 30;

            surpriseLeft.height = 42;
            surpriseRight.height = 42;

            surpriseLeft.radius = 15;
            surpriseRight.radius = 15;

            left = lerp(defaultLeft, surpriseLeft, amount);
            right = lerp(defaultRight, surpriseRight, amount);

            break;
        }

        case Expression::Sad:
        {
            EyeShape sadLeft = defaultLeft;
            EyeShape sadRight = defaultRight;

            sadLeft.upperSlope = -10;
            sadLeft.lowerSlope = 8;

            sadRight.upperSlope = 10;
            sadRight.lowerSlope = -8;

            left = lerp(defaultLeft, sadLeft, amount);
            right = lerp(defaultRight, sadRight, amount);

            break;
        }

        case Expression::Curious:
        {
            EyeShape curiousLeft = defaultLeft;
            EyeShape curiousRight = defaultRight;

            curiousLeft.height = 40;
            curiousLeft.width = 34;

            curiousRight.height = 24;

            left = lerp(defaultLeft, curiousLeft, amount);
            right = lerp(defaultRight, curiousRight, amount);

            break;
        }
    }
}

//================================== scenario =================================

void FaceAnimator::buildBlinkScenario()
{
    EyeShape closedLeft = currentLeft;
    EyeShape closedRight = currentRight;

    closedLeft.height = 2;
    closedRight.height = 2;

    closedLeft.radius = 1;
    closedRight.radius = 1;

    startLeft = currentLeft;
    startRight = currentRight;

    keyFrameCount = 2;
    currentKeyFrame = 0;

    keyFrames[0].left = closedLeft;
    keyFrames[0].right = closedRight;
    keyFrames[0].duration = 0.08f;

    keyFrames[1].left = currentLeft;
    keyFrames[1].right = currentRight;
    keyFrames[1].duration = 0.10f;

    elapsed = 0.0f;
    playing = true;
}

void FaceAnimator::buildWakeUpScenario()
{
    //----------------------------------------
    // 半目
    //----------------------------------------

    EyeShape sleepyLeft;
    EyeShape sleepyRight;

    createExpression(
        Expression::Sleepy,
        1.0f,
        sleepyLeft,
        sleepyRight);

    //----------------------------------------
    // パッチリ
    //----------------------------------------

    EyeShape surpriseLeft;
    EyeShape surpriseRight;

    createExpression(
        Expression::Surprise,
        1.0f,
        surpriseLeft,
        surpriseRight);

    //----------------------------------------
    // 現在要求されている表情
    //----------------------------------------

    EyeShape targetL;
    EyeShape targetR;

    createExpression(
        requestedExpression,
        requestedAmount,
        targetL,
        targetR);

    //----------------------------------------

    startLeft = currentLeft;
    startRight = currentRight;

    keyFrameCount = 3;
    currentKeyFrame = 0;

    //----------------------------------------
    // Sleepy
    //----------------------------------------

    keyFrames[0].left = sleepyLeft;
    keyFrames[0].right = sleepyRight;
    keyFrames[0].duration = 0.25f;

    //----------------------------------------
    // Surprise
    //----------------------------------------

    keyFrames[1].left = surpriseLeft;
    keyFrames[1].right = surpriseRight;
    keyFrames[1].duration = 0.15f;

    //----------------------------------------
    // 最終表情
    //----------------------------------------

    keyFrames[2].left = targetL;
    keyFrames[2].right = targetR;
    keyFrames[2].duration = 0.25f;

    elapsed = 0.0f;
    playing = true;
}

void FaceAnimator::buildFallAsleepScenario()
{
    EyeShape sleepyLeft;
    EyeShape sleepyRight;

    createExpression(
        Expression::Sleepy,
        1.0f,
        sleepyLeft,
        sleepyRight);

    //----------------------------------------
    // 完全に閉じる
    //----------------------------------------

    EyeShape closedLeft = sleepyLeft;
    EyeShape closedRight = sleepyRight;

    closedLeft.height = 2;
    closedRight.height = 2;

    closedLeft.radius = 1;
    closedRight.radius = 1;

    //----------------------------------------

    startLeft = currentLeft;
    startRight = currentRight;

    keyFrameCount = 2;
    currentKeyFrame = 0;

    //----------------------------------------
    // 半目
    //----------------------------------------

    keyFrames[0].left = sleepyLeft;
    keyFrames[0].right = sleepyRight;
    keyFrames[0].duration = 0.6f;

    //----------------------------------------
    // 閉じる
    //----------------------------------------

    keyFrames[1].left = closedLeft;
    keyFrames[1].right = closedRight;
    keyFrames[1].duration = 0.8f;

    elapsed = 0.0f;
    playing = true;
}