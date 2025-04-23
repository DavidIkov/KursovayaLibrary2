#pragma once
#include"DelayMacro.h"

#define _KE2_HELPER_EndOfPairsLoop_Macro(...) __VA_ARGS__##_End
#define _KE2_HELPER_RecursiveLoopA_Macro(...) KE2_MacroForLoop(__VA_ARGS__) _KE2_HELPER_RecursiveLoopB_Macro
#define _KE2_HELPER_RecursiveLoopB_Macro(...) KE2_MacroForLoop(__VA_ARGS__) _KE2_HELPER_RecursiveLoopA_Macro
#define _KE2_HELPER_RecursiveLoopA_Macro_End
#define _KE2_HELPER_RecursiveLoopB_Macro_End

//you need to define KE2_MacroForLoop(parameters you provided)
#define KE2_ApplyMacroInLoop_Macro(...) KE2_DelayMacro_Macro(_KE2_HELPER_EndOfPairsLoop_Macro,(_KE2_HELPER_RecursiveLoopA_Macro __VA_ARGS__))

/*example of use case
#define KE2_MacroForLoop(type,name,val) type name = val;
KE2_ApplyMacroInLoop_Macro((int,b,1)(bool,uuu,33)(double,idk,1032.3))

will result in:
int b = 1; bool uuu = 33; double idk = 1032.3;
*/

