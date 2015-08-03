#ifndef __db__
#define __db__
#include <mysql/mysql.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct db_directory_info {
	int directory_id;
	char * directory_name;
	char * disk_uuid;
	char * disk_name;
	int parent_id;
	double directory_size;
	time_t time;
	time_t recent_use_time;
	int permission;
	int accessed_time;
	int file_number;
} db_directory_info;
typedef struct db_disk_info {
	int disk_id;
	char * disk_name;
	char * disk_uuid;
	char * disk_type;
	double disk_capacity;
	double disk_used;
	time_t recent_use_time;
	int permission;
	int disk_status;
} db_disk_info;
typedef struct db_file_info {
	int file_id;
	char * file_name;
	char * disk_uuid;
	char * directory_name;
	char * disk_name;
	char * md5;
	double file_size;
	double ra_val;
	double dec_val;
	time_t time;
	time_t recent_use_time;
	int permission;
	int file_type;
	int accessed_time;
} db_file_info;
typedef struct db_history_20150716 {
	int id;
	char * operation;
	time_t time;
} db_history_20150716;
typedef struct db_history_20150720 {
	int id;
	char * operation;
	time_t time;
} db_history_20150720;
typedef struct db_history_20150722 {
	int id;
	char * operation;
	time_t time;
} db_history_20150722;
typedef struct db_history_20150723 {
	int id;
	char * operation;
	time_t time;
} db_history_20150723;
int db_init (MYSQL *);

db_directory_info *db_directory_info__new ();
void db_directory_info__free (db_directory_info*);
int db_directory_info__save (db_directory_info *record);
int db_directory_info__update (db_directory_info *record);
int db_directory_info__insert (db_directory_info *record);
int db_directory_info__delete (db_directory_info *record);
db_directory_info *db_directory_info__get_by_id (int directory_id);

db_disk_info *db_disk_info__new ();
void db_disk_info__free (db_disk_info*);
int db_disk_info__save (db_disk_info *record);
int db_disk_info__update (db_disk_info *record);
int db_disk_info__insert (db_disk_info *record);
int db_disk_info__delete (db_disk_info *record);
db_disk_info *db_disk_info__get_by_id (int disk_id);

db_file_info *db_file_info__new ();
void db_file_info__free (db_file_info*);
int db_file_info__save (db_file_info *record);
int db_file_info__update (db_file_info *record);
int db_file_info__insert (db_file_info *record);
int db_file_info__delete (db_file_info *record);
db_file_info *db_file_info__get_by_id (int file_id);

db_history_20150716 *db_history_20150716__new ();
void db_history_20150716__free (db_history_20150716*);
int db_history_20150716__save (db_history_20150716 *record);
int db_history_20150716__update (db_history_20150716 *record);
int db_history_20150716__insert (db_history_20150716 *record);
int db_history_20150716__delete (db_history_20150716 *record);
db_history_20150716 *db_history_20150716__get_by_id (int id);

db_history_20150720 *db_history_20150720__new ();
void db_history_20150720__free (db_history_20150720*);
int db_history_20150720__save (db_history_20150720 *record);
int db_history_20150720__update (db_history_20150720 *record);
int db_history_20150720__insert (db_history_20150720 *record);
int db_history_20150720__delete (db_history_20150720 *record);
db_history_20150720 *db_history_20150720__get_by_id (int id);

db_history_20150722 *db_history_20150722__new ();
void db_history_20150722__free (db_history_20150722*);
int db_history_20150722__save (db_history_20150722 *record);
int db_history_20150722__update (db_history_20150722 *record);
int db_history_20150722__insert (db_history_20150722 *record);
int db_history_20150722__delete (db_history_20150722 *record);
db_history_20150722 *db_history_20150722__get_by_id (int id);

db_history_20150723 *db_history_20150723__new ();
void db_history_20150723__free (db_history_20150723*);
int db_history_20150723__save (db_history_20150723 *record);
int db_history_20150723__update (db_history_20150723 *record);
int db_history_20150723__insert (db_history_20150723 *record);
int db_history_20150723__delete (db_history_20150723 *record);
db_history_20150723 *db_history_20150723__get_by_id (int id);

#ifdef __cplusplus
}
#endif
#endif
