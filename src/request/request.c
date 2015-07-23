/*
 * request.c
 *
 *  Created on: 2015年7月20日
 *      Author: yuan
 */

#include "request.h"

int
gen_request(char *fileName, char *dirName, char *diskName) {

	//请求文件的uuid值及MD5值
	char disk_uuid[MAX_BUF_SIZE];
	char md5[MAX_BUF_SIZE];

	//文件的基本信息
	db_file_info* file_info;
	file_info = NULL;

	//查询文件基本信息
	file_info = get_file_info(fileName, dirName, diskName);

	if(file_info == NULL) {
		return EXIT_FAILURE;
	}

	//获取uuid
	memset(disk_uuid, 0, sizeof(disk_uuid));
	memcpy(disk_uuid, file_info->disk_uuid, sizeof(disk_uuid));

	//获取md5
	memset(md5, 0, sizeof(md5));
	memcpy(md5, file_info->md5, sizeof(md5));


	cJSON * requests = NULL;
	cJSON * requests_arr = NULL;
	cJSON * request = NULL;
	cJSON * files_arr = NULL;
	cJSON * file1 = NULL;
//	cJSON * file2 = NULL;

	requests = cJSON_CreateObject();

	requests_arr = cJSON_CreateArray();

	request = cJSON_CreateObject();

	files_arr = cJSON_CreateArray();

	file1 = cJSON_CreateObject();
//	file2 = cJSON_CreateObject();


	cJSON_AddStringToObject(file1, "filename", fileName);
	cJSON_AddStringToObject(file1, "md5", md5);
//	cJSON_AddStringToObject(file2, "filename", "20150716_002.fis");
//	cJSON_AddStringToObject(file2, "md5", "446465131651321654132157");

	cJSON_AddItemToObject(files_arr, "test", file1);
//	cJSON_AddItemToObject(files_arr, "test", file2);


	cJSON_AddStringToObject(request, "uuid", disk_uuid);

	cJSON_AddItemToObject(request, "files", files_arr);

	cJSON_AddItemToObject(requests_arr, "test", request);

	cJSON_AddItemToObject(requests, "requests", requests_arr);


	request_str = cJSON_Print(requests);

	printf("%s\n", request_str);



    return EXIT_SUCCESS; // 返回成功
}

