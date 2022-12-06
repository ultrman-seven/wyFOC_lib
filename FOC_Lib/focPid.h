#ifndef B0BD65C0_F511_458E_BA95_C1932AC3E1D7
#define B0BD65C0_F511_458E_BA95_C1932AC3E1D7

typedef struct
{
    float kp, ki, kd;
    float target;
    float err, lastErr, integral, diff;
    float lastLastErr;
} PID_Unit;

float pidUpdate(PID_Unit *o, float val);
float pidIncrementalUpdate(PID_Unit *o, float val);
#endif /* B0BD65C0_F511_458E_BA95_C1932AC3E1D7 */
