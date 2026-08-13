#pragma once

#include "eye_renderer.h"
#include "face_types.h"


class FaceAnimator
{
public:

    bool begin();
    
    void update(float dt);
    
    void requestExpression(
        Expression expression,
        float amount = 1.0f);

    void playAction(Action action);
    
    const EyeShape& getLeftEye() const;
    const EyeShape& getRightEye() const;
        

private:

    EyeShape startLeft;
    EyeShape currentLeft;
    EyeShape targetLeft;

    EyeShape startRight;
    EyeShape currentRight;
    EyeShape targetRight;

    float elapsed = 0.0f;
    float duration = 0.3f;

    struct KeyFrame
    {
        EyeShape left;
        EyeShape right;

        float duration;
    };

    static constexpr int MAX_KEYFRAME = 8;

    KeyFrame keyFrames[MAX_KEYFRAME];

    int keyFrameCount = 0;
    int currentKeyFrame = 0;

    bool playing = false;

private:

    // 現在表示しているExpression
    Expression currentExpression = Expression::Neutral;
    float currentAmount = 1.0f;

    // 上位層から要求されているExpression
    Expression requestedExpression = Expression::Neutral;
    float requestedAmount = 1.0f;

    void createExpression(
        Expression expression,
        float amount,
        EyeShape& left,
        EyeShape& right);

    static EyeShape lerp(
        const EyeShape& a,
        const EyeShape& b,
        float t);

    void buildBlinkScenario();
    void buildWakeUpScenario();
    void buildFallAsleepScenario();
};

