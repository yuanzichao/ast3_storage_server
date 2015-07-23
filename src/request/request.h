/*
 * request.h
 *
 *  Created on: 2015年7月20日
 *      Author: yuan
 */

#include <stdio.h>
#include <stdlib.h>

#include "../utils/cJSON.h"
#include "../sql/sql.h"


#ifndef SRC_REQUEST_REQUEST_H_
#define SRC_REQUEST_REQUEST_H_

char* request_str;

int gen_request(char *fileName, char *dirName, char *diskName);					//生成请求消息


#endif /* SRC_REQUEST_REQUEST_H_ */
