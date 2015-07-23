/*
 * history.c
 *
 *  Created on: 2015年7月15日
 *      Author: yuan
 */

#include "history.h"

char db_name[MAX_BUF_SIZE];


/**
 * 记录操作日志
 */
int
insert_history(char* operation) {

	char sql[MAX_BUF_SIZE];
    memset(sql, 0, sizeof(sql));

	time_t cur_time = time(NULL);		//获取系统当前时间并格式化

	struct tm *timeinfo;
	timeinfo = localtime(&cur_time);

	char date[MAX_BUF_SIZE];
	memset(date, 0, sizeof(date));

	strftime(date, sizeof(date), "history_%Y%m%d", timeinfo);

	char time[MAX_BUF_SIZE];
	memset(time, 0, sizeof(time));

	strftime(time, sizeof(time), "%F %T", timeinfo);
	sprintf(sql, "CREATE TABLE IF NOT EXISTS `%s` ( `id` int(11) NOT NULL AUTO_INCREMENT, `operation` varchar(255) NOT NULL,`time` datetime NOT NULL, PRIMARY KEY (`id`))", date);


	if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    	 return EXIT_FAILURE;
    }

	//插入数据
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "INSERT INTO `%s`(operation, time)VALUES ('%s', '%s')", date, operation, time);


	if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    	 return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;

}


/**
 * 获取当天操作纪录信息
 */
int
get_cur_history() {

	char sql[MAX_BUF_SIZE];
    memset(sql, 0, sizeof(sql));

	time_t cur_time = time(NULL);		//获取系统当前时间并格式化

	struct tm *timeinfo;
	timeinfo = localtime(&cur_time);

	char date[MAX_BUF_SIZE];
	memset(date, 0, sizeof(date));

	strftime (date, sizeof(date), "history_%Y%m%d", timeinfo);



	sprintf(sql, "SELECT * FROM `%s`", date);


	if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    	 return EXIT_FAILURE;
    }

    g_res = mysql_store_result(g_conn); //从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	print_result();		//打印
	free_result();		//释放结果集

    return EXIT_SUCCESS;

}


/**
 * 获取指定日期的操作纪录信息
 */
int
get_spe_history(char* date) {

	char sql[MAX_BUF_SIZE];
	memset(sql, 0, sizeof(sql));
	sprintf(sql, "SELECT * FROM `history_%s`", date);

	if (mysql_query(g_conn, sql)){
    	 return EXIT_FAILURE;
    }

    g_res = mysql_store_result(g_conn); //从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	print_result();		//打印
	free_result();		//释放结果集

	return EXIT_SUCCESS;
}


/**
 * 获取所有的操作纪录信息
 */
int
get_all_history() {

    char sql[MAX_BUF_SIZE];
    memset(sql, 0, sizeof(sql));

    GetProfileString("./etc/ast3_db_info.conf", "DB_INFO", "DBName", db_name);
    sprintf(sql, "SELECT table_name FROM information_schema.tables WHERE table_schema='%s' AND table_type='base table' AND table_name LIKE 'history_%s'", db_name, "%");

    if (mysql_query(g_conn, sql)){
    	 print_mysql_error(NULL);
    	 return EXIT_FAILURE;
    }

    g_res = mysql_store_result(g_conn); //从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

	char *tbName = (char *) malloc(MAX_BUF_SIZE);
	while ((g_row = mysql_fetch_row(g_res))) {
		sprintf(tbName, "%s", g_row[0]);   //获取记录数

		//	char sql[MAX_BUF_SIZE];
		memset(sql, 0, sizeof(sql));
		sprintf(sql, "SELECT * FROM `%s`", tbName);

		if (mysql_query(g_conn, sql)){
			 print_mysql_error(NULL);
			 return EXIT_FAILURE;
		}

		MYSQL_RES *history; // mysql 记录集
		history = mysql_store_result(g_conn); //从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集

		int i;
		while ((g_row = mysql_fetch_row(history))) {
			for(i=0; i<mysql_num_fields(history); i++){
				printf("%s\t", g_row[i]);
			}
			printf("\n");
		}

		mysql_free_result(history);

	}

	return EXIT_SUCCESS;
}


