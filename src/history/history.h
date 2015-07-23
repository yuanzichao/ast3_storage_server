/*
 * history.h
 * 操作纪录日志
 *
 *  Created on: 2015年7月15日
 *      Author: yuan
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../sql/sql.h"
#include "../utils/read_conf.h"


//记录
int insert_history();					//记录操作记录信息


//查询
int get_cur_history();					//获取当天操作纪录信息
int get_spe_history(char* date);		//获取指定日期的操作纪录信息
int get_all_history();					//获取所有的操作纪录信息

