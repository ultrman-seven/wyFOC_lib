#include "focPid.h"

float pidUpdate(PID_Unit *o, float val)
{
    o->err = o->target - val;
    o->integral += o->err;
    o->diff = o->err - o->lastErr;
    o->lastErr = o->err;

    return (o->err * o->kp + o->integral * o->ki + o->diff * o->kd);
}

float pidIncrementalUpdate(PID_Unit *o, float val)
{
    float p, d;
    o->err = o->target - val;
    p = o->err - o->lastErr;
    d = o->err + o->lastLastErr - 2 * o->lastErr;

    o->lastLastErr = o->lastErr;
    o->lastErr = o->err;

    return (o->kp * p + o->ki * o->err + o->kd * d);
}
