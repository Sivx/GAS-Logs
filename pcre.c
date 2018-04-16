#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcre.h"


/* -------------------------------------------------------------------------
 * private functions
 * ------------------------------------------------------------------------- */

static char* read_regs(char* regs_file)
{
    int             size;
    char            string[PCRE_VAL_SIZE+1];
    int             cnt_line;
    FILE*           fp;
    char*           pattern;
    int             capa = PCRE_VAL_SIZE;
    int             limit = PCRE_VAL_SIZE;
    char*           realloc_ptr;
    
    fp = fopen(regs_file, "r");
    if( !fp ) {
        fprintf(stderr,"file open error : %s\n", regs_file);
        return NULL;
    }
    pattern = malloc(sizeof(char)*PCRE_VAL_SIZE+10);
    if( !pattern ) {
        fprintf(stderr,"malloc fail\n");
        return NULL;
    }
    
    *pattern = '\0';
    
    cnt_line = 0;
    while(fgets(string, PCRE_VAL_SIZE, fp) != NULL) {
        size = strlen(string);
        if(string[size-1] == '\n'){
            string[size-1] = '\0';
            --size;
        }
        if(size > 1 && string[size-1] == '\r'){
            string[size-1] = '\0';
            --size;
        }
        if(string[0] == '\0')
            continue;
        
        // re-sizing
        if( limit - size <= 100 ) {
            capa = capa * 2;
            limit = capa + limit;
            realloc_ptr = realloc(pattern, capa * 2);
            if( !realloc_ptr ) {
                fprintf(stderr,"realloc fail\n");
                return NULL;
            }
            pattern = realloc_ptr;
        }
        
        if(cnt_line==0) {
            strcat(pattern, string);
            limit -= strlen(string);
        } else {
            strcat(pattern, "|");
            limit -= 1;
            strcat(pattern, string);
            limit -= strlen(string);
        }
        
        cnt_line++;
    }
    
    fclose(fp);
    return pattern;
    
}

/* -------------------------------------------------------------------------
 * public functions
 * ------------------------------------------------------------------------- */

pcre2_code* build_pcre(char* regs_file)
{
    pcre2_code*         re;
    char*               pattern;
    PCRE2_SIZE          erroffset;
    int                 errorcode;
    PCRE2_UCHAR8        buffer[120];
    
    pattern = read_regs(regs_file);
    if( !pattern ) {
        return NULL;
    }
    
    re = pcre2_compile((PCRE2_SPTR)pattern, -1, PCRE2_UTF, &errorcode, &erroffset, NULL);
    if ( re == NULL ) {
        (void)pcre2_get_error_message(errorcode, buffer, 120);
        fprintf(stderr,"%d\t%s\n", errorcode, buffer);
        return NULL;
    }
    
    if( pattern ) free(pattern);
    return re;
}

void destroy_pcre(pcre2_code* re)
{
    if( re ) {
        pcre2_code_free(re);
    }
}

void match_pcre(pcre2_code* re, char* string)
{
    pcre2_match_data*   match_data;
    PCRE2_SIZE*         ovector;
    int                 rc;
    int                 i;
    int                 begin;
    int                 len;
    char                chr;
    
    fprintf(stdout, "%s\n", string);
    match_data = pcre2_match_data_create(3*10, NULL);
    rc = pcre2_match(re, (PCRE2_SPTR)string, -1, 0, 0, match_data, NULL);
    if( rc > 0 ) {
        ovector = pcre2_get_ovector_pointer(match_data);
        for(i = 0; i < rc; i++) {
            begin = ovector[2*i];
            len   = ovector[2*i+1] - ovector[2*i];
            chr = string[begin+len];
            string[begin+len] = '\0';
            fprintf(stdout, "%d\t%d\t%d\t%s\n", i, begin, begin+len, &string[begin]);
            string[begin+len] = chr;
        }
    }
    fprintf(stdout, "\n");
    pcre2_match_data_free(match_data);
}
