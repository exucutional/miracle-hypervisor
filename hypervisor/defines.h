#pragma once

#ifndef NDEBUG
#define DEBUG(cmd) cmd;
#else
#define DEBUG(cmd) ;
#endif

#define IA32_FEATURE_CONTROL 0x3A
#define IA32_VMX_BASIC       0x480

#define CACHE_TP_WB          6
