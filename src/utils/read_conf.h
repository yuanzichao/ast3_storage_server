/*
 * read_conf.h
 *
 *  Created on: 2015年5月18日
 *      Author: yuan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define KEYVALLEN 1024

char *l_trim(char * szOutput, const char *szInput);
char *r_trim(char *szOutput, const char *szInput);
char *a_trim(char * szOutput, const char * szInput);
int GetProfileString(char *profile, char *AppName, char *KeyName, char *KeyVal);

