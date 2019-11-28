#include "SDL_endian.h"

#ifndef _BRHTON_H_

#if SDL_BYTEORDER == SDL_LIL_ENDIAN

#define BrBStoHS(x)         SDL_Swap16(x)
#define BrBLtoHL(x)         SDL_Swap32(x)
#define BrBFtoHF(x)         BrSwapFloat(x)
#define BrLStoHS(x)         (x)
#define BrLLtoHL(x)         (x)
#define BrLFtoHF(x)         (x)

#define BrNtoHL(x)          SDL_Swap32(x)
#define BrHtoNL(x)          SDL_Swap32(x)
#define BrNtoHS(x)          SDL_Swap16(x)
#define BrHtoNS(x)          SDL_Swap16(x)

#define BrHtoNF(x)          BrSwapFloat(x)
#define BrNtoHF(x)          BrSwapFloat(x)

#else

#define BrBStoHS(x)         (x)
#define BrBLtoHL(x)         (x)
#define BrBFtoHF(x)         (x)
#define BrLStoHS(x)         SDL_Swap16(x)
#define BrLLtoHL(x)         SDL_Swap32(x)
#define BrLFtoHF(x)         BrSwapFloat(x)

#define BrHtoNL(x) (x)
#define BrNtoHL(x) (x)
#define BrHtoNS(x) (x)
#define BrNtoHS(x) (x)

#define BrHtoNF(x) (x)
#define BrNtoHF(x) (x)

#endif

#define BrSwap32(x)         SDL_Swap32(x)
float BrSwapFloat(float value);

#define _BRHTON_H_
#endif
