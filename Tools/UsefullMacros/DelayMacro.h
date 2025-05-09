#pragma once

//params should be in brackets, for example (1,2,3,4)
//used to delay the macro expansion, for example when "params" are 
//some macros that needed to expand before the "macro"
#define KE2_DelayMacro_Macro(macro,params) macro params
