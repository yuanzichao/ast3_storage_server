#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sql.h"
#include "../utils/read_conf.h"

//数据库连接信息
char g_host_name[MAX_BUF_SIZE];
char g_user_name[MAX_BUF_SIZE];
char g_password[MAX_BUF_SIZE];
char g_db_name[MAX_BUF_SIZE];
char g_db_port[MAX_BUF_SIZE];


/**
 * 初始化数据库连接
 */
int
init_mysql() {

	//获取数据库连接信息
	GetProfileString("./etc/ast3_db_info.conf", "DB_INFO", "HostName", g_host_name);
	GetProfileString("./etc/ast3_db_info.conf", "DB_INFO", "UserName", g_user_name);
	GetProfileString("./etc/ast3_db_info.conf", "DB_INFO", "Password", g_password);
	GetProfileString("./etc/ast3_db_info.conf", "DB_INFO", "DBName", g_db_name);
	GetProfileString("./etc/ast3_db_info.conf", "DB_INFO", "DBPort", g_db_port);

	//初始化
    g_conn = mysql_init(NULL);

    //连接数据库
    if(!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, atoi(g_db_port), NULL, 0)) // 如果失败
        return EXIT_FAILURE;

    //初始化orm
    db_init(g_conn);

    return EXIT_SUCCESS; // 返回成功
}


/**
 * 查询所有硬盘信息--硬盘名
 */
int
query_disks(){

    char sql[MAX_BUF_SIZE];
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "SELECT disk_name FROM `disk_info`");

    if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    }

    g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

    return EXIT_SUCCESS;
}


/**
 * 查询所有硬盘基本信息--id,name,容量，权限，状态
 */
int
query_primary_disks(){

    char sql[MAX_BUF_SIZE];
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "SELECT disk_id,disk_name,permisssion,disk_capacity,disk_status FROM `disk_info`");

    if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    }

    g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

    return EXIT_SUCCESS;
}


/**
 * 查询所有硬盘详细信息
 */
int
query_all_disks(){

    char sql[MAX_BUF_SIZE];
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "SELECT * FROM `disk_info`");

    if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    }

    g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

    return EXIT_SUCCESS;
}


/**
 * 获取磁盘ID
 */
char*
get_disk_id(char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT disk_id FROM `disk_info` Where disk_name = '%s'", diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	char *disk_id = (char *) malloc(MAX_BUF_SIZE);
	int i;
	while ((g_row = mysql_fetch_row(g_res))) {
		for(i=0; i<get_fields(); i++){
			sprintf(disk_id, "%s", g_row[i]);   //获取目录ID
		}
	}

	return disk_id;
}


/**
 * 查询指定硬盘信息
 */
int
query_disk(char *diskName){

    char sql[MAX_BUF_SIZE];
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "SELECT * FROM `disk_info` Where disk_name = '%s'", diskName);

    if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    }

    g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

    return EXIT_SUCCESS;
}


/**
 * 获取db_disk_info
 */
db_disk_info*
get_disk_info(char *diskName){

	//获取磁盘ID
	char *disk_id = (char *) malloc(MAX_BUF_SIZE);
	disk_id = get_disk_id(diskName);
	free_result();

	//采用orm获取disk_info
	db_disk_info *disk_info;
	disk_info = db_disk_info__new();
	disk_info = db_disk_info__get_by_id(atoi(disk_id));

    return disk_info;
}


/**
 * 插入磁盘信息
 */
int
insert_disk(db_disk_info *disk_info){

      if (db_disk_info__insert(disk_info) != 0){
    	  print_mysql_error(NULL);
          return EXIT_FAILURE;
       }

      printf("insert data into disk_info successfully\n");
      return EXIT_SUCCESS;
  }


/**
 * 更新磁盘信息
 * 包括 `disk_used`, `recent_use_time`, `disk_status`
 */
void
update_disk(db_disk_info *disk_info) {

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));

	//格式化时间
	struct tm * tm_local = localtime(&disk_info->recent_use_time);
	char str_f_t [MAX_BUF_SIZE];
	strftime(str_f_t, sizeof(str_f_t), "%G-%m-%d %H:%M:%S", tm_local);

    sprintf(sql, "UPDATE `disk_info` SET disk_used = %d, recent_use_time = '%s', disk_status = %d WHERE  disk_id = %d",
    		disk_info->disk_used, str_f_t, disk_info->disk_status, disk_info->disk_id);


	int res = mysql_query(g_conn, sql);
	if (!res) {
		printf("Update %lu rows\n", (unsigned long)mysql_affected_rows(g_conn));
	} else {
		print_mysql_error(NULL);
	}
}


/**
 * 查询硬盘内目录信息
 */
int
query_disks_info(char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT directory_name FROM `directory_info` Where disk_name = '%s' AND parent_id = 0", diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 查询硬盘内目录基本信息--目录id，目录名，目录权限，目录大小，创建时间
 */
int
query_disks_primary_info(char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT directory_id,directory_name,directory_size,time,permission FROM `directory_info` Where disk_name = '%s' AND parent_id = 0", diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 查询硬盘内目录信息
 */
int
query_disks_all_info(char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `directory_info` Where disk_name = '%s' AND parent_id = 0", diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 返回指定ID的目录
 */
char*
return_directory(char *dirID, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT directory_name FROM `directory_info` Where directory_id = '%s' AND disk_name = '%s'", dirID, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	char *directory = (char *) malloc(MAX_BUF_SIZE);
	int i;
	g_row = mysql_fetch_row(g_res);
	sprintf(directory,"%s", g_row[i]);

	return directory;
}


/**
 * 获取父目录ID
 */
char*
get_parent_id(char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT parent_id FROM `directory_info` Where directory_name = '%s' AND disk_name = '%s'", dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	char *dirId = (char *) malloc(MAX_BUF_SIZE);
	int i;
	g_row = mysql_fetch_row(g_res);
	sprintf(dirId,"%s", g_row[i]);

	return dirId;
}

/**
 * 获取目录ID
 */
char*
get_directory_id(char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT directory_id FROM `directory_info` Where directory_name = '%s' AND disk_name = '%s'", dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	char *dirId = (char *) malloc(MAX_BUF_SIZE);
	int i;
	while ((g_row = mysql_fetch_row(g_res))) {
		for(i=0; i<get_fields(); i++){
			sprintf(dirId, "%s", g_row[i]);   //获取目录ID
		}
	}

	return dirId;
}


/**
 * 获取db_directory_info
 */
db_directory_info*
get_directory_info(char *dirName, char *diskName){

	//获取目录ID
	char *dir_id = (char *) malloc(MAX_BUF_SIZE);
	dir_id = get_directory_id(dirName, diskName);
	free_result();

	//采用orm获取directory_info
	db_directory_info *directory_info;
	directory_info = db_directory_info__new();
	directory_info = db_directory_info__get_by_id(atoi(dir_id));

	return directory_info;
}


/**
 * 插入目录信息
 */
int
insert_directory(db_directory_info *directory_info){

      if (db_directory_info__insert (directory_info) != 0){
    	  print_mysql_error(NULL);
          return EXIT_FAILURE;
       }

      printf("insert data into directory_info successfully\n");
      return EXIT_SUCCESS;
  }


/**
 * 更新目录信息
 * 包括 `directory_size`, `recent_use_time`, `accessed_time`, `file_number`
 */
void
update_directory(db_directory_info *directory_info){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));

	//格式化时间
	struct tm * tm_local = localtime(&directory_info->recent_use_time);
	char str_f_t [MAX_BUF_SIZE];
	strftime(str_f_t, sizeof(str_f_t), "%G-%m-%d %H:%M:%S", tm_local);

    sprintf(sql, "UPDATE `directory_info` SET directory_size = %d, recent_use_time = '%s', accessed_time = %d, file_number = %d WHERE  directory_id = %d",
    		directory_info->directory_size, str_f_t,
			directory_info->accessed_time, directory_info->file_number,
			directory_info->directory_id);


	int res = mysql_query(g_conn, sql);
	if (!res) {
		printf("Update %lu rows\n", (unsigned long)mysql_affected_rows(g_conn));
	} else {
		print_mysql_error(NULL);
	}
}


/**
 * 查询目录内文件和目录信息
 */
int
query_directory_info(char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT file_name FROM `file_info` Where directory_name = '%s' AND disk_name = '%s'", dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 查询目录内文件和目录详细信息
 */
int
query_directory_primary_info(char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT file_id,file_name,file_size,time,permission FROM `file_info` Where directory_name = '%s' AND disk_name = '%s'", dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 查询目录内文件和目录详细信息
 */
int
query_directory_all_info(char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where directory_name = '%s' AND disk_name = '%s'", dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


int
query_directory_wildcard_info(char* fileName,char* dirName,char* diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where file_name LIKE '%%s' AND directory_name = '%s' AND disk_name = '%s'", fileName,dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;

}
/**
 * 获取文件ID
 */
char*
get_file_id(char *fileName, char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT file_id FROM `file_info` Where file_name = '%s' AND directory_name = '%s' AND disk_name = '%s'", fileName, dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	char *file_id = (char *) malloc(MAX_BUF_SIZE);
	int i;
	while ((g_row = mysql_fetch_row(g_res))) {
		for(i=0; i<get_fields(); i++){
			sprintf(file_id, "%s", g_row[i]);   //获取目录ID
		}
	}

	return file_id;
}


/**
 * 获取指定文件基本信息
 */
int
get_file_primary_info(char *fileName, char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT file_id,file_name,file_size,time,permission FROM `file_info` Where directory_name = '%s' AND disk_name = '%s' AND file_name = '%s'", dirName, diskName,fileName);

	if (mysql_query(g_conn, sql)){
			print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 获取指定文件详细信息
 */
int
get_file_all_info(char *fileName, char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where directory_name = '%s' AND disk_name = '%s' AND file_name = '%s'", dirName, diskName,fileName);

	if (mysql_query(g_conn, sql)){
			print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 获取db_file_info
 */
db_file_info*
get_file_info(char *fileName, char *dirName, char *diskName){

	//获取磁盘ID
	char *file_id = (char *) malloc(MAX_BUF_SIZE);
	file_id = get_file_id(fileName, dirName, diskName);
	free_result();

	//采用orm获取file_info
	db_file_info *file_info;
	file_info = db_file_info__new();
	file_info = db_file_info__get_by_id(atoi(file_id));

    return file_info;
}


/**
 * 插入文件信息
 */
int
insert_file(db_file_info *file_info){

      if (db_file_info__insert (file_info) != 0){
    	  print_mysql_error(NULL);
          return EXIT_FAILURE;
       }

      printf("insert data into file_info successfully\n");
      return EXIT_SUCCESS;
  }


/**
 * 更新文件信息
 * 包括 `recent_use_time`, `accessed_time`
 */
void
update_file(db_file_info *file_info){
	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));

	//格式化时间
	struct tm * tm_local = localtime(&file_info->recent_use_time);
	char str_f_t [MAX_BUF_SIZE];
	strftime(str_f_t, sizeof(str_f_t), "%G-%m-%d %H:%M:%S", tm_local);

	sprintf(sql, "UPDATE `file_info` SET recent_use_time = '%s', accessed_time = %d WHERE  file_id = %d",
			str_f_t, file_info->accessed_time, file_info->file_id);


	int res = mysql_query(g_conn, sql);
	if (!res) {
		printf("Update %lu rows\n", (unsigned long)mysql_affected_rows(g_conn));
	} else {
		print_mysql_error(NULL);
	}
}


/**
 * 查询文件信息
 */
int
query_file_info(char *fileName, char *dirName, char *diskName){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where file_name = '%s' AND directory_name = '%s' AND disk_name = '%s'", fileName, dirName, diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 根据时间查询文件信息
 */
int
query_file_by_time(char *start_time, char * end_time){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where time >= '%s' AND time <= '%s'", start_time, end_time);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 根据时间查询文件信息
 */
int
query_file_by_time_curr(char *dirName, char *diskName,char *start_time, char * end_time){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where time >= '%s' AND time <= '%s'AND directory_name = '%s' AND disk_name = '%s'", start_time, end_time,dirName,diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 根据位置查询文件信息
 */
int
query_file_by_location(char *start_ra, char *end_ra, char *start_dec, char *end_dec){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where ra_val >= '%s' AND ra_val <= '%s' AND dec_val >= '%s' AND dec_val <= '%s'",
			start_ra, end_ra, start_dec, end_dec);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 根据位置查询文件信息
 */
int
query_file_by_curr_location(char* dirName,char* diskName,char *start_ra, char *end_ra, char *start_dec, char *end_dec){

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `file_info` Where ra_val >= '%s' AND ra_val <= '%s' AND dec_val >= '%s' AND dec_val <= '%s' AND directory_name = '%s' AND disk_name = '%s'",
			start_ra, end_ra, start_dec, end_dec,dirName,diskName);

	if (mysql_query(g_conn, sql)){
		 print_mysql_error(NULL);
	}

	g_res = mysql_store_result(g_conn); // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	return EXIT_SUCCESS;
}


/**
 * 删除记录（不提供）
 */
void
delete() {
}


/**
 * 打印结果
 */
int
print_result(){
	int i;
	while ((g_row = mysql_fetch_row(g_res))) {
		for(i=0; i<get_fields(); i++){
			printf("%s\t", g_row[i]);
		}
		printf("\n");
	}
	return i;
}


/**
 * 打印最后一次错误
 */
void
print_mysql_error(const char *msg){
    if(msg)
        printf("%s: %s\n", msg, mysql_error(g_conn));
    else
        puts(mysql_error(g_conn));
}


/**
 * 获取数据集行数
 */
int
get_rows(){
	return mysql_num_rows(g_res);
}


/**
 * 获取数据集列数
 */
int
get_fields(){
	return mysql_num_fields(g_res);
}


/**
 * 释放结果集
 */
void
free_result(){
	mysql_free_result(g_res);
}

/**
 * 关闭数据库
 */
void
close_db(){
	mysql_close(g_conn); // 关闭链接
}



