#ifndef B2AEE714_8C01_4945_B954_C0938BD7AC46
#define B2AEE714_8C01_4945_B954_C0938BD7AC46

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

#endif /* B2AEE714_8C01_4945_B954_C0938BD7AC46 */
