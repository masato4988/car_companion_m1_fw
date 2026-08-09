#include "face_animator.h"

bool FaceAnimator::begin()
{
    setExpression(Expression::Neutral,1.0f,0.0f);

    startLeft = targetLeft;
    currentLeft = targetLeft;

    startRight = targetRight;
    currentRight = targetRight;

    return true;
}

void FaceAnimator::update(float dt)
{
    elapsed += dt;

    float t;

    if(duration <= 0.0f)
    {
        t = 1.0f;
    }
    else
    {
        t = elapsed / duration;

        if(t > 1.0f)
            t = 1.0f;
    }

    currentLeft =
        lerp(
            startLeft,
            targetLeft,
            t);

    currentRight =
        lerp(
            startRight,
            targetRight,
            t);
}

void FaceAnimator::setExpression(
    Expression expression,
    float amount,
    float transitionTime)
{
    startLeft = currentLeft;
    startRight = currentRight;

    createExpression(
        expression,
        amount,
        targetLeft,
        targetRight);

    elapsed = 0.0f;
    duration = transitionTime;
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

const EyeShape& FaceAnimator::getLeftEye() const
{
    return currentLeft;
}

const EyeShape& FaceAnimator::getRightEye() const
{
    return currentRight;
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