#define  _GNU_SOURCE 1
#include "db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
struct {
	MYSQL_STMT *stmt_directory_info_upd;
	MYSQL_BIND par_directory_info_upd [12];
	MYSQL_STMT *stmt_directory_info_ins;
	MYSQL_BIND par_directory_info_ins [12];
	MYSQL_STMT *stmt_directory_info_save;
	MYSQL_BIND par_directory_info_save [12];
	MYSQL_BIND res_directory_info_load [11];
	MYSQL_STMT *stmt_directory_info_get_by_id;
	MYSQL_BIND par_directory_info_get_by_id [1];
	MYSQL_BIND res_directory_info_get_by_id [11];
	MYSQL_STMT *stmt_directory_info_delete;
	MYSQL_BIND par_directory_info_delete [1];
	MYSQL_STMT *stmt_disk_info_upd;
	MYSQL_BIND par_disk_info_upd [10];
	MYSQL_STMT *stmt_disk_info_ins;
	MYSQL_BIND par_disk_info_ins [10];
	MYSQL_STMT *stmt_disk_info_save;
	MYSQL_BIND par_disk_info_save [10];
	MYSQL_BIND res_disk_info_load [9];
	MYSQL_STMT *stmt_disk_info_get_by_id;
	MYSQL_BIND par_disk_info_get_by_id [1];
	MYSQL_BIND res_disk_info_get_by_id [9];
	MYSQL_STMT *stmt_disk_info_delete;
	MYSQL_BIND par_disk_info_delete [1];
	MYSQL_STMT *stmt_file_info_upd;
	MYSQL_BIND par_file_info_upd [15];
	MYSQL_STMT *stmt_file_info_ins;
	MYSQL_BIND par_file_info_ins [15];
	MYSQL_STMT *stmt_file_info_save;
	MYSQL_BIND par_file_info_save [15];
	MYSQL_BIND res_file_info_load [14];
	MYSQL_STMT *stmt_file_info_get_by_id;
	MYSQL_BIND par_file_info_get_by_id [1];
	MYSQL_BIND res_file_info_get_by_id [14];
	MYSQL_STMT *stmt_file_info_delete;
	MYSQL_BIND par_file_info_delete [1];
	MYSQL_STMT *stmt_history_20150716_upd;
	MYSQL_BIND par_history_20150716_upd [4];
	MYSQL_STMT *stmt_history_20150716_ins;
	MYSQL_BIND par_history_20150716_ins [4];
	MYSQL_STMT *stmt_history_20150716_save;
	MYSQL_BIND par_history_20150716_save [4];
	MYSQL_BIND res_history_20150716_load [3];
	MYSQL_STMT *stmt_history_20150716_get_by_id;
	MYSQL_BIND par_history_20150716_get_by_id [1];
	MYSQL_BIND res_history_20150716_get_by_id [3];
	MYSQL_STMT *stmt_history_20150716_delete;
	MYSQL_BIND par_history_20150716_delete [1];
	MYSQL_STMT *stmt_history_20150720_upd;
	MYSQL_BIND par_history_20150720_upd [4];
	MYSQL_STMT *stmt_history_20150720_ins;
	MYSQL_BIND par_history_20150720_ins [4];
	MYSQL_STMT *stmt_history_20150720_save;
	MYSQL_BIND par_history_20150720_save [4];
	MYSQL_BIND res_history_20150720_load [3];
	MYSQL_STMT *stmt_history_20150720_get_by_id;
	MYSQL_BIND par_history_20150720_get_by_id [1];
	MYSQL_BIND res_history_20150720_get_by_id [3];
	MYSQL_STMT *stmt_history_20150720_delete;
	MYSQL_BIND par_history_20150720_delete [1];
	MYSQL_STMT *stmt_history_20150722_upd;
	MYSQL_BIND par_history_20150722_upd [4];
	MYSQL_STMT *stmt_history_20150722_ins;
	MYSQL_BIND par_history_20150722_ins [4];
	MYSQL_STMT *stmt_history_20150722_save;
	MYSQL_BIND par_history_20150722_save [4];
	MYSQL_BIND res_history_20150722_load [3];
	MYSQL_STMT *stmt_history_20150722_get_by_id;
	MYSQL_BIND par_history_20150722_get_by_id [1];
	MYSQL_BIND res_history_20150722_get_by_id [3];
	MYSQL_STMT *stmt_history_20150722_delete;
	MYSQL_BIND par_history_20150722_delete [1];
	MYSQL_STMT *stmt_history_20150723_upd;
	MYSQL_BIND par_history_20150723_upd [4];
	MYSQL_STMT *stmt_history_20150723_ins;
	MYSQL_BIND par_history_20150723_ins [4];
	MYSQL_STMT *stmt_history_20150723_save;
	MYSQL_BIND par_history_20150723_save [4];
	MYSQL_BIND res_history_20150723_load [3];
	MYSQL_STMT *stmt_history_20150723_get_by_id;
	MYSQL_BIND par_history_20150723_get_by_id [1];
	MYSQL_BIND res_history_20150723_get_by_id [3];
	MYSQL_STMT *stmt_history_20150723_delete;
	MYSQL_BIND par_history_20150723_delete [1];
} db_struct;
static int copyval_c2mysql_int (int *val, MYSQL_BIND* bnd)
{
	memcpy (bnd->buffer, val, sizeof (int));
	return (0);
}
static int copyval_c2mysql_long (long *val, MYSQL_BIND* bnd)
{
	memcpy (bnd->buffer, val, sizeof (long));
	return (0);
}
static int copyval_c2mysql_double (double *val, MYSQL_BIND* bnd)
{
	memcpy (bnd->buffer, val, sizeof (double));
	return (0);
}
static int copyval_c2mysql_char (char **val, MYSQL_BIND* bnd)
{
	strncpy (bnd->buffer, * val, bnd->buffer_length);if (bnd->length) *(unsigned int*)bnd->length = (val && *val)?strlen (*val):0;
	return (0);
}
static int copyval_c2mysql_time (time_t *val, MYSQL_BIND* bnd)
{
	struct tm *t; MYSQL_TIME *m;
	t = localtime ((const time_t*) val); m = (MYSQL_TIME*)bnd->buffer; memset (m, 0, sizeof (MYSQL_TIME));
	m->time_type = MYSQL_TIMESTAMP_DATETIME;
	m->year = t->tm_year + 1900; m->month = t->tm_mon + 1; m->day = t->tm_mday; m->hour = t->tm_hour; m->minute = t->tm_min; m->second = t->tm_sec;
	return (0);
}
static int copyval_mysql2c_char (MYSQL_BIND* bnd, char **c)
{
	*c = (strndup (bnd->buffer, bnd->buffer_length));
	return (0);
}
static int copyval_mysql2c_int (MYSQL_BIND* bnd, int *i)
{
	int *val;
	*i = * (int*) bnd->buffer;
	return (0);
}
static int copyval_mysql2c_long (MYSQL_BIND* bnd, long *i)
{
	long *val;
	*i = * (long*) bnd->buffer;
	return (0);
}
static int copyval_mysql2c_double (MYSQL_BIND* bnd, double *i)
{
	double *val;
	*i = * (double*) bnd->buffer;
	return (0);
}
static int copyval_mysql2c_time (MYSQL_BIND* bnd, time_t *i)
{
	MYSQL_TIME *m;struct tm t;
	m = (MYSQL_TIME*)bnd->buffer;
	memset (& t, 0, sizeof (struct tm));
	t.tm_year = m->year - 1900; t.tm_mon = m->month - 1; t.tm_mday = m->day;
	t.tm_hour = m->hour;
	t.tm_min = m->minute;
	t.tm_sec = m->second; *i = mktime (& t);
	return (0);
}
int db_init (MYSQL *mysql)
{
	memset (& db_struct, 0, sizeof (db_struct));
	char *sql;
	int ret;
	db_struct.res_directory_info_load [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_directory_info_load [0].buffer = malloc (sizeof(int));
	db_struct.res_directory_info_load [0].buffer_length = 0;
	db_struct.res_directory_info_load [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_directory_info_load [1].buffer = malloc (255 + 1);
	db_struct.res_directory_info_load [1].buffer_length = 255;
	db_struct.res_directory_info_load [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_directory_info_load [2].buffer = malloc (255 + 1);
	db_struct.res_directory_info_load [2].buffer_length = 255;
	db_struct.res_directory_info_load [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_directory_info_load [3].buffer = malloc (255 + 1);
	db_struct.res_directory_info_load [3].buffer_length = 255;
	db_struct.res_directory_info_load [4].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_directory_info_load [4].buffer = malloc (sizeof(int));
	db_struct.res_directory_info_load [4].buffer_length = 0;
	db_struct.res_directory_info_load [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.res_directory_info_load [5].buffer = malloc (sizeof(double));
	db_struct.res_directory_info_load [5].buffer_length = 0;
	db_struct.res_directory_info_load [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_directory_info_load [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_directory_info_load [6].buffer_length = 0;
	db_struct.res_directory_info_load [7].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_directory_info_load [7].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_directory_info_load [7].buffer_length = 0;
	db_struct.res_directory_info_load [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_directory_info_load [8].buffer = malloc (sizeof(int));
	db_struct.res_directory_info_load [8].buffer_length = 0;
	db_struct.res_directory_info_load [9].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_directory_info_load [9].buffer = malloc (sizeof(int));
	db_struct.res_directory_info_load [9].buffer_length = 0;
	db_struct.res_directory_info_load [10].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_directory_info_load [10].buffer = malloc (sizeof(int));
	db_struct.res_directory_info_load [10].buffer_length = 0;
	db_struct.par_directory_info_get_by_id [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_get_by_id [0].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_get_by_id [0].buffer_length = 0;
	db_struct.par_directory_info_delete [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_delete [0].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_delete [0].buffer_length = 0;
	db_struct.stmt_directory_info_get_by_id = mysql_stmt_init (mysql);
	sql = "SELECT directory_id, directory_name, disk_uuid, disk_name, parent_id, directory_size, time, recent_use_time, permission, accessed_time, file_number FROM directory_info WHERE directory_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_directory_info_get_by_id , sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_directory_info_get_by_id, db_struct.par_directory_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_result (db_struct.stmt_directory_info_get_by_id, db_struct.res_directory_info_load);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_get_by_id));
		return (-1);
	}
	db_struct.stmt_directory_info_delete = mysql_stmt_init (mysql);
	sql = "DELETE FROM directory_info  WHERE directory_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_directory_info_delete, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_delete));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_directory_info_delete, db_struct.par_directory_info_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_get_by_id));
		return (-1);
	}
	db_struct.par_directory_info_save [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_save [0].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_save [0].buffer_length = 0;
	db_struct.par_directory_info_save [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_save [1].buffer = malloc (255 + 1);
	db_struct.par_directory_info_save [1].buffer_length = 255;
	db_struct.par_directory_info_save [1].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_save [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_save [2].buffer = malloc (255 + 1);
	db_struct.par_directory_info_save [2].buffer_length = 255;
	db_struct.par_directory_info_save [2].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_save [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_save [3].buffer = malloc (255 + 1);
	db_struct.par_directory_info_save [3].buffer_length = 255;
	db_struct.par_directory_info_save [3].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_save [4].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_save [4].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_save [4].buffer_length = 0;
	db_struct.par_directory_info_save [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_directory_info_save [5].buffer = malloc (sizeof(double));
	db_struct.par_directory_info_save [5].buffer_length = 0;
	db_struct.par_directory_info_save [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_directory_info_save [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_directory_info_save [6].buffer_length = 0;
	db_struct.par_directory_info_save [7].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_directory_info_save [7].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_directory_info_save [7].buffer_length = 0;
	db_struct.par_directory_info_save [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_save [8].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_save [8].buffer_length = 0;
	db_struct.par_directory_info_save [9].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_save [9].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_save [9].buffer_length = 0;
	db_struct.par_directory_info_save [10].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_save [10].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_save [10].buffer_length = 0;
	db_struct.par_directory_info_save [11].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_save [11].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_save [11].buffer_length = 0;
	db_struct.stmt_directory_info_save = mysql_stmt_init (mysql);
	sql = "REPLACE directory_info SET directory_id = ? , directory_name = ? , disk_uuid = ? , disk_name = ? , parent_id = ? , directory_size = ? , time = ? , recent_use_time = ? , permission = ? , accessed_time = ? , file_number = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_directory_info_save, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_save));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_directory_info_save, db_struct.par_directory_info_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_save));
		return (-1);
	}
	db_struct.par_directory_info_upd [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_upd [0].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_upd [0].buffer_length = 0;
	db_struct.par_directory_info_upd [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_upd [1].buffer = malloc (255 + 1);
	db_struct.par_directory_info_upd [1].buffer_length = 255;
	db_struct.par_directory_info_upd [1].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_upd [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_upd [2].buffer = malloc (255 + 1);
	db_struct.par_directory_info_upd [2].buffer_length = 255;
	db_struct.par_directory_info_upd [2].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_upd [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_upd [3].buffer = malloc (255 + 1);
	db_struct.par_directory_info_upd [3].buffer_length = 255;
	db_struct.par_directory_info_upd [3].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_upd [4].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_upd [4].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_upd [4].buffer_length = 0;
	db_struct.par_directory_info_upd [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_directory_info_upd [5].buffer = malloc (sizeof(double));
	db_struct.par_directory_info_upd [5].buffer_length = 0;
	db_struct.par_directory_info_upd [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_directory_info_upd [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_directory_info_upd [6].buffer_length = 0;
	db_struct.par_directory_info_upd [7].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_directory_info_upd [7].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_directory_info_upd [7].buffer_length = 0;
	db_struct.par_directory_info_upd [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_upd [8].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_upd [8].buffer_length = 0;
	db_struct.par_directory_info_upd [9].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_upd [9].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_upd [9].buffer_length = 0;
	db_struct.par_directory_info_upd [10].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_upd [10].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_upd [10].buffer_length = 0;
	db_struct.par_directory_info_upd [11].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_upd [11].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_upd [11].buffer_length = 0;
	db_struct.stmt_directory_info_upd = mysql_stmt_init (mysql);
	sql = "UPDATE directory_info SET directory_id = ? , directory_name = ? , disk_uuid = ? , disk_name = ? , parent_id = ? , directory_size = ? , time = ? , recent_use_time = ? , permission = ? , accessed_time = ? , file_number = ? WHERE directory_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_directory_info_upd, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_upd));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_directory_info_upd, db_struct.par_directory_info_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_upd));
		return (-1);
	}
	db_struct.par_directory_info_ins [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_ins [0].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_ins [0].buffer_length = 0;
	db_struct.par_directory_info_ins [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_ins [1].buffer = malloc (255 + 1);
	db_struct.par_directory_info_ins [1].buffer_length = 255;
	db_struct.par_directory_info_ins [1].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_ins [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_ins [2].buffer = malloc (255 + 1);
	db_struct.par_directory_info_ins [2].buffer_length = 255;
	db_struct.par_directory_info_ins [2].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_ins [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_directory_info_ins [3].buffer = malloc (255 + 1);
	db_struct.par_directory_info_ins [3].buffer_length = 255;
	db_struct.par_directory_info_ins [3].length = malloc (sizeof (unsigned int));
	db_struct.par_directory_info_ins [4].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_ins [4].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_ins [4].buffer_length = 0;
	db_struct.par_directory_info_ins [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_directory_info_ins [5].buffer = malloc (sizeof(double));
	db_struct.par_directory_info_ins [5].buffer_length = 0;
	db_struct.par_directory_info_ins [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_directory_info_ins [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_directory_info_ins [6].buffer_length = 0;
	db_struct.par_directory_info_ins [7].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_directory_info_ins [7].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_directory_info_ins [7].buffer_length = 0;
	db_struct.par_directory_info_ins [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_ins [8].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_ins [8].buffer_length = 0;
	db_struct.par_directory_info_ins [9].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_ins [9].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_ins [9].buffer_length = 0;
	db_struct.par_directory_info_ins [10].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_ins [10].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_ins [10].buffer_length = 0;
	db_struct.par_directory_info_ins [11].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_directory_info_ins [11].buffer = malloc (sizeof(int));
	db_struct.par_directory_info_ins [11].buffer_length = 0;
	db_struct.stmt_directory_info_ins = mysql_stmt_init (mysql);
	sql = "INSERT INTO directory_info (directory_id, directory_name, disk_uuid, disk_name, parent_id, directory_size, time, recent_use_time, permission, accessed_time, file_number) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
	ret = mysql_stmt_prepare (db_struct.stmt_directory_info_ins, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_ins));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_directory_info_ins, db_struct.par_directory_info_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_ins));
		return (-1);
	}
	db_struct.res_disk_info_load [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_disk_info_load [0].buffer = malloc (sizeof(int));
	db_struct.res_disk_info_load [0].buffer_length = 0;
	db_struct.res_disk_info_load [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_disk_info_load [1].buffer = malloc (255 + 1);
	db_struct.res_disk_info_load [1].buffer_length = 255;
	db_struct.res_disk_info_load [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_disk_info_load [2].buffer = malloc (255 + 1);
	db_struct.res_disk_info_load [2].buffer_length = 255;
	db_struct.res_disk_info_load [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_disk_info_load [3].buffer = malloc (255 + 1);
	db_struct.res_disk_info_load [3].buffer_length = 255;
	db_struct.res_disk_info_load [4].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.res_disk_info_load [4].buffer = malloc (sizeof(double));
	db_struct.res_disk_info_load [4].buffer_length = 0;
	db_struct.res_disk_info_load [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.res_disk_info_load [5].buffer = malloc (sizeof(double));
	db_struct.res_disk_info_load [5].buffer_length = 0;
	db_struct.res_disk_info_load [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_disk_info_load [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_disk_info_load [6].buffer_length = 0;
	db_struct.res_disk_info_load [7].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_disk_info_load [7].buffer = malloc (sizeof(int));
	db_struct.res_disk_info_load [7].buffer_length = 0;
	db_struct.res_disk_info_load [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_disk_info_load [8].buffer = malloc (sizeof(int));
	db_struct.res_disk_info_load [8].buffer_length = 0;
	db_struct.par_disk_info_get_by_id [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_get_by_id [0].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_get_by_id [0].buffer_length = 0;
	db_struct.par_disk_info_delete [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_delete [0].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_delete [0].buffer_length = 0;
	db_struct.stmt_disk_info_get_by_id = mysql_stmt_init (mysql);
	sql = "SELECT disk_id, disk_name, disk_uuid, disk_type, disk_capacity, disk_used, recent_use_time, permission, disk_status FROM disk_info WHERE disk_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_disk_info_get_by_id , sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_disk_info_get_by_id, db_struct.par_disk_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_result (db_struct.stmt_disk_info_get_by_id, db_struct.res_disk_info_load);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_get_by_id));
		return (-1);
	}
	db_struct.stmt_disk_info_delete = mysql_stmt_init (mysql);
	sql = "DELETE FROM disk_info  WHERE disk_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_disk_info_delete, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_delete));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_disk_info_delete, db_struct.par_disk_info_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_get_by_id));
		return (-1);
	}
	db_struct.par_disk_info_save [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_save [0].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_save [0].buffer_length = 0;
	db_struct.par_disk_info_save [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_save [1].buffer = malloc (255 + 1);
	db_struct.par_disk_info_save [1].buffer_length = 255;
	db_struct.par_disk_info_save [1].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_save [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_save [2].buffer = malloc (255 + 1);
	db_struct.par_disk_info_save [2].buffer_length = 255;
	db_struct.par_disk_info_save [2].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_save [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_save [3].buffer = malloc (255 + 1);
	db_struct.par_disk_info_save [3].buffer_length = 255;
	db_struct.par_disk_info_save [3].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_save [4].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_disk_info_save [4].buffer = malloc (sizeof(double));
	db_struct.par_disk_info_save [4].buffer_length = 0;
	db_struct.par_disk_info_save [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_disk_info_save [5].buffer = malloc (sizeof(double));
	db_struct.par_disk_info_save [5].buffer_length = 0;
	db_struct.par_disk_info_save [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_disk_info_save [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_disk_info_save [6].buffer_length = 0;
	db_struct.par_disk_info_save [7].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_save [7].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_save [7].buffer_length = 0;
	db_struct.par_disk_info_save [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_save [8].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_save [8].buffer_length = 0;
	db_struct.par_disk_info_save [9].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_save [9].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_save [9].buffer_length = 0;
	db_struct.stmt_disk_info_save = mysql_stmt_init (mysql);
	sql = "REPLACE disk_info SET disk_id = ? , disk_name = ? , disk_uuid = ? , disk_type = ? , disk_capacity = ? , disk_used = ? , recent_use_time = ? , permission = ? , disk_status = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_disk_info_save, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_save));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_disk_info_save, db_struct.par_disk_info_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_save));
		return (-1);
	}
	db_struct.par_disk_info_upd [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_upd [0].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_upd [0].buffer_length = 0;
	db_struct.par_disk_info_upd [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_upd [1].buffer = malloc (255 + 1);
	db_struct.par_disk_info_upd [1].buffer_length = 255;
	db_struct.par_disk_info_upd [1].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_upd [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_upd [2].buffer = malloc (255 + 1);
	db_struct.par_disk_info_upd [2].buffer_length = 255;
	db_struct.par_disk_info_upd [2].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_upd [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_upd [3].buffer = malloc (255 + 1);
	db_struct.par_disk_info_upd [3].buffer_length = 255;
	db_struct.par_disk_info_upd [3].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_upd [4].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_disk_info_upd [4].buffer = malloc (sizeof(double));
	db_struct.par_disk_info_upd [4].buffer_length = 0;
	db_struct.par_disk_info_upd [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_disk_info_upd [5].buffer = malloc (sizeof(double));
	db_struct.par_disk_info_upd [5].buffer_length = 0;
	db_struct.par_disk_info_upd [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_disk_info_upd [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_disk_info_upd [6].buffer_length = 0;
	db_struct.par_disk_info_upd [7].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_upd [7].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_upd [7].buffer_length = 0;
	db_struct.par_disk_info_upd [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_upd [8].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_upd [8].buffer_length = 0;
	db_struct.par_disk_info_upd [9].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_upd [9].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_upd [9].buffer_length = 0;
	db_struct.stmt_disk_info_upd = mysql_stmt_init (mysql);
	sql = "UPDATE disk_info SET disk_id = ? , disk_name = ? , disk_uuid = ? , disk_type = ? , disk_capacity = ? , disk_used = ? , recent_use_time = ? , permission = ? , disk_status = ? WHERE disk_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_disk_info_upd, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_upd));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_disk_info_upd, db_struct.par_disk_info_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_upd));
		return (-1);
	}
	db_struct.par_disk_info_ins [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_ins [0].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_ins [0].buffer_length = 0;
	db_struct.par_disk_info_ins [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_ins [1].buffer = malloc (255 + 1);
	db_struct.par_disk_info_ins [1].buffer_length = 255;
	db_struct.par_disk_info_ins [1].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_ins [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_ins [2].buffer = malloc (255 + 1);
	db_struct.par_disk_info_ins [2].buffer_length = 255;
	db_struct.par_disk_info_ins [2].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_ins [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_disk_info_ins [3].buffer = malloc (255 + 1);
	db_struct.par_disk_info_ins [3].buffer_length = 255;
	db_struct.par_disk_info_ins [3].length = malloc (sizeof (unsigned int));
	db_struct.par_disk_info_ins [4].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_disk_info_ins [4].buffer = malloc (sizeof(double));
	db_struct.par_disk_info_ins [4].buffer_length = 0;
	db_struct.par_disk_info_ins [5].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_disk_info_ins [5].buffer = malloc (sizeof(double));
	db_struct.par_disk_info_ins [5].buffer_length = 0;
	db_struct.par_disk_info_ins [6].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_disk_info_ins [6].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_disk_info_ins [6].buffer_length = 0;
	db_struct.par_disk_info_ins [7].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_ins [7].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_ins [7].buffer_length = 0;
	db_struct.par_disk_info_ins [8].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_ins [8].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_ins [8].buffer_length = 0;
	db_struct.par_disk_info_ins [9].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_disk_info_ins [9].buffer = malloc (sizeof(int));
	db_struct.par_disk_info_ins [9].buffer_length = 0;
	db_struct.stmt_disk_info_ins = mysql_stmt_init (mysql);
	sql = "INSERT INTO disk_info (disk_id, disk_name, disk_uuid, disk_type, disk_capacity, disk_used, recent_use_time, permission, disk_status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
	ret = mysql_stmt_prepare (db_struct.stmt_disk_info_ins, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_ins));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_disk_info_ins, db_struct.par_disk_info_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_ins));
		return (-1);
	}
	db_struct.res_file_info_load [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_file_info_load [0].buffer = malloc (sizeof(int));
	db_struct.res_file_info_load [0].buffer_length = 0;
	db_struct.res_file_info_load [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_file_info_load [1].buffer = malloc (255 + 1);
	db_struct.res_file_info_load [1].buffer_length = 255;
	db_struct.res_file_info_load [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_file_info_load [2].buffer = malloc (255 + 1);
	db_struct.res_file_info_load [2].buffer_length = 255;
	db_struct.res_file_info_load [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_file_info_load [3].buffer = malloc (255 + 1);
	db_struct.res_file_info_load [3].buffer_length = 255;
	db_struct.res_file_info_load [4].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_file_info_load [4].buffer = malloc (255 + 1);
	db_struct.res_file_info_load [4].buffer_length = 255;
	db_struct.res_file_info_load [5].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_file_info_load [5].buffer = malloc (255 + 1);
	db_struct.res_file_info_load [5].buffer_length = 255;
	db_struct.res_file_info_load [6].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.res_file_info_load [6].buffer = malloc (sizeof(double));
	db_struct.res_file_info_load [6].buffer_length = 0;
	db_struct.res_file_info_load [7].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.res_file_info_load [7].buffer = malloc (sizeof(double));
	db_struct.res_file_info_load [7].buffer_length = 0;
	db_struct.res_file_info_load [8].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.res_file_info_load [8].buffer = malloc (sizeof(double));
	db_struct.res_file_info_load [8].buffer_length = 0;
	db_struct.res_file_info_load [9].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_file_info_load [9].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_file_info_load [9].buffer_length = 0;
	db_struct.res_file_info_load [10].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_file_info_load [10].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_file_info_load [10].buffer_length = 0;
	db_struct.res_file_info_load [11].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_file_info_load [11].buffer = malloc (sizeof(int));
	db_struct.res_file_info_load [11].buffer_length = 0;
	db_struct.res_file_info_load [12].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_file_info_load [12].buffer = malloc (sizeof(int));
	db_struct.res_file_info_load [12].buffer_length = 0;
	db_struct.res_file_info_load [13].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_file_info_load [13].buffer = malloc (sizeof(int));
	db_struct.res_file_info_load [13].buffer_length = 0;
	db_struct.par_file_info_get_by_id [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_get_by_id [0].buffer = malloc (sizeof(int));
	db_struct.par_file_info_get_by_id [0].buffer_length = 0;
	db_struct.par_file_info_delete [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_delete [0].buffer = malloc (sizeof(int));
	db_struct.par_file_info_delete [0].buffer_length = 0;
	db_struct.stmt_file_info_get_by_id = mysql_stmt_init (mysql);
	sql = "SELECT file_id, file_name, disk_uuid, directory_name, disk_name, md5, file_size, ra_val, dec_val, time, recent_use_time, permission, file_type, accessed_time FROM file_info WHERE file_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_file_info_get_by_id , sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_file_info_get_by_id, db_struct.par_file_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_result (db_struct.stmt_file_info_get_by_id, db_struct.res_file_info_load);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_get_by_id));
		return (-1);
	}
	db_struct.stmt_file_info_delete = mysql_stmt_init (mysql);
	sql = "DELETE FROM file_info  WHERE file_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_file_info_delete, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_delete));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_file_info_delete, db_struct.par_file_info_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_get_by_id));
		return (-1);
	}
	db_struct.par_file_info_save [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_save [0].buffer = malloc (sizeof(int));
	db_struct.par_file_info_save [0].buffer_length = 0;
	db_struct.par_file_info_save [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_save [1].buffer = malloc (255 + 1);
	db_struct.par_file_info_save [1].buffer_length = 255;
	db_struct.par_file_info_save [1].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_save [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_save [2].buffer = malloc (255 + 1);
	db_struct.par_file_info_save [2].buffer_length = 255;
	db_struct.par_file_info_save [2].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_save [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_save [3].buffer = malloc (255 + 1);
	db_struct.par_file_info_save [3].buffer_length = 255;
	db_struct.par_file_info_save [3].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_save [4].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_save [4].buffer = malloc (255 + 1);
	db_struct.par_file_info_save [4].buffer_length = 255;
	db_struct.par_file_info_save [4].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_save [5].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_save [5].buffer = malloc (255 + 1);
	db_struct.par_file_info_save [5].buffer_length = 255;
	db_struct.par_file_info_save [5].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_save [6].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_save [6].buffer = malloc (sizeof(double));
	db_struct.par_file_info_save [6].buffer_length = 0;
	db_struct.par_file_info_save [7].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_save [7].buffer = malloc (sizeof(double));
	db_struct.par_file_info_save [7].buffer_length = 0;
	db_struct.par_file_info_save [8].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_save [8].buffer = malloc (sizeof(double));
	db_struct.par_file_info_save [8].buffer_length = 0;
	db_struct.par_file_info_save [9].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_file_info_save [9].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_file_info_save [9].buffer_length = 0;
	db_struct.par_file_info_save [10].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_file_info_save [10].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_file_info_save [10].buffer_length = 0;
	db_struct.par_file_info_save [11].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_save [11].buffer = malloc (sizeof(int));
	db_struct.par_file_info_save [11].buffer_length = 0;
	db_struct.par_file_info_save [12].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_save [12].buffer = malloc (sizeof(int));
	db_struct.par_file_info_save [12].buffer_length = 0;
	db_struct.par_file_info_save [13].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_save [13].buffer = malloc (sizeof(int));
	db_struct.par_file_info_save [13].buffer_length = 0;
	db_struct.par_file_info_save [14].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_save [14].buffer = malloc (sizeof(int));
	db_struct.par_file_info_save [14].buffer_length = 0;
	db_struct.stmt_file_info_save = mysql_stmt_init (mysql);
	sql = "REPLACE file_info SET file_id = ? , file_name = ? , disk_uuid = ? , directory_name = ? , disk_name = ? , md5 = ? , file_size = ? , ra_val = ? , dec_val = ? , time = ? , recent_use_time = ? , permission = ? , file_type = ? , accessed_time = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_file_info_save, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_save));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_file_info_save, db_struct.par_file_info_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_save));
		return (-1);
	}
	db_struct.par_file_info_upd [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_upd [0].buffer = malloc (sizeof(int));
	db_struct.par_file_info_upd [0].buffer_length = 0;
	db_struct.par_file_info_upd [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_upd [1].buffer = malloc (255 + 1);
	db_struct.par_file_info_upd [1].buffer_length = 255;
	db_struct.par_file_info_upd [1].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_upd [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_upd [2].buffer = malloc (255 + 1);
	db_struct.par_file_info_upd [2].buffer_length = 255;
	db_struct.par_file_info_upd [2].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_upd [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_upd [3].buffer = malloc (255 + 1);
	db_struct.par_file_info_upd [3].buffer_length = 255;
	db_struct.par_file_info_upd [3].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_upd [4].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_upd [4].buffer = malloc (255 + 1);
	db_struct.par_file_info_upd [4].buffer_length = 255;
	db_struct.par_file_info_upd [4].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_upd [5].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_upd [5].buffer = malloc (255 + 1);
	db_struct.par_file_info_upd [5].buffer_length = 255;
	db_struct.par_file_info_upd [5].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_upd [6].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_upd [6].buffer = malloc (sizeof(double));
	db_struct.par_file_info_upd [6].buffer_length = 0;
	db_struct.par_file_info_upd [7].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_upd [7].buffer = malloc (sizeof(double));
	db_struct.par_file_info_upd [7].buffer_length = 0;
	db_struct.par_file_info_upd [8].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_upd [8].buffer = malloc (sizeof(double));
	db_struct.par_file_info_upd [8].buffer_length = 0;
	db_struct.par_file_info_upd [9].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_file_info_upd [9].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_file_info_upd [9].buffer_length = 0;
	db_struct.par_file_info_upd [10].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_file_info_upd [10].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_file_info_upd [10].buffer_length = 0;
	db_struct.par_file_info_upd [11].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_upd [11].buffer = malloc (sizeof(int));
	db_struct.par_file_info_upd [11].buffer_length = 0;
	db_struct.par_file_info_upd [12].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_upd [12].buffer = malloc (sizeof(int));
	db_struct.par_file_info_upd [12].buffer_length = 0;
	db_struct.par_file_info_upd [13].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_upd [13].buffer = malloc (sizeof(int));
	db_struct.par_file_info_upd [13].buffer_length = 0;
	db_struct.par_file_info_upd [14].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_upd [14].buffer = malloc (sizeof(int));
	db_struct.par_file_info_upd [14].buffer_length = 0;
	db_struct.stmt_file_info_upd = mysql_stmt_init (mysql);
	sql = "UPDATE file_info SET file_id = ? , file_name = ? , disk_uuid = ? , directory_name = ? , disk_name = ? , md5 = ? , file_size = ? , ra_val = ? , dec_val = ? , time = ? , recent_use_time = ? , permission = ? , file_type = ? , accessed_time = ? WHERE file_id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_file_info_upd, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_upd));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_file_info_upd, db_struct.par_file_info_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_upd));
		return (-1);
	}
	db_struct.par_file_info_ins [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_ins [0].buffer = malloc (sizeof(int));
	db_struct.par_file_info_ins [0].buffer_length = 0;
	db_struct.par_file_info_ins [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_ins [1].buffer = malloc (255 + 1);
	db_struct.par_file_info_ins [1].buffer_length = 255;
	db_struct.par_file_info_ins [1].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_ins [2].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_ins [2].buffer = malloc (255 + 1);
	db_struct.par_file_info_ins [2].buffer_length = 255;
	db_struct.par_file_info_ins [2].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_ins [3].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_ins [3].buffer = malloc (255 + 1);
	db_struct.par_file_info_ins [3].buffer_length = 255;
	db_struct.par_file_info_ins [3].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_ins [4].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_ins [4].buffer = malloc (255 + 1);
	db_struct.par_file_info_ins [4].buffer_length = 255;
	db_struct.par_file_info_ins [4].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_ins [5].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_file_info_ins [5].buffer = malloc (255 + 1);
	db_struct.par_file_info_ins [5].buffer_length = 255;
	db_struct.par_file_info_ins [5].length = malloc (sizeof (unsigned int));
	db_struct.par_file_info_ins [6].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_ins [6].buffer = malloc (sizeof(double));
	db_struct.par_file_info_ins [6].buffer_length = 0;
	db_struct.par_file_info_ins [7].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_ins [7].buffer = malloc (sizeof(double));
	db_struct.par_file_info_ins [7].buffer_length = 0;
	db_struct.par_file_info_ins [8].buffer_type = MYSQL_TYPE_DOUBLE;
	db_struct.par_file_info_ins [8].buffer = malloc (sizeof(double));
	db_struct.par_file_info_ins [8].buffer_length = 0;
	db_struct.par_file_info_ins [9].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_file_info_ins [9].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_file_info_ins [9].buffer_length = 0;
	db_struct.par_file_info_ins [10].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_file_info_ins [10].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_file_info_ins [10].buffer_length = 0;
	db_struct.par_file_info_ins [11].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_ins [11].buffer = malloc (sizeof(int));
	db_struct.par_file_info_ins [11].buffer_length = 0;
	db_struct.par_file_info_ins [12].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_ins [12].buffer = malloc (sizeof(int));
	db_struct.par_file_info_ins [12].buffer_length = 0;
	db_struct.par_file_info_ins [13].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_ins [13].buffer = malloc (sizeof(int));
	db_struct.par_file_info_ins [13].buffer_length = 0;
	db_struct.par_file_info_ins [14].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_file_info_ins [14].buffer = malloc (sizeof(int));
	db_struct.par_file_info_ins [14].buffer_length = 0;
	db_struct.stmt_file_info_ins = mysql_stmt_init (mysql);
	sql = "INSERT INTO file_info (file_id, file_name, disk_uuid, directory_name, disk_name, md5, file_size, ra_val, dec_val, time, recent_use_time, permission, file_type, accessed_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
	ret = mysql_stmt_prepare (db_struct.stmt_file_info_ins, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_ins));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_file_info_ins, db_struct.par_file_info_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_ins));
		return (-1);
	}
	db_struct.res_history_20150716_load [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_history_20150716_load [0].buffer = malloc (sizeof(int));
	db_struct.res_history_20150716_load [0].buffer_length = 0;
	db_struct.res_history_20150716_load [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_history_20150716_load [1].buffer = malloc (255 + 1);
	db_struct.res_history_20150716_load [1].buffer_length = 255;
	db_struct.res_history_20150716_load [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_history_20150716_load [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_history_20150716_load [2].buffer_length = 0;
	db_struct.par_history_20150716_get_by_id [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_get_by_id [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_get_by_id [0].buffer_length = 0;
	db_struct.par_history_20150716_delete [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_delete [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_delete [0].buffer_length = 0;
	db_struct.stmt_history_20150716_get_by_id = mysql_stmt_init (mysql);
	sql = "SELECT id, operation, time FROM history_20150716 WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150716_get_by_id , sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150716_get_by_id, db_struct.par_history_20150716_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_result (db_struct.stmt_history_20150716_get_by_id, db_struct.res_history_20150716_load);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_get_by_id));
		return (-1);
	}
	db_struct.stmt_history_20150716_delete = mysql_stmt_init (mysql);
	sql = "DELETE FROM history_20150716  WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150716_delete, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_delete));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150716_delete, db_struct.par_history_20150716_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_get_by_id));
		return (-1);
	}
	db_struct.par_history_20150716_save [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_save [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_save [0].buffer_length = 0;
	db_struct.par_history_20150716_save [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150716_save [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150716_save [1].buffer_length = 255;
	db_struct.par_history_20150716_save [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150716_save [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150716_save [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150716_save [2].buffer_length = 0;
	db_struct.par_history_20150716_save [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_save [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_save [3].buffer_length = 0;
	db_struct.stmt_history_20150716_save = mysql_stmt_init (mysql);
	sql = "REPLACE history_20150716 SET id = ? , operation = ? , time = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150716_save, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_save));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150716_save, db_struct.par_history_20150716_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_save));
		return (-1);
	}
	db_struct.par_history_20150716_upd [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_upd [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_upd [0].buffer_length = 0;
	db_struct.par_history_20150716_upd [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150716_upd [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150716_upd [1].buffer_length = 255;
	db_struct.par_history_20150716_upd [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150716_upd [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150716_upd [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150716_upd [2].buffer_length = 0;
	db_struct.par_history_20150716_upd [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_upd [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_upd [3].buffer_length = 0;
	db_struct.stmt_history_20150716_upd = mysql_stmt_init (mysql);
	sql = "UPDATE history_20150716 SET id = ? , operation = ? , time = ? WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150716_upd, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_upd));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150716_upd, db_struct.par_history_20150716_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_upd));
		return (-1);
	}
	db_struct.par_history_20150716_ins [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_ins [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_ins [0].buffer_length = 0;
	db_struct.par_history_20150716_ins [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150716_ins [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150716_ins [1].buffer_length = 255;
	db_struct.par_history_20150716_ins [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150716_ins [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150716_ins [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150716_ins [2].buffer_length = 0;
	db_struct.par_history_20150716_ins [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150716_ins [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150716_ins [3].buffer_length = 0;
	db_struct.stmt_history_20150716_ins = mysql_stmt_init (mysql);
	sql = "INSERT INTO history_20150716 (id, operation, time) VALUES (?, ?, ?)";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150716_ins, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_ins));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150716_ins, db_struct.par_history_20150716_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_ins));
		return (-1);
	}
	db_struct.res_history_20150720_load [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_history_20150720_load [0].buffer = malloc (sizeof(int));
	db_struct.res_history_20150720_load [0].buffer_length = 0;
	db_struct.res_history_20150720_load [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_history_20150720_load [1].buffer = malloc (255 + 1);
	db_struct.res_history_20150720_load [1].buffer_length = 255;
	db_struct.res_history_20150720_load [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_history_20150720_load [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_history_20150720_load [2].buffer_length = 0;
	db_struct.par_history_20150720_get_by_id [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_get_by_id [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_get_by_id [0].buffer_length = 0;
	db_struct.par_history_20150720_delete [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_delete [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_delete [0].buffer_length = 0;
	db_struct.stmt_history_20150720_get_by_id = mysql_stmt_init (mysql);
	sql = "SELECT id, operation, time FROM history_20150720 WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150720_get_by_id , sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150720_get_by_id, db_struct.par_history_20150720_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_result (db_struct.stmt_history_20150720_get_by_id, db_struct.res_history_20150720_load);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_get_by_id));
		return (-1);
	}
	db_struct.stmt_history_20150720_delete = mysql_stmt_init (mysql);
	sql = "DELETE FROM history_20150720  WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150720_delete, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_delete));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150720_delete, db_struct.par_history_20150720_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_get_by_id));
		return (-1);
	}
	db_struct.par_history_20150720_save [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_save [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_save [0].buffer_length = 0;
	db_struct.par_history_20150720_save [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150720_save [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150720_save [1].buffer_length = 255;
	db_struct.par_history_20150720_save [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150720_save [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150720_save [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150720_save [2].buffer_length = 0;
	db_struct.par_history_20150720_save [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_save [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_save [3].buffer_length = 0;
	db_struct.stmt_history_20150720_save = mysql_stmt_init (mysql);
	sql = "REPLACE history_20150720 SET id = ? , operation = ? , time = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150720_save, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_save));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150720_save, db_struct.par_history_20150720_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_save));
		return (-1);
	}
	db_struct.par_history_20150720_upd [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_upd [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_upd [0].buffer_length = 0;
	db_struct.par_history_20150720_upd [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150720_upd [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150720_upd [1].buffer_length = 255;
	db_struct.par_history_20150720_upd [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150720_upd [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150720_upd [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150720_upd [2].buffer_length = 0;
	db_struct.par_history_20150720_upd [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_upd [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_upd [3].buffer_length = 0;
	db_struct.stmt_history_20150720_upd = mysql_stmt_init (mysql);
	sql = "UPDATE history_20150720 SET id = ? , operation = ? , time = ? WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150720_upd, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_upd));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150720_upd, db_struct.par_history_20150720_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_upd));
		return (-1);
	}
	db_struct.par_history_20150720_ins [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_ins [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_ins [0].buffer_length = 0;
	db_struct.par_history_20150720_ins [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150720_ins [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150720_ins [1].buffer_length = 255;
	db_struct.par_history_20150720_ins [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150720_ins [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150720_ins [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150720_ins [2].buffer_length = 0;
	db_struct.par_history_20150720_ins [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150720_ins [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150720_ins [3].buffer_length = 0;
	db_struct.stmt_history_20150720_ins = mysql_stmt_init (mysql);
	sql = "INSERT INTO history_20150720 (id, operation, time) VALUES (?, ?, ?)";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150720_ins, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_ins));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150720_ins, db_struct.par_history_20150720_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_ins));
		return (-1);
	}
	db_struct.res_history_20150722_load [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_history_20150722_load [0].buffer = malloc (sizeof(int));
	db_struct.res_history_20150722_load [0].buffer_length = 0;
	db_struct.res_history_20150722_load [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_history_20150722_load [1].buffer = malloc (255 + 1);
	db_struct.res_history_20150722_load [1].buffer_length = 255;
	db_struct.res_history_20150722_load [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_history_20150722_load [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_history_20150722_load [2].buffer_length = 0;
	db_struct.par_history_20150722_get_by_id [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_get_by_id [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_get_by_id [0].buffer_length = 0;
	db_struct.par_history_20150722_delete [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_delete [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_delete [0].buffer_length = 0;
	db_struct.stmt_history_20150722_get_by_id = mysql_stmt_init (mysql);
	sql = "SELECT id, operation, time FROM history_20150722 WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150722_get_by_id , sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150722_get_by_id, db_struct.par_history_20150722_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_result (db_struct.stmt_history_20150722_get_by_id, db_struct.res_history_20150722_load);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_get_by_id));
		return (-1);
	}
	db_struct.stmt_history_20150722_delete = mysql_stmt_init (mysql);
	sql = "DELETE FROM history_20150722  WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150722_delete, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_delete));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150722_delete, db_struct.par_history_20150722_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_get_by_id));
		return (-1);
	}
	db_struct.par_history_20150722_save [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_save [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_save [0].buffer_length = 0;
	db_struct.par_history_20150722_save [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150722_save [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150722_save [1].buffer_length = 255;
	db_struct.par_history_20150722_save [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150722_save [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150722_save [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150722_save [2].buffer_length = 0;
	db_struct.par_history_20150722_save [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_save [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_save [3].buffer_length = 0;
	db_struct.stmt_history_20150722_save = mysql_stmt_init (mysql);
	sql = "REPLACE history_20150722 SET id = ? , operation = ? , time = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150722_save, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_save));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150722_save, db_struct.par_history_20150722_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_save));
		return (-1);
	}
	db_struct.par_history_20150722_upd [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_upd [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_upd [0].buffer_length = 0;
	db_struct.par_history_20150722_upd [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150722_upd [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150722_upd [1].buffer_length = 255;
	db_struct.par_history_20150722_upd [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150722_upd [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150722_upd [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150722_upd [2].buffer_length = 0;
	db_struct.par_history_20150722_upd [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_upd [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_upd [3].buffer_length = 0;
	db_struct.stmt_history_20150722_upd = mysql_stmt_init (mysql);
	sql = "UPDATE history_20150722 SET id = ? , operation = ? , time = ? WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150722_upd, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_upd));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150722_upd, db_struct.par_history_20150722_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_upd));
		return (-1);
	}
	db_struct.par_history_20150722_ins [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_ins [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_ins [0].buffer_length = 0;
	db_struct.par_history_20150722_ins [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150722_ins [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150722_ins [1].buffer_length = 255;
	db_struct.par_history_20150722_ins [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150722_ins [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150722_ins [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150722_ins [2].buffer_length = 0;
	db_struct.par_history_20150722_ins [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150722_ins [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150722_ins [3].buffer_length = 0;
	db_struct.stmt_history_20150722_ins = mysql_stmt_init (mysql);
	sql = "INSERT INTO history_20150722 (id, operation, time) VALUES (?, ?, ?)";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150722_ins, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_ins));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150722_ins, db_struct.par_history_20150722_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_ins));
		return (-1);
	}
	db_struct.res_history_20150723_load [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.res_history_20150723_load [0].buffer = malloc (sizeof(int));
	db_struct.res_history_20150723_load [0].buffer_length = 0;
	db_struct.res_history_20150723_load [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.res_history_20150723_load [1].buffer = malloc (255 + 1);
	db_struct.res_history_20150723_load [1].buffer_length = 255;
	db_struct.res_history_20150723_load [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.res_history_20150723_load [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.res_history_20150723_load [2].buffer_length = 0;
	db_struct.par_history_20150723_get_by_id [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_get_by_id [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_get_by_id [0].buffer_length = 0;
	db_struct.par_history_20150723_delete [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_delete [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_delete [0].buffer_length = 0;
	db_struct.stmt_history_20150723_get_by_id = mysql_stmt_init (mysql);
	sql = "SELECT id, operation, time FROM history_20150723 WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150723_get_by_id , sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150723_get_by_id, db_struct.par_history_20150723_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_get_by_id));
		return (-1);
	}
	ret = mysql_stmt_bind_result (db_struct.stmt_history_20150723_get_by_id, db_struct.res_history_20150723_load);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_get_by_id));
		return (-1);
	}
	db_struct.stmt_history_20150723_delete = mysql_stmt_init (mysql);
	sql = "DELETE FROM history_20150723  WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150723_delete, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_delete));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150723_delete, db_struct.par_history_20150723_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_get_by_id));
		return (-1);
	}
	db_struct.par_history_20150723_save [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_save [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_save [0].buffer_length = 0;
	db_struct.par_history_20150723_save [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150723_save [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150723_save [1].buffer_length = 255;
	db_struct.par_history_20150723_save [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150723_save [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150723_save [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150723_save [2].buffer_length = 0;
	db_struct.par_history_20150723_save [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_save [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_save [3].buffer_length = 0;
	db_struct.stmt_history_20150723_save = mysql_stmt_init (mysql);
	sql = "REPLACE history_20150723 SET id = ? , operation = ? , time = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150723_save, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_save));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150723_save, db_struct.par_history_20150723_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_save));
		return (-1);
	}
	db_struct.par_history_20150723_upd [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_upd [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_upd [0].buffer_length = 0;
	db_struct.par_history_20150723_upd [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150723_upd [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150723_upd [1].buffer_length = 255;
	db_struct.par_history_20150723_upd [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150723_upd [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150723_upd [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150723_upd [2].buffer_length = 0;
	db_struct.par_history_20150723_upd [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_upd [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_upd [3].buffer_length = 0;
	db_struct.stmt_history_20150723_upd = mysql_stmt_init (mysql);
	sql = "UPDATE history_20150723 SET id = ? , operation = ? , time = ? WHERE id = ? ";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150723_upd, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_upd));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150723_upd, db_struct.par_history_20150723_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_upd));
		return (-1);
	}
	db_struct.par_history_20150723_ins [0].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_ins [0].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_ins [0].buffer_length = 0;
	db_struct.par_history_20150723_ins [1].buffer_type = MYSQL_TYPE_STRING;
	db_struct.par_history_20150723_ins [1].buffer = malloc (255 + 1);
	db_struct.par_history_20150723_ins [1].buffer_length = 255;
	db_struct.par_history_20150723_ins [1].length = malloc (sizeof (unsigned int));
	db_struct.par_history_20150723_ins [2].buffer_type = MYSQL_TYPE_DATETIME;
	db_struct.par_history_20150723_ins [2].buffer = malloc (sizeof (MYSQL_TIME));
	db_struct.par_history_20150723_ins [2].buffer_length = 0;
	db_struct.par_history_20150723_ins [3].buffer_type = MYSQL_TYPE_LONG;
	db_struct.par_history_20150723_ins [3].buffer = malloc (sizeof(int));
	db_struct.par_history_20150723_ins [3].buffer_length = 0;
	db_struct.stmt_history_20150723_ins = mysql_stmt_init (mysql);
	sql = "INSERT INTO history_20150723 (id, operation, time) VALUES (?, ?, ?)";
	ret = mysql_stmt_prepare (db_struct.stmt_history_20150723_ins, sql, strlen (sql));
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_ins));
		return (-1);
	}
	ret = mysql_stmt_bind_param (db_struct.stmt_history_20150723_ins, db_struct.par_history_20150723_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_ins));
		return (-1);
	}
	return (0);
}

db_directory_info *db_directory_info__new ()
{
	db_directory_info *r;
	r = malloc (sizeof (db_directory_info));
	memset (r, 0, sizeof (db_directory_info));
	return (r);
}

void db_directory_info__free (db_directory_info *rec)
{
	if (rec->directory_name != NULL) free (rec->directory_name);
	if (rec->disk_uuid != NULL) free (rec->disk_uuid);
	if (rec->disk_name != NULL) free (rec->disk_name);
	free (rec);
}

int db_directory_info__save (db_directory_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_save [0]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_directory_info_save [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_directory_info_save [2]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_directory_info_save [3]);
	copyval_c2mysql_int (& record->parent_id, & db_struct.par_directory_info_save [4]);
	copyval_c2mysql_double (& record->directory_size, & db_struct.par_directory_info_save [5]);
	copyval_c2mysql_time (& record->time, & db_struct.par_directory_info_save [6]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_directory_info_save [7]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_directory_info_save [8]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_directory_info_save [9]);
	copyval_c2mysql_int (& record->file_number, & db_struct.par_directory_info_save [10]);
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_save [11]);
	ret = mysql_stmt_execute (db_struct.stmt_directory_info_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_save));
		return (-1);
	}
	if (record->directory_id == 0) {
		record->directory_id = mysql_stmt_insert_id (db_struct.stmt_directory_info_save);
	}
	return (0);
}
int db_directory_info__delete (db_directory_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_delete [0]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_directory_info_delete [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_directory_info_delete [2]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_directory_info_delete [3]);
	copyval_c2mysql_int (& record->parent_id, & db_struct.par_directory_info_delete [4]);
	copyval_c2mysql_double (& record->directory_size, & db_struct.par_directory_info_delete [5]);
	copyval_c2mysql_time (& record->time, & db_struct.par_directory_info_delete [6]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_directory_info_delete [7]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_directory_info_delete [8]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_directory_info_delete [9]);
	copyval_c2mysql_int (& record->file_number, & db_struct.par_directory_info_delete [10]);
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_delete [11]);
	ret = mysql_stmt_execute (db_struct.stmt_directory_info_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_delete));
		return (-1);
	}
	return (0);
}
int db_directory_info__update (db_directory_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_upd [0]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_directory_info_upd [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_directory_info_upd [2]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_directory_info_upd [3]);
	copyval_c2mysql_int (& record->parent_id, & db_struct.par_directory_info_upd [4]);
	copyval_c2mysql_double (& record->directory_size, & db_struct.par_directory_info_upd [5]);
	copyval_c2mysql_time (& record->time, & db_struct.par_directory_info_upd [6]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_directory_info_upd [7]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_directory_info_upd [8]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_directory_info_upd [9]);
	copyval_c2mysql_int (& record->file_number, & db_struct.par_directory_info_upd [10]);
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_upd [11]);
	ret = mysql_stmt_execute (db_struct.stmt_directory_info_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_upd));
		return (-1);
	}
	return (0);
}
int db_directory_info__insert (db_directory_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_ins [0]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_directory_info_ins [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_directory_info_ins [2]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_directory_info_ins [3]);
	copyval_c2mysql_int (& record->parent_id, & db_struct.par_directory_info_ins [4]);
	copyval_c2mysql_double (& record->directory_size, & db_struct.par_directory_info_ins [5]);
	copyval_c2mysql_time (& record->time, & db_struct.par_directory_info_ins [6]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_directory_info_ins [7]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_directory_info_ins [8]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_directory_info_ins [9]);
	copyval_c2mysql_int (& record->file_number, & db_struct.par_directory_info_ins [10]);
	copyval_c2mysql_int (& record->directory_id, & db_struct.par_directory_info_ins [11]);
	ret = mysql_stmt_execute (db_struct.stmt_directory_info_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_ins));
		return (-1);
	}
	if (record->directory_id == 0) {
		record->directory_id = mysql_stmt_insert_id (db_struct.stmt_directory_info_ins);
	}
	return (0);
}
db_directory_info *db_directory_info__get_by_id (int directory_id)
{
	int ret;
	db_directory_info *rec;
	copyval_c2mysql_int (& directory_id, & db_struct.par_directory_info_get_by_id [0]);
	ret = mysql_stmt_execute (db_struct.stmt_directory_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_get_by_id));
		return (NULL);
	}
	ret = mysql_stmt_fetch (db_struct.stmt_directory_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_get_by_id));
		return (NULL);
	}
	rec = db_directory_info__new ();
	copyval_mysql2c_int (& db_struct.res_directory_info_load [0], & rec->directory_id);
	copyval_mysql2c_char (& db_struct.res_directory_info_load [1], & rec->directory_name);
	copyval_mysql2c_char (& db_struct.res_directory_info_load [2], & rec->disk_uuid);
	copyval_mysql2c_char (& db_struct.res_directory_info_load [3], & rec->disk_name);
	copyval_mysql2c_int (& db_struct.res_directory_info_load [4], & rec->parent_id);
	copyval_mysql2c_double (& db_struct.res_directory_info_load [5], & rec->directory_size);
	copyval_mysql2c_time (& db_struct.res_directory_info_load [6], & rec->time);
	copyval_mysql2c_time (& db_struct.res_directory_info_load [7], & rec->recent_use_time);
	copyval_mysql2c_int (& db_struct.res_directory_info_load [8], & rec->permission);
	copyval_mysql2c_int (& db_struct.res_directory_info_load [9], & rec->accessed_time);
	copyval_mysql2c_int (& db_struct.res_directory_info_load [10], & rec->file_number);
	ret = mysql_stmt_reset (db_struct.stmt_directory_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_directory_info_get_by_id));
		return (NULL);
	}
	return (rec);
}
db_disk_info *db_disk_info__new ()
{
	db_disk_info *r;
	r = malloc (sizeof (db_disk_info));
	memset (r, 0, sizeof (db_disk_info));
	return (r);
}

void db_disk_info__free (db_disk_info *rec)
{
	if (rec->disk_name != NULL) free (rec->disk_name);
	if (rec->disk_uuid != NULL) free (rec->disk_uuid);
	if (rec->disk_type != NULL) free (rec->disk_type);
	free (rec);
}

int db_disk_info__save (db_disk_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_save [0]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_disk_info_save [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_disk_info_save [2]);
	copyval_c2mysql_char (& record->disk_type, & db_struct.par_disk_info_save [3]);
	copyval_c2mysql_double (& record->disk_capacity, & db_struct.par_disk_info_save [4]);
	copyval_c2mysql_double (& record->disk_used, & db_struct.par_disk_info_save [5]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_disk_info_save [6]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_disk_info_save [7]);
	copyval_c2mysql_int (& record->disk_status, & db_struct.par_disk_info_save [8]);
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_save [9]);
	ret = mysql_stmt_execute (db_struct.stmt_disk_info_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_save));
		return (-1);
	}
	if (record->disk_id == 0) {
		record->disk_id = mysql_stmt_insert_id (db_struct.stmt_disk_info_save);
	}
	return (0);
}
int db_disk_info__delete (db_disk_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_delete [0]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_disk_info_delete [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_disk_info_delete [2]);
	copyval_c2mysql_char (& record->disk_type, & db_struct.par_disk_info_delete [3]);
	copyval_c2mysql_double (& record->disk_capacity, & db_struct.par_disk_info_delete [4]);
	copyval_c2mysql_double (& record->disk_used, & db_struct.par_disk_info_delete [5]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_disk_info_delete [6]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_disk_info_delete [7]);
	copyval_c2mysql_int (& record->disk_status, & db_struct.par_disk_info_delete [8]);
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_delete [9]);
	ret = mysql_stmt_execute (db_struct.stmt_disk_info_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_delete));
		return (-1);
	}
	return (0);
}
int db_disk_info__update (db_disk_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_upd [0]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_disk_info_upd [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_disk_info_upd [2]);
	copyval_c2mysql_char (& record->disk_type, & db_struct.par_disk_info_upd [3]);
	copyval_c2mysql_double (& record->disk_capacity, & db_struct.par_disk_info_upd [4]);
	copyval_c2mysql_double (& record->disk_used, & db_struct.par_disk_info_upd [5]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_disk_info_upd [6]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_disk_info_upd [7]);
	copyval_c2mysql_int (& record->disk_status, & db_struct.par_disk_info_upd [8]);
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_upd [9]);
	ret = mysql_stmt_execute (db_struct.stmt_disk_info_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_upd));
		return (-1);
	}
	return (0);
}
int db_disk_info__insert (db_disk_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_ins [0]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_disk_info_ins [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_disk_info_ins [2]);
	copyval_c2mysql_char (& record->disk_type, & db_struct.par_disk_info_ins [3]);
	copyval_c2mysql_double (& record->disk_capacity, & db_struct.par_disk_info_ins [4]);
	copyval_c2mysql_double (& record->disk_used, & db_struct.par_disk_info_ins [5]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_disk_info_ins [6]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_disk_info_ins [7]);
	copyval_c2mysql_int (& record->disk_status, & db_struct.par_disk_info_ins [8]);
	copyval_c2mysql_int (& record->disk_id, & db_struct.par_disk_info_ins [9]);
	ret = mysql_stmt_execute (db_struct.stmt_disk_info_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_ins));
		return (-1);
	}
	if (record->disk_id == 0) {
		record->disk_id = mysql_stmt_insert_id (db_struct.stmt_disk_info_ins);
	}
	return (0);
}
db_disk_info *db_disk_info__get_by_id (int disk_id)
{
	int ret;
	db_disk_info *rec;
	copyval_c2mysql_int (& disk_id, & db_struct.par_disk_info_get_by_id [0]);
	ret = mysql_stmt_execute (db_struct.stmt_disk_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_get_by_id));
		return (NULL);
	}
	ret = mysql_stmt_fetch (db_struct.stmt_disk_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_get_by_id));
		return (NULL);
	}
	rec = db_disk_info__new ();
	copyval_mysql2c_int (& db_struct.res_disk_info_load [0], & rec->disk_id);
	copyval_mysql2c_char (& db_struct.res_disk_info_load [1], & rec->disk_name);
	copyval_mysql2c_char (& db_struct.res_disk_info_load [2], & rec->disk_uuid);
	copyval_mysql2c_char (& db_struct.res_disk_info_load [3], & rec->disk_type);
	copyval_mysql2c_double (& db_struct.res_disk_info_load [4], & rec->disk_capacity);
	copyval_mysql2c_double (& db_struct.res_disk_info_load [5], & rec->disk_used);
	copyval_mysql2c_time (& db_struct.res_disk_info_load [6], & rec->recent_use_time);
	copyval_mysql2c_int (& db_struct.res_disk_info_load [7], & rec->permission);
	copyval_mysql2c_int (& db_struct.res_disk_info_load [8], & rec->disk_status);
	ret = mysql_stmt_reset (db_struct.stmt_disk_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_disk_info_get_by_id));
		return (NULL);
	}
	return (rec);
}
db_file_info *db_file_info__new ()
{
	db_file_info *r;
	r = malloc (sizeof (db_file_info));
	memset (r, 0, sizeof (db_file_info));
	return (r);
}

void db_file_info__free (db_file_info *rec)
{
	if (rec->file_name != NULL) free (rec->file_name);
	if (rec->disk_uuid != NULL) free (rec->disk_uuid);
	if (rec->directory_name != NULL) free (rec->directory_name);
	if (rec->disk_name != NULL) free (rec->disk_name);
	if (rec->md5 != NULL) free (rec->md5);
	free (rec);
}

int db_file_info__save (db_file_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_save [0]);
	copyval_c2mysql_char (& record->file_name, & db_struct.par_file_info_save [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_file_info_save [2]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_file_info_save [3]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_file_info_save [4]);
	copyval_c2mysql_char (& record->md5, & db_struct.par_file_info_save [5]);
	copyval_c2mysql_double (& record->file_size, & db_struct.par_file_info_save [6]);
	copyval_c2mysql_double (& record->ra_val, & db_struct.par_file_info_save [7]);
	copyval_c2mysql_double (& record->dec_val, & db_struct.par_file_info_save [8]);
	copyval_c2mysql_time (& record->time, & db_struct.par_file_info_save [9]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_file_info_save [10]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_file_info_save [11]);
	copyval_c2mysql_int (& record->file_type, & db_struct.par_file_info_save [12]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_file_info_save [13]);
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_save [14]);
	ret = mysql_stmt_execute (db_struct.stmt_file_info_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_save));
		return (-1);
	}
	if (record->file_id == 0) {
		record->file_id = mysql_stmt_insert_id (db_struct.stmt_file_info_save);
	}
	return (0);
}
int db_file_info__delete (db_file_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_delete [0]);
	copyval_c2mysql_char (& record->file_name, & db_struct.par_file_info_delete [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_file_info_delete [2]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_file_info_delete [3]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_file_info_delete [4]);
	copyval_c2mysql_char (& record->md5, & db_struct.par_file_info_delete [5]);
	copyval_c2mysql_double (& record->file_size, & db_struct.par_file_info_delete [6]);
	copyval_c2mysql_double (& record->ra_val, & db_struct.par_file_info_delete [7]);
	copyval_c2mysql_double (& record->dec_val, & db_struct.par_file_info_delete [8]);
	copyval_c2mysql_time (& record->time, & db_struct.par_file_info_delete [9]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_file_info_delete [10]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_file_info_delete [11]);
	copyval_c2mysql_int (& record->file_type, & db_struct.par_file_info_delete [12]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_file_info_delete [13]);
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_delete [14]);
	ret = mysql_stmt_execute (db_struct.stmt_file_info_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_delete));
		return (-1);
	}
	return (0);
}
int db_file_info__update (db_file_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_upd [0]);
	copyval_c2mysql_char (& record->file_name, & db_struct.par_file_info_upd [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_file_info_upd [2]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_file_info_upd [3]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_file_info_upd [4]);
	copyval_c2mysql_char (& record->md5, & db_struct.par_file_info_upd [5]);
	copyval_c2mysql_double (& record->file_size, & db_struct.par_file_info_upd [6]);
	copyval_c2mysql_double (& record->ra_val, & db_struct.par_file_info_upd [7]);
	copyval_c2mysql_double (& record->dec_val, & db_struct.par_file_info_upd [8]);
	copyval_c2mysql_time (& record->time, & db_struct.par_file_info_upd [9]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_file_info_upd [10]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_file_info_upd [11]);
	copyval_c2mysql_int (& record->file_type, & db_struct.par_file_info_upd [12]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_file_info_upd [13]);
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_upd [14]);
	ret = mysql_stmt_execute (db_struct.stmt_file_info_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_upd));
		return (-1);
	}
	return (0);
}
int db_file_info__insert (db_file_info *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_ins [0]);
	copyval_c2mysql_char (& record->file_name, & db_struct.par_file_info_ins [1]);
	copyval_c2mysql_char (& record->disk_uuid, & db_struct.par_file_info_ins [2]);
	copyval_c2mysql_char (& record->directory_name, & db_struct.par_file_info_ins [3]);
	copyval_c2mysql_char (& record->disk_name, & db_struct.par_file_info_ins [4]);
	copyval_c2mysql_char (& record->md5, & db_struct.par_file_info_ins [5]);
	copyval_c2mysql_double (& record->file_size, & db_struct.par_file_info_ins [6]);
	copyval_c2mysql_double (& record->ra_val, & db_struct.par_file_info_ins [7]);
	copyval_c2mysql_double (& record->dec_val, & db_struct.par_file_info_ins [8]);
	copyval_c2mysql_time (& record->time, & db_struct.par_file_info_ins [9]);
	copyval_c2mysql_time (& record->recent_use_time, & db_struct.par_file_info_ins [10]);
	copyval_c2mysql_int (& record->permission, & db_struct.par_file_info_ins [11]);
	copyval_c2mysql_int (& record->file_type, & db_struct.par_file_info_ins [12]);
	copyval_c2mysql_int (& record->accessed_time, & db_struct.par_file_info_ins [13]);
	copyval_c2mysql_int (& record->file_id, & db_struct.par_file_info_ins [14]);
	ret = mysql_stmt_execute (db_struct.stmt_file_info_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_ins));
		return (-1);
	}
	if (record->file_id == 0) {
		record->file_id = mysql_stmt_insert_id (db_struct.stmt_file_info_ins);
	}
	return (0);
}
db_file_info *db_file_info__get_by_id (int file_id)
{
	int ret;
	db_file_info *rec;
	copyval_c2mysql_int (& file_id, & db_struct.par_file_info_get_by_id [0]);
	ret = mysql_stmt_execute (db_struct.stmt_file_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_get_by_id));
		return (NULL);
	}
	ret = mysql_stmt_fetch (db_struct.stmt_file_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_get_by_id));
		return (NULL);
	}
	rec = db_file_info__new ();
	copyval_mysql2c_int (& db_struct.res_file_info_load [0], & rec->file_id);
	copyval_mysql2c_char (& db_struct.res_file_info_load [1], & rec->file_name);
	copyval_mysql2c_char (& db_struct.res_file_info_load [2], & rec->disk_uuid);
	copyval_mysql2c_char (& db_struct.res_file_info_load [3], & rec->directory_name);
	copyval_mysql2c_char (& db_struct.res_file_info_load [4], & rec->disk_name);
	copyval_mysql2c_char (& db_struct.res_file_info_load [5], & rec->md5);
	copyval_mysql2c_double (& db_struct.res_file_info_load [6], & rec->file_size);
	copyval_mysql2c_double (& db_struct.res_file_info_load [7], & rec->ra_val);
	copyval_mysql2c_double (& db_struct.res_file_info_load [8], & rec->dec_val);
	copyval_mysql2c_time (& db_struct.res_file_info_load [9], & rec->time);
	copyval_mysql2c_time (& db_struct.res_file_info_load [10], & rec->recent_use_time);
	copyval_mysql2c_int (& db_struct.res_file_info_load [11], & rec->permission);
	copyval_mysql2c_int (& db_struct.res_file_info_load [12], & rec->file_type);
	copyval_mysql2c_int (& db_struct.res_file_info_load [13], & rec->accessed_time);
	ret = mysql_stmt_reset (db_struct.stmt_file_info_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_file_info_get_by_id));
		return (NULL);
	}
	return (rec);
}
db_history_20150716 *db_history_20150716__new ()
{
	db_history_20150716 *r;
	r = malloc (sizeof (db_history_20150716));
	memset (r, 0, sizeof (db_history_20150716));
	return (r);
}

void db_history_20150716__free (db_history_20150716 *rec)
{
	if (rec->operation != NULL) free (rec->operation);
	free (rec);
}

int db_history_20150716__save (db_history_20150716 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_save [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150716_save [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150716_save [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_save [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150716_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_save));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150716_save);
	}
	return (0);
}
int db_history_20150716__delete (db_history_20150716 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_delete [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150716_delete [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150716_delete [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_delete [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150716_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_delete));
		return (-1);
	}
	return (0);
}
int db_history_20150716__update (db_history_20150716 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_upd [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150716_upd [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150716_upd [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_upd [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150716_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_upd));
		return (-1);
	}
	return (0);
}
int db_history_20150716__insert (db_history_20150716 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_ins [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150716_ins [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150716_ins [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150716_ins [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150716_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_ins));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150716_ins);
	}
	return (0);
}
db_history_20150716 *db_history_20150716__get_by_id (int id)
{
	int ret;
	db_history_20150716 *rec;
	copyval_c2mysql_int (& id, & db_struct.par_history_20150716_get_by_id [0]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150716_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_get_by_id));
		return (NULL);
	}
	ret = mysql_stmt_fetch (db_struct.stmt_history_20150716_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_get_by_id));
		return (NULL);
	}
	rec = db_history_20150716__new ();
	copyval_mysql2c_int (& db_struct.res_history_20150716_load [0], & rec->id);
	copyval_mysql2c_char (& db_struct.res_history_20150716_load [1], & rec->operation);
	copyval_mysql2c_time (& db_struct.res_history_20150716_load [2], & rec->time);
	ret = mysql_stmt_reset (db_struct.stmt_history_20150716_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150716_get_by_id));
		return (NULL);
	}
	return (rec);
}
db_history_20150720 *db_history_20150720__new ()
{
	db_history_20150720 *r;
	r = malloc (sizeof (db_history_20150720));
	memset (r, 0, sizeof (db_history_20150720));
	return (r);
}

void db_history_20150720__free (db_history_20150720 *rec)
{
	if (rec->operation != NULL) free (rec->operation);
	free (rec);
}

int db_history_20150720__save (db_history_20150720 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_save [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150720_save [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150720_save [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_save [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150720_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_save));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150720_save);
	}
	return (0);
}
int db_history_20150720__delete (db_history_20150720 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_delete [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150720_delete [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150720_delete [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_delete [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150720_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_delete));
		return (-1);
	}
	return (0);
}
int db_history_20150720__update (db_history_20150720 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_upd [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150720_upd [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150720_upd [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_upd [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150720_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_upd));
		return (-1);
	}
	return (0);
}
int db_history_20150720__insert (db_history_20150720 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_ins [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150720_ins [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150720_ins [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150720_ins [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150720_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_ins));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150720_ins);
	}
	return (0);
}
db_history_20150720 *db_history_20150720__get_by_id (int id)
{
	int ret;
	db_history_20150720 *rec;
	copyval_c2mysql_int (& id, & db_struct.par_history_20150720_get_by_id [0]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150720_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_get_by_id));
		return (NULL);
	}
	ret = mysql_stmt_fetch (db_struct.stmt_history_20150720_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_get_by_id));
		return (NULL);
	}
	rec = db_history_20150720__new ();
	copyval_mysql2c_int (& db_struct.res_history_20150720_load [0], & rec->id);
	copyval_mysql2c_char (& db_struct.res_history_20150720_load [1], & rec->operation);
	copyval_mysql2c_time (& db_struct.res_history_20150720_load [2], & rec->time);
	ret = mysql_stmt_reset (db_struct.stmt_history_20150720_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150720_get_by_id));
		return (NULL);
	}
	return (rec);
}
db_history_20150722 *db_history_20150722__new ()
{
	db_history_20150722 *r;
	r = malloc (sizeof (db_history_20150722));
	memset (r, 0, sizeof (db_history_20150722));
	return (r);
}

void db_history_20150722__free (db_history_20150722 *rec)
{
	if (rec->operation != NULL) free (rec->operation);
	free (rec);
}

int db_history_20150722__save (db_history_20150722 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_save [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150722_save [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150722_save [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_save [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150722_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_save));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150722_save);
	}
	return (0);
}
int db_history_20150722__delete (db_history_20150722 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_delete [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150722_delete [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150722_delete [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_delete [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150722_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_delete));
		return (-1);
	}
	return (0);
}
int db_history_20150722__update (db_history_20150722 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_upd [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150722_upd [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150722_upd [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_upd [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150722_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_upd));
		return (-1);
	}
	return (0);
}
int db_history_20150722__insert (db_history_20150722 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_ins [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150722_ins [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150722_ins [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150722_ins [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150722_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_ins));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150722_ins);
	}
	return (0);
}
db_history_20150722 *db_history_20150722__get_by_id (int id)
{
	int ret;
	db_history_20150722 *rec;
	copyval_c2mysql_int (& id, & db_struct.par_history_20150722_get_by_id [0]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150722_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_get_by_id));
		return (NULL);
	}
	ret = mysql_stmt_fetch (db_struct.stmt_history_20150722_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_get_by_id));
		return (NULL);
	}
	rec = db_history_20150722__new ();
	copyval_mysql2c_int (& db_struct.res_history_20150722_load [0], & rec->id);
	copyval_mysql2c_char (& db_struct.res_history_20150722_load [1], & rec->operation);
	copyval_mysql2c_time (& db_struct.res_history_20150722_load [2], & rec->time);
	ret = mysql_stmt_reset (db_struct.stmt_history_20150722_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150722_get_by_id));
		return (NULL);
	}
	return (rec);
}
db_history_20150723 *db_history_20150723__new ()
{
	db_history_20150723 *r;
	r = malloc (sizeof (db_history_20150723));
	memset (r, 0, sizeof (db_history_20150723));
	return (r);
}

void db_history_20150723__free (db_history_20150723 *rec)
{
	if (rec->operation != NULL) free (rec->operation);
	free (rec);
}

int db_history_20150723__save (db_history_20150723 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_save [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150723_save [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150723_save [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_save [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150723_save);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_save));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150723_save);
	}
	return (0);
}
int db_history_20150723__delete (db_history_20150723 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_delete [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150723_delete [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150723_delete [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_delete [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150723_delete);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_delete));
		return (-1);
	}
	return (0);
}
int db_history_20150723__update (db_history_20150723 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_upd [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150723_upd [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150723_upd [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_upd [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150723_upd);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_upd));
		return (-1);
	}
	return (0);
}
int db_history_20150723__insert (db_history_20150723 *record)
{
	int ret;
	
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_ins [0]);
	copyval_c2mysql_char (& record->operation, & db_struct.par_history_20150723_ins [1]);
	copyval_c2mysql_time (& record->time, & db_struct.par_history_20150723_ins [2]);
	copyval_c2mysql_int (& record->id, & db_struct.par_history_20150723_ins [3]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150723_ins);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_ins));
		return (-1);
	}
	if (record->id == 0) {
		record->id = mysql_stmt_insert_id (db_struct.stmt_history_20150723_ins);
	}
	return (0);
}
db_history_20150723 *db_history_20150723__get_by_id (int id)
{
	int ret;
	db_history_20150723 *rec;
	copyval_c2mysql_int (& id, & db_struct.par_history_20150723_get_by_id [0]);
	ret = mysql_stmt_execute (db_struct.stmt_history_20150723_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_get_by_id));
		return (NULL);
	}
	ret = mysql_stmt_fetch (db_struct.stmt_history_20150723_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_get_by_id));
		return (NULL);
	}
	rec = db_history_20150723__new ();
	copyval_mysql2c_int (& db_struct.res_history_20150723_load [0], & rec->id);
	copyval_mysql2c_char (& db_struct.res_history_20150723_load [1], & rec->operation);
	copyval_mysql2c_time (& db_struct.res_history_20150723_load [2], & rec->time);
	ret = mysql_stmt_reset (db_struct.stmt_history_20150723_get_by_id);
	if (ret) {
		fprintf (stderr, __FILE__":%d: error (%s)\n", __LINE__, mysql_stmt_error (db_struct.stmt_history_20150723_get_by_id));
		return (NULL);
	}
	return (rec);
}
