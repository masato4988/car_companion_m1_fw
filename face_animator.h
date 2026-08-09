#pragma once

#include "eye_renderer.h"

enum class Expression
{
    Neutral,

    Smile,
    Happy,

    Angry,
    Sad,

    Sleepy,

    Surprise,

    Curious
};

class FaceAnimator
{
public:

    bool begin();

    void update(float dt);

    void setExpression(
        Expression expression,
        float amount = 1.0f,
        float transitionTime = 0.3f);

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

private:

    void createExpression(
        Expression expression,
        float amount,
        EyeShape& left,
        EyeShape& right);

    static EyeShape lerp(
        const EyeShape& a,
        const EyeShape& b,
        float t);
};