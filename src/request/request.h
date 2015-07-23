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


#define ASSOCIATE_FILES 1			//关联文件数
#define REQUEST_NUM 1			//关联文件数

int parse_request(char* request_str);					//生成请求消息


