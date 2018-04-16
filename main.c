//
//  main.c
//  yum
//
//  Created by Daniel J. Whitehead on 4/15/18.
//  Copyright © 2018 Daniel J. Whitehead. All rights reserved.
//

#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <regex.h>
#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"
#ifdef _WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif
void timestamp();
void create_guid(char guid[]);

int main(int argc, const char * argv[]) {
    FILE *f = fopen("file.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    /* print some text */
    const char *text = "Write this to the file";
    fprintf(f, "Some text: %s\n", text);
    
    /* print integers and floats */
    int i = 1;
    float py = 3.1415927;
    fprintf(f, "Integer: %d, float: %f\n", i, py);
    
    /* printing single chatacters */
    char c = 'A';
    fprintf(f, "A character: %c\n", c);
    
    fclose(f);
    printf("Hell, World!\n");
    timestamp();
    int x;
    char guid[37];
    
    for (x = 10 ; x < 20; x=x+1)
    {
        create_guid(guid);
        printf("%s\n", guid);
        
    }
    return 0;
}
//2018-04-13 14:06:51,191
//2018-04-04 09:59:58,736
void timestamp()
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    printf("%s",asctime( localtime(&ltime) ) );
    return;
}


#ifndef _WIN32
/* This is the linux friendly implementation, but it could work on other
 * systems that have libuuid available
 */
void create_guid(char guid[])
{
    uuid_t out;
    uuid_generate(out);
    uuid_unparse_lower(out, guid);
}
#else
void create_guid(char guid[])
{
    GUID out = {0};
    CoCreateGuid(&out);
    sprintf(guid, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", out.Data1, out.Data2, out.Data3, out.Data4[0], out.Data4[1], out.Data4[2], out.Data4[3], out.Data4[4], out.Data4[5], out.Data4[6], out.Data4[7]);
    for ( ; *guid; ++guid) *guid = tolower(*guid); // To lower one-liner
}
#endif
