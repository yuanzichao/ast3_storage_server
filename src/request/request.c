/*
 * request.c
 *
 *  Created on: 2015年7月20日
 *      Author: yuan
 */

#include "request.h"

int
parse_request(char* request_str) {

	char fileName[MAX_BUF_SIZE];
	char dirName[MAX_BUF_SIZE];
	char diskName[MAX_BUF_SIZE];

	memset(fileName, 0, sizeof(fileName));
	memset(dirName, 0, sizeof(dirName));
	memset(diskName, 0, sizeof(diskName));


	cJSON * requests = cJSON_Parse (request_str);
	cJSON * requests_arr = NULL;
	cJSON * request[REQUEST_NUM] = {};
	cJSON * files_arr = NULL;
	cJSON * file[ASSOCIATE_FILES] = {};

	if (requests) {
		requests_arr = cJSON_GetObjectItem(requests, "requests");

		if (requests_arr) {
			int count;
			count = cJSON_GetArraySize(requests_arr);

			int i;
			for(i=0; i<count; i++) {
				request[i] = cJSON_GetArrayItem(requests_arr, i);
			}

			for(i=0; i<ASSOCIATE_FILES; i++) {
				cJSON* pItem = NULL;

				pItem = cJSON_GetObjectItem (request[i], "diskname");
				memcpy(diskName, pItem->valuestring, strlen(pItem->valuestring));

				pItem = NULL;
				pItem = cJSON_GetObjectItem(request[i], "dirname");
				memcpy(dirName, pItem->valuestring, strlen(pItem->valuestring));


				files_arr = cJSON_GetObjectItem(request[i], "files");
				count = cJSON_GetArraySize(files_arr);
				int j;
				for(j=0; j<count; j++) {
					file[i] = cJSON_GetArrayItem(files_arr, j);
					pItem = NULL;
					pItem = cJSON_GetObjectItem(file[i], "filename");
					memcpy(fileName, pItem->valuestring, strlen(pItem->valuestring));
				}

			}

			cJSON_Delete(requests);
		}
	}

	printf("diskName:%s\n", diskName);
	printf("dirName:%s\n", dirName);
	printf("fileName:%s\n", fileName);

	return 0;
}

