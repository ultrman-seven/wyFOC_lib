#include "FOCcore.h"
#include "stdlib.h"
#include "math.h"

#include "focPid.h"
#include "stdint.h"
typedef struct
{
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
    PID_Unit pidA = {.kd = 0, .target = 0};
    PID_Unit pidB = {.kd = 0};

    core->id = &pidA;
    core->iq = &pidB;
}

void FOC_UpdateFunction(FOC_Core *core)
{
    float iDetA, iDetB, iDetC; // Ia Ib and Ic
    float iDetAlpha, iDetBeta; // Iα and Iβ
    float iDetQ, iDetD;        // Iq and Id
    float theta;               // 转子位置θ
    float st, ct;              // sin θ and cos θ

    float Uq, Ud;
    float U_alpha, U_beta;

    uint32_t pwmA, pwmB, pwmC;

    // 读AB两相电流
    iDetA = core->getIa();
    iDetB = core->getIb();

    // 估计转子位置
    theta = core->getTheta();

    // C相电流
    iDetC = -(iDetA + iDetB);

    // clark
    iDetAlpha = iDetA - 0.5 * (iDetB + iDetC);
    iDetBeta = 0.8660254037844386 * (iDetB - iDetC); // \frac{\sqrt3}2 * (I_b - I_c)

    // park
    st = sinf(theta);
    ct = cosf(theta);

    iDetD = iDetAlpha * ct + iDetBeta * st;
    iDetQ = iDetBeta * ct - iDetAlpha * st;

    // PID (PI*2)
    Uq = pidUpdate(core->iq, iDetQ);
    Ud = pidUpdate(core->id, iDetD);

    // inverse park
    U_alpha = Ud * ct - Uq * st;
    U_beta = Ud * st + Uq * ct;

    // SVPWM
    // 计算过程
    

    core->PWM_Set(pwmA, pwmB, pwmC);
}
