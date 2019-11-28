#include "brender.h"
#include "brhton.h"

float BrSwapFloat(float value) {
    float r;
    char *in  = (char*)&value;
    char *out = (char*)&r;

    out[0] = in[3];
    out[1] = in[2];
    out[2] = in[1];
    out[3] = in[0];

    return r;
}
