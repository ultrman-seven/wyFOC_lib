#include "FOCcore.h"
#include "stdlib.h"
#include "math.h"

#include "focPid.h"
#include "stdint.h"
typedef struct
{
    float Udc;
    union
    {
        uint32_t Period;
        uint32_t T;
    };

    float (*getIa)(void);
    float (*getIb)(void);
    float (*getTheta)(void);
    PID_Unit *iq;
    PID_Unit *id;
    PID_Unit *speed;
    PID_Unit *position;
    void (*PWM_Set)(uint32_t A, uint32_t B, uint32_t C);
} FOC_Core;

void FOC_Init(FOC_Core *core);
void FOC_UpdateFunction(FOC_Core *core);

void FOC_Init(FOC_Core *core)
{
    PID_Unit pidID = {.kd = 0, .target = 0};
    PID_Unit pidIQ = {.kd = 0};
    PID_Unit pidSpeed = {.kd = 0};
    PID_Unit pidPosi = {.kd = 0};

    core->id = &pidID;
    core->iq = &pidIQ;
    core->speed = &pidSpeed;
    core->position = &pidPosi;
}

const uint8_t sectorHighPosition[8][3] = {
    {1, 2, 3},
    {2, 1, 3},
    {3, 1, 2},
    {3, 2, 1},
    {2, 3, 1},
    {1, 3, 2}};

void (*__timeCompute[8])(float,float, float *);

static float __computePWM_duty(uint8_t p, float *t)
{
    float result = 0;
    while (p < 4)
        result += t[p++];
    return result;
}

void FOC_UpdateFunction(FOC_Core *core)
{
    float iDetA, iDetB, iDetC; // Ia Ib and Ic
    float theta;               // 转子位置θ
    // 读AB两相电流
    iDetA = core->getIa();
    iDetB = core->getIb();
    // 估计转子位置
    theta = core->getTheta();
    // C相电流
    iDetC = -(iDetA + iDetB);

    float iDetAlpha, iDetBeta; // Iα and Iβ
    // clark
    iDetAlpha = iDetA - 0.5 * (iDetB + iDetC);
    iDetBeta = 0.8660254037844386 * (iDetB - iDetC); // \frac{\sqrt3}2 * (I_b - I_c)

    float iDetQ, iDetD; // Iq and Id
    float st, ct;       // sin θ and cos θ
    // park
    st = sinf(theta);
    ct = cosf(theta);

    iDetD = iDetAlpha * ct + iDetBeta * st;
    iDetQ = iDetBeta * ct - iDetAlpha * st;

    float Uq, Ud;
    // PID (PI*2)
    Uq = pidUpdate(core->iq, iDetQ);
    Ud = pidUpdate(core->id, iDetD);

    float U_alpha, U_beta;
    // inverse park
    U_alpha = Ud * ct - Uq * st;
    U_beta = Ud * st + Uq * ct;

    // SVPWM
    // 计算过程
    uint32_t pwmA, pwmB, pwmC;
    uint8_t sector;
    float U_ref;

    sector = theta / 60;
    U_ref = sqrtf(U_alpha * U_alpha + U_beta * U_beta);

    float ctrlTimes[4];

    __timeCompute[sector](U_alpha, U_beta, ctrlTimes);

    pwmA = __computePWM_duty(sectorHighPosition[sector][0], ctrlTimes) * core->T;
    pwmC = __computePWM_duty(sectorHighPosition[sector][1], ctrlTimes) * core->T;
    pwmB = __computePWM_duty(sectorHighPosition[sector][2], ctrlTimes) * core->T;

    core->PWM_Set(pwmA, pwmB, pwmC);
}
