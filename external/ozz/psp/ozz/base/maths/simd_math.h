#pragma once

// Force PSP libstdc++ ctype macros to be consumed before touching _X/_Y/_Z/_W.
#include <locale>

#ifdef _X
    #pragma push_macro("_X")
    #undef _X
    #define LUCARIA_RESTORE_X
#endif
#ifdef _Y
    #pragma push_macro("_Y")
    #undef _Y
    #define LUCARIA_RESTORE_Y
#endif
#ifdef _Z
    #pragma push_macro("_Z")
    #undef _Z
    #define LUCARIA_RESTORE_Z
#endif
#ifdef _W
    #pragma push_macro("_W")
    #undef _W
    #define LUCARIA_RESTORE_W
#endif

#include_next <ozz/base/maths/simd_math.h>

#ifdef LUCARIA_RESTORE_X
    #pragma pop_macro("_X")
    #undef LUCARIA_RESTORE_X
#endif
#ifdef LUCARIA_RESTORE_Y
    #pragma pop_macro("_Y")
    #undef LUCARIA_RESTORE_Y
#endif
#ifdef LUCARIA_RESTORE_Z
    #pragma pop_macro("_Z")
    #undef LUCARIA_RESTORE_Z
#endif
#ifdef LUCARIA_RESTORE_W
    #pragma pop_macro("_W")
    #undef LUCARIA_RESTORE_W
#endif