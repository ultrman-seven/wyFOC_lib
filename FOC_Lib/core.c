#include "FOCcore.h"
#include "stdlib.h"
#include "math.h"

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

void __sector1Time(float alpha, float beta, float *time)
{ // 0467
    time[1] = 0.8660254037844386 * alpha - 0.5 * beta;
    time[2] = beta;
}
void __sector2Time(float alpha, float beta, float *time)
{ // 0267
    time[1] = -(0.8660254037844386 * alpha - 0.5 * beta);
    time[2] = 0.8660254037844386 * alpha + 0.5 * beta;
}
void __sector3Time(float alpha, float beta, float *time)
{ // 0237
    time[1] = beta;
    time[2] = -0.8660254037844386 * alpha - 0.5 * beta;
}
void __sector4Time(float alpha, float beta, float *time)
{ // 0137
    time[1] = -beta;
    time[2] = -(0.8660254037844386 * alpha - 0.5 * beta);
}
void __sector5Time(float alpha, float beta, float *time)
{ // 0157
    time[1] = -0.8660254037844386 * alpha - 0.5 * beta;
    time[2] = 0.8660254037844386 * alpha - 0.5 * beta;
}
void __sector6Time(float alpha, float beta, float *time)
{ // 0457
    time[1] = 0.8660254037844386 * alpha + 0.5 * beta;
    time[2] = -beta;
}

/**
 * @brief 根据公式计算除T0T7以外的时间
 * @param float alpha Ua
 * @param float beta Ub
 * @param float *time 计算出的时间
 */
void (*__timeCompute[8])(float, float, float *) = {__sector1Time, __sector2Time, __sector3Time, __sector4Time, __sector5Time, __sector6Time};

/**
 * @brief 已经看不懂当时为什么这么写了，很震惊。
 */
const uint8_t sectorHighPosition[8][3] = {
    {1, 2, 3},
    {2, 1, 3},
    {3, 1, 2},
    {3, 2, 1},
    {1, 3, 2},
    {2, 3, 1}};

/**
 * @brief 计算每相PWM有效时间。
 * 隔得太久没看了，已经看不懂为什么这么写了
 * @param uint8_t p
 * @param float *t
 */
static float __computePWM_duty(uint8_t p, float *t)
{
    float result = 0;
    while (p < 4)
        result += t[p++];
    return result;
}

float UQ_val = 0.5;
float UD_val = 0.1;

void FOC_UpdateFunction(FOC_Core *core)
{
    float iDetA, iDetB, iDetC; // Ia Ib and Ic
    float theta;               // 转子位置θ
    // 读AB两相电流
    iDetA = 1; // core->getIa();
    iDetB = 1; // core->getIb();
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
    Uq = UQ_val; // pidUpdate(core->iq, iDetQ);
    Ud = UD_val; // pidUpdate(core->id, iDetD);

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
    ctrlTimes[1] *= core->T / 4.849742261192857;
    ctrlTimes[2] *= core->T / 4.849742261192857;
    ctrlTimes[0] = 0.5 * (core->T - ctrlTimes[1] - ctrlTimes[2]);
    ctrlTimes[3] = ctrlTimes[0];

    pwmA = __computePWM_duty(sectorHighPosition[sector][0], ctrlTimes);
    pwmC = __computePWM_duty(sectorHighPosition[sector][1], ctrlTimes);
    pwmB = __computePWM_duty(sectorHighPosition[sector][2], ctrlTimes);

    core->PWM_Set(pwmA, pwmB, pwmC);
}
