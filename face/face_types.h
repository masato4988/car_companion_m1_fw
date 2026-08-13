#pragma once

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

enum class Action
{
    None,
    Blink,
    DoubleBlink,
    WakeUp,
    FallAsleep,
    LookLeft,
    LookRight,
    LookAround
};