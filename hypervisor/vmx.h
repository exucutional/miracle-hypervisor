#pragma once

typedef struct vmm {
    BYTE vmxon_region[4096];
    BYTE vmcs_region[4096];
}__attribute__((aligned(4096))) vmm_t;

bool vmx_support_check();
bool vmx_init();
bool vmx_on();
