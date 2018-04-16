#ifdef __cplusplus
extern "C" {
#endif
    
#ifndef PCRE_H
#define PCRE_H
    
#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"
    
#define PCRE_VAL_SIZE   1024
    
    pcre2_code* build_pcre(char* regs_file);
    void        destroy_pcre(pcre2_code* re);
    void        match_pcre(pcre2_code* re, char* string);
    
#endif
    
#ifdef __cplusplus
}
#endif
