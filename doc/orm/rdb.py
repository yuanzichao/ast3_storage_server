# license:  http://www.opensource.org/licenses/lgpl-3.0.html
# author:   Alesak <alesak@gmail.com>
# requires: MySQLdb module, db.py config file

import MySQLdb as DB
import re

from db import tables as spec_tables
from db import name as spec_name
from db import dbname as spec_database
try:
	pass
except:
	print "ERROR"
	tables = {}
	pass

class RType:
	T_INT     = 1
	T_DOUBLE  = 2
	T_STRING  = 3
	T_DATA    = 4
	T_TIMESTAMP = 5
	cnv = {
		T_INT      :("int","MYSQL_TYPE_LONG", "sizeof(int)", "0", "copyval_c2mysql_int", "copyval_mysql2c_int",False),
                T_DOUBLE   :("double","MYSQL_TYPE_DOUBLE", "sizeof(double)", "0", "copyval_c2mysql_double", "copyval_mysql2c_double",False),
		T_STRING   :("char *","MYSQL_TYPE_STRING", "%d + 1", "%d", "copyval_c2mysql_char", "copyval_mysql2c_char", True),
		T_TIMESTAMP:("time_t","MYSQL_TYPE_DATETIME", "sizeof (MYSQL_TIME)", "0", "copyval_c2mysql_time", "copyval_mysql2c_time", False),
		T_DATA     :("void*","MYSQL_TYPE_BLOB", "100", "100", "copyval_c2mysql_char", "copyval_mysql2c_char", True)}

	def __init__ (self):
		self.type = 0
		self.length = -1

	def c_type (self):
		str = RType.cnv [self.type][0]
		return str
	def c_mysql (self):
		str = RType.cnv [self.type]
		return str
	def c_mysql_buffertype (self):
		str = RType.cnv [self.type][1]
		return str
	

class RField:
	def __init__(self):
		self.name = ""
		self.type = RType ()

class RTable:
	def __init__(self):
		self.fields = []
		self.primary = []

class RDb:
	def __init__(self):
		self.tables = []
		self.name = spec_name
		self.prefix = ""

	def load_from_db (self, db):
		pass

	def produce_source (self):
		self.produce_h ()
		self.produce_c ()

	def produce_c (self):
		out = file (self.prefix + self.name + ".c","w")

		out.write ("#define  _GNU_SOURCE 1\n")
		out.write ("#include <%s.h>\n" % (self.prefix + self.name))
		out.write ("#include <stdlib.h>\n#include <string.h>\n#include <stdio.h>\n")
		out.write ("#include <time.h>\n")
		out.write ("#include <mysql/mysql.h>\n")
		out.write ("#include <mysql/mysqld_error.h>\n")

		out.write ("struct {\n")

		strname = "%s%s_struct" % (self.prefix, self.name)

		for t in self.tables:
			s = "%s%s_%s" % (self.prefix, self.name, t.name)
			out.write ("\tMYSQL_STMT *stmt_%s_upd;\n" % t.name)
			out.write ("\tMYSQL_BIND par_%s_upd [%d];\n" % (t.name, len (t.fields)+len(t.primary)))
			out.write ("\tMYSQL_STMT *stmt_%s_ins;\n" % t.name)
			out.write ("\tMYSQL_BIND par_%s_ins [%d];\n" % (t.name, len (t.fields)+len(t.primary)))
			out.write ("\tMYSQL_STMT *stmt_%s_save;\n" % t.name)
			out.write ("\tMYSQL_BIND par_%s_save [%d];\n" % (t.name, len (t.fields)+len(t.primary)))
			out.write ("\tMYSQL_BIND res_%s_load [%d];\n" % (t.name, len (t.fields)))

			if len (t.primary) > 0:
				out.write ("\tMYSQL_STMT *stmt_%s_get_by_id;\n" % t.name)
				out.write ("\tMYSQL_BIND par_%s_get_by_id [%d];\n" % (t.name, len (t.primary)))
				out.write ("\tMYSQL_BIND res_%s_get_by_id [%d];\n" % (t.name, len (t.fields)))
				out.write ("\tMYSQL_STMT *stmt_%s_delete;\n" % t.name)
				out.write ("\tMYSQL_BIND par_%s_delete [%d];\n" % (t.name, len (t.primary)))

			if spec_tables.has_key (t.name):
				for im in spec_tables [t.name]:
					if im[0] == 'get':
						out.write ("\tMYSQL_STMT *stmt_%s_%s;\n" % (t.name, im[1]))
						out.write ("\tMYSQL_BIND par_%s_%s [%d];\n" % (t.name, im[1], len(im[2])))
						"""
						for par in im[2]:
							ty = RType ()
							typ = self.parse_type (par[0], ty)
							out.write ("%s %s" % (ty.c_mysql ()[0], par[1]))
						out.write (");\n")
						out.write ("%s *%s__%s_fetch ()\n{\n\tint ret;\n\t%s *rec;\n" % (t.name, t.name, im[1], t.name))
						out.write ("\tret = mysql_stmt_fetch (%s.stmt_%s_%s);\n" % (strname, t.name, im[1]))
						"""



		out.write ("} %s%s_struct;\n" % (self.prefix, self.name))

		out.write ("static int copyval_c2mysql_int (int *val, MYSQL_BIND* bnd)\n{\n\tmemcpy (bnd->buffer, val, sizeof (int));\n\treturn (0);\n}\n");
		out.write ("static int copyval_c2mysql_long (long *val, MYSQL_BIND* bnd)\n{\n\tmemcpy (bnd->buffer, val, sizeof (long));\n\treturn (0);\n}\n");
                out.write ("static int copyval_c2mysql_double (double *val, MYSQL_BIND* bnd)\n{\n\tmemcpy (bnd->buffer, val, sizeof (double));\n\treturn (0);\n}\n");
		out.write ("static int copyval_c2mysql_char (char **val, MYSQL_BIND* bnd)\n{\n\tstrncpy (bnd->buffer, * val, bnd->buffer_length);if (bnd->length) *(unsigned int*)bnd->length = (val && *val)?strlen (*val):0;\n\treturn (0);\n}\n");
		out.write ("static int copyval_c2mysql_time (time_t *val, MYSQL_BIND* bnd)\n{\n\tstruct tm *t; MYSQL_TIME *m;\n\tt = localtime ((const time_t*) val); m = (MYSQL_TIME*)bnd->buffer; memset (m, 0, sizeof (MYSQL_TIME));\n\tm->time_type = MYSQL_TIMESTAMP_DATETIME;\n\tm->year = t->tm_year + 1900; m->month = t->tm_mon + 1; m->day = t->tm_mday; m->hour = t->tm_hour; m->minute = t->tm_min; m->second = t->tm_sec;\n\treturn (0);\n}\n");

		out.write ("static int copyval_mysql2c_char (MYSQL_BIND* bnd, char **c)\n{\n\t*c = (strndup (bnd->buffer, bnd->buffer_length));\n\treturn (0);\n}\n");

		out.write ("static int copyval_mysql2c_int (MYSQL_BIND* bnd, int *i)\n{\n\tint *val;\n\t*i = * (int*) bnd->buffer;\n\treturn (0);\n}\n");
		out.write ("static int copyval_mysql2c_long (MYSQL_BIND* bnd, long *i)\n{\n\tlong *val;\n\t*i = * (long*) bnd->buffer;\n\treturn (0);\n}\n");
                out.write ("static int copyval_mysql2c_double (MYSQL_BIND* bnd, double *i)\n{\n\tdouble *val;\n\t*i = * (double*) bnd->buffer;\n\treturn (0);\n}\n");
		out.write ("static int copyval_mysql2c_time (MYSQL_BIND* bnd, time_t *i)\n{\n\tMYSQL_TIME *m;struct tm t;\n\tm = (MYSQL_TIME*)bnd->buffer;\n\tmemset (& t, 0, sizeof (struct tm));\n\tt.tm_year = m->year - 1900; t.tm_mon = m->month - 1; t.tm_mday = m->day;\n\tt.tm_hour = m->hour;\n\tt.tm_min = m->minute;\n\tt.tm_sec = m->second; *i = mktime (& t);\n\treturn (0);\n}\n");

		out.write ("int %s%s_init (MYSQL *mysql)\n{\n" % (self.prefix, self.name))
		str = "%s%s_struct" % (self.prefix, self.name)

		out.write ("\tmemset (& %s, 0, sizeof (%s));\n" % (str, str))
		out.write ("\tchar *sql;\n\tint ret;\n")


		for t in self.tables:
			s = "%s%s_%s" % (self.prefix, self.name, t.name)
			i = 0
			s_load = "SELECT "
			for f in (t.fields):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.res_%s_load [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.res_%s_load [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.res_%s_load [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				if i > 0: s_load += ", "
				s_load += f.name

				i += 1

			i = 0

			for f in (t.primary):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_get_by_id [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_get_by_id [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_get_by_id [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				i += 1

			i = 0

			for f in (t.primary):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_delete [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_delete [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_delete [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				i += 1

			s_load += " FROM %s" % (t.name)
			if len (t.primary):
				out.write ("\t%s.stmt_%s_get_by_id = mysql_stmt_init (mysql);\n" % (strname, t.name))
				cpri = 0
				for pri in t.primary:
					if cpri == 0: s_load += " WHERE "
					else: s_load += "AND"
					s_load += pri.name 
					s_load += " = ? "
					cpri += 1

				out.write ("\tsql = \"%s\";\n" % s_load)
				out.write ("\tret = mysql_stmt_prepare (%s.stmt_%s_get_by_id , sql, strlen (sql));\n" % (strname, t.name))
				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_get_by_id));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))
				out.write ("\tret = mysql_stmt_bind_param (%s.stmt_%s_get_by_id, %s.par_%s_get_by_id);\n" % (strname, t.name, strname, t.name))
				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_get_by_id));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))
				out.write ("\tret = mysql_stmt_bind_result (%s.stmt_%s_get_by_id, %s.res_%s_load);\n" % (strname, t.name, strname, t.name))
				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_get_by_id));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

				out.write ("\t%s.stmt_%s_delete = mysql_stmt_init (mysql);\n" % (strname, t.name))
				cpri = 0
				s_del = "DELETE FROM %s " % t.name
				for pri in t.primary:
					if cpri == 0: s_del += " WHERE "
					else: s_del += "AND"
					s_del += pri.name 
					s_del += " = ? "
					cpri += 1

				out.write ("\tsql = \"%s\";\n" % s_del)
				out.write ("\tret = mysql_stmt_prepare (%s.stmt_%s_delete, sql, strlen (sql));\n" % (strname, t.name))
				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_delete));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))
				out.write ("\tret = mysql_stmt_bind_param (%s.stmt_%s_delete, %s.par_%s_delete);\n" % (strname, t.name, strname, t.name))
				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_get_by_id));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

			i = 0
			s_save = "REPLACE %s SET " % t.name
			#s_save = "INSERT INTO %s (" % t.name
			for f in (t.fields):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_save [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_save [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_save [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				if m [6]:
					out.write ("\t%s.par_%s_save [%d].length = malloc (sizeof (unsigned int));\n" % (strname, t.name, i))
				if i > 0: s_save += ", "
				s_save += f.name + " = ? "
				i += 1

			for f in (t.primary):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_save [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_save [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_save [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				i += 1

			out.write ("\t%s.stmt_%s_save = mysql_stmt_init (mysql);\n" % (strname, t.name))
			cpri = 0
			"""
			for pri in t.primary:
				if cpri == 0: s_save += "WHERE "
				else: s_save += "AND"
				s_save += pri.name 
				s_save += " = ? "
				cpri += 1
			"""

			out.write ("\tsql = \"%s\";\n" % s_save)
			out.write ("\tret = mysql_stmt_prepare (%s.stmt_%s_save, sql, strlen (sql));\n" % (strname, t.name))
			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_save));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))


			out.write ("\tret = mysql_stmt_bind_param (%s.stmt_%s_save, %s.par_%s_save);\n" % (strname, t.name, strname, t.name))
			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_save));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

			i = 0
			s_save = "UPDATE %s SET " % t.name
			#s_save = "INSERT INTO %s (" % t.name
			for f in (t.fields):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_upd [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_upd [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_upd [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				if m [6]:
					out.write ("\t%s.par_%s_upd [%d].length = malloc (sizeof (unsigned int));\n" % (strname, t.name, i))
				if i > 0: s_save += ", "
				s_save += f.name + " = ? "
				i += 1

			for f in (t.primary):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_upd [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_upd [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_upd [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				i += 1

			out.write ("\t%s.stmt_%s_upd = mysql_stmt_init (mysql);\n" % (strname, t.name))
			cpri = 0

			for pri in t.primary:
				if cpri == 0: s_save += "WHERE "
				else: s_save += "AND"
				s_save += pri.name 
				s_save += " = ? "
				cpri += 1

			out.write ("\tsql = \"%s\";\n" % s_save)
			out.write ("\tret = mysql_stmt_prepare (%s.stmt_%s_upd, sql, strlen (sql));\n" % (strname, t.name))
			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_upd));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))


			out.write ("\tret = mysql_stmt_bind_param (%s.stmt_%s_upd, %s.par_%s_upd);\n" % (strname, t.name, strname, t.name))
			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_upd));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

			i = 0
			s_ins = "INSERT INTO %s (" % t.name
			for f in (t.fields):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_ins [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_ins [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_ins [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				if m [6]:
					out.write ("\t%s.par_%s_ins [%d].length = malloc (sizeof (unsigned int));\n" % (strname, t.name, i))
				if i > 0: s_ins += ", "
				s_ins += f.name #+ " = ? "
				i += 1
			s_ins += ") VALUES ("
			i = 0
			for f in t.fields:
				if i > 0: s_ins += ", "
				s_ins += "?"
				i += 1
			s_ins += ")"

			for f in (t.primary):
				f._index = i
				m = f.type.c_mysql ()
				out.write ("\t%s.par_%s_ins [%d].buffer_type = %s;\n" % (strname, t.name, i, m [1]))

				ml = m [2]
				if ml.count ('%'): ml = ml % f.type.length

				out.write ("\t%s.par_%s_ins [%d].buffer = malloc (%s);\n" % (strname, t.name, i, ml))
				ml = m [3]
				if ml.count ('%'): ml = ml % f.type.length
				out.write ("\t%s.par_%s_ins [%d].buffer_length = %s;\n" % (strname, t.name, i, ml))
				i += 1

			out.write ("\t%s.stmt_%s_ins = mysql_stmt_init (mysql);\n" % (strname, t.name))
			cpri = 0
			"""
			for pri in t.primary:
				if cpri == 0: s_save += "WHERE "
				else: s_save += "AND"
				s_save += pri.name 
				s_save += " = ? "
				cpri += 1
			"""

			out.write ("\tsql = \"%s\";\n" % s_ins)
			out.write ("\tret = mysql_stmt_prepare (%s.stmt_%s_ins, sql, strlen (sql));\n" % (strname, t.name))
			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_ins));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))


			out.write ("\tret = mysql_stmt_bind_param (%s.stmt_%s_ins, %s.par_%s_ins);\n" % (strname, t.name, strname, t.name))
			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_ins));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

			if spec_tables.has_key (t.name):
				for im in spec_tables [t.name]:
					if im[0] == 'get':
						i = 0
						for f in im [2]:
							ty = RType ()
							self.parse_type (f [0], ty)
							m = ty.c_mysql ()
							out.write ("\t%s.par_%s_%s [%d].buffer_type = %s;\n" % (strname, t.name, im[1], i, m [1]))

							ml = m [2]
							if ml.count ('%'): ml = ml % ty.length

							out.write ("\t%s.par_%s_%s [%d].buffer = malloc (%s);\n" % (strname, t.name, im[1], i, ml))
							ml = m [3]
							if ml.count ('%'): ml = ml % ty.length
							out.write ("\t%s.par_%s_%s [%d].buffer_length = %s;\n" % (strname, t.name, im [1], i, ml))
							i += 1

						out.write ("\t%s.stmt_%s_%s = mysql_stmt_init (mysql);\n" % (strname, t.name, im[1]))
						out.write ("\tsql = \"%s\";\n" % im[3])
						out.write ("\tret = mysql_stmt_prepare (%s.stmt_%s_%s, sql, strlen (sql));\n" % (strname, t.name, im[1]))
						out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_%s));\n\t\treturn (-1);\n\t}\n" % (strname, t.name, im[1]))

						out.write ("\tret = mysql_stmt_bind_result (%s.stmt_%s_%s, %s.res_%s_load);\n" % (strname, t.name, im[1], strname, t.name))
						out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_%s));\n\t\treturn (-1);\n\t}\n" % (strname, t.name, im[1]))

						out.write ("\tret = mysql_stmt_bind_param (%s.stmt_%s_%s, %s.par_%s_%s);\n" % (strname, t.name, im[1], strname, t.name, im [1]))
						out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_%s));\n\t\treturn (-1);\n\t}\n" % (strname, t.name, im[1]))


		out.write ("\treturn (0);\n}\n\n")

		for t in self.tables:
			s = "%s%s_%s" % (self.prefix, self.name, t.name)
			out.write ("%s *%s__new ()\n{\n\t%s *r;\n\tr = "
				"malloc (sizeof (%s));\n\tmemset (r, 0,"
				" sizeof (%s));\n\treturn (r);\n}\n\n" % (s, s, s, s, s))
			out.write ("void %s__free (%s *rec)\n{\n" % (s, s))

			for f in t.fields:
				if f.type.type == RType.T_STRING:
					out.write ("\tif (rec->%s != NULL) free (rec->%s);\n" % (f.name, f.name))
				elif f.type.type == RType.T_DATA:
					out.write ("\tif (rec->%s != NULL) free (rec->%s);\n" % (f.name, f.name))

			out.write ("\tfree (rec);\n}\n\n")

			out.write ("int %s__save (%s *record)\n{\n\tint ret;\n" % (s, s))
			out.write ("\t\n")
			i = 0

			for f in t.fields + t.primary:
				f._index = i
				m = f.type.c_mysql ()

				ml = m [2]

				out.write ("\t%s (& record->%s, & %s.par_%s_save [%d]);\n" % (m[4], f.name, strname, t.name, i))

				i += 1
			out.write ("\tret = mysql_stmt_execute (%s.stmt_%s_save);\n" % (strname, t.name))

			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_save));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

			if len (t.primary):
				pri = t.primary [0]
				out.write ("\tif (record->%s == 0" % pri.name)
#				for pri in t.primary [1:]:
#					out.write (" && %s == 0" % pri.name)

				out.write (") {\n")

				out.write ("\t\trecord->%s = mysql_stmt_insert_id (%s.stmt_%s_save);\n\t}\n" % (pri.name, strname, t.name))

				

			out.write ("\treturn (0);\n}\n")

			if len (t.primary):
				out.write ("int %s__delete (%s *record)\n{\n\tint ret;\n" % (s, s))
				out.write ("\t\n")
				i = 0

				for f in t.fields + t.primary:
					f._index = i
					m = f.type.c_mysql ()

					ml = m [2]

					out.write ("\t%s (& record->%s, & %s.par_%s_delete [%d]);\n" % (m[4], f.name, strname, t.name, i))

					i += 1
				out.write ("\tret = mysql_stmt_execute (%s.stmt_%s_delete);\n" % (strname, t.name))

				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_delete));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

				out.write ("\treturn (0);\n}\n")


				out.write ("int %s__update (%s *record)\n{\n\tint ret;\n" % (s, s))
				out.write ("\t\n")
				i = 0

				for f in t.fields + t.primary:
					f._index = i
					m = f.type.c_mysql ()

					ml = m [2]

					out.write ("\t%s (& record->%s, & %s.par_%s_upd [%d]);\n" % (m[4], f.name, strname, t.name, i))

					i += 1
				out.write ("\tret = mysql_stmt_execute (%s.stmt_%s_upd);\n" % (strname, t.name))

				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_upd));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))
				""" if len (t.primary):
					pri = t.primary [0]
					out.write ("\tif (record->%s == 0" % pri.name)
	#				for pri in t.primary [1:]:
	#					out.write (" && %s == 0" % pri.name)

					out.write (") {\n")

					out.write ("\t\trecord->%s = mysql_stmt_insert_id (%s.stmt_%s_save);\n\t}\n" % (pri.name, strname, t.name)) """
				out.write ("\treturn (0);\n}\n")


			out.write ("int %s__insert (%s *record)\n{\n\tint ret;\n" % (s, s))
			out.write ("\t\n")
			i = 0

			for f in t.fields + t.primary:
				f._index = i
				m = f.type.c_mysql ()

				ml = m [2]

				out.write ("\t%s (& record->%s, & %s.par_%s_ins [%d]);\n" % (m[4], f.name, strname, t.name, i))

				i += 1
			out.write ("\tret = mysql_stmt_execute (%s.stmt_%s_ins);\n" % (strname, t.name))

			out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_ins));\n\t\treturn (-1);\n\t}\n" % (strname, t.name))

			if len (t.primary):
				pri = t.primary [0]
				out.write ("\tif (record->%s == 0" % pri.name)
#				for pri in t.primary [1:]:
#					out.write (" && %s == 0" % pri.name)

				out.write (") {\n")

				out.write ("\t\trecord->%s = mysql_stmt_insert_id (%s.stmt_%s_ins);\n\t}\n" % (pri.name, strname, t.name))

				

			out.write ("\treturn (0);\n")


			out.write ("}\n")

			if len (t.primary):
				out.write ("%s *%s__get_by_id (" % (s, s))
				for p in t.primary:
					if not p == t.primary [0]:
						out.write (", ");
					out.write ("%s %s" % (p.type.c_type (), p.name))


				out.write (")\n{\n\tint ret;\n\t%s *rec;\n" % s)

				i = 0

				for f in (t.primary):
					f._index = i
					m = f.type.c_mysql ()

					ml = m [2]
					if ml.count ('%'): ml = ml % f.type.length

					out.write ("\t%s (& %s, & %s.par_%s_get_by_id [%d]);\n" % (m[4], f.name, strname, t.name, i))

					i += 1

				out.write ("\tret = mysql_stmt_execute (%s.stmt_%s_get_by_id);\n" % (strname, t.name))

				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_get_by_id));\n\t\treturn (NULL);\n\t}\n" % (strname, t.name))
				out.write ("\tret = mysql_stmt_fetch (%s.stmt_%s_get_by_id);\n" % (strname, t.name))

				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_get_by_id));\n\t\treturn (NULL);\n\t}\n" % (strname, t.name))

				out.write ("\trec = %s__new ();\n" % s)

				i = 0

				for f in t.fields:
					f._index = i
					m = f.type.c_mysql ()

					ml = m [2]

					out.write ("\t%s (& %s.res_%s_load [%d], & %s->%s);\n" % (m[5], strname, t.name, i, "rec", f.name))

					i += 1
		
				out.write ("\tret = mysql_stmt_reset (%s.stmt_%s_get_by_id);\n" % (strname, t.name))

				out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_get_by_id));\n\t\treturn (NULL);\n\t}\n" % (strname, t.name))
				out.write ("\treturn (rec);\n}\n")

			if spec_tables.has_key (t.name):
				for im in spec_tables [t.name]:
					if im[0] == 'get':
						out.write ("int %s__%s_open (" % (s, im[1]))
						for par in im[2]:
							ty = RType ()
							typ = self.parse_type (par[0], ty)
							out.write ("%s %s" % (ty.c_mysql ()[0], par[1]))
						out.write (")\n{\n\tint ret;\n\n")

						i = 0

						for f in im [2]:
							ty = RType ()
							self.parse_type (f [0], ty)
							m = ty.c_mysql ()

							ml = m [2]
							if ml.count ('%'): ml = ml % ty.length

							out.write ("\t%s (& %s, & %s.par_%s_%s [%d]);\n" % (m[4], f [1], strname, t.name, im [1], i))
							i += 1

						out.write ("\tret = mysql_stmt_execute (%s.stmt_%s_%s);\n" % (strname, t.name, im [1]))

						out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_%s));\n\t\treturn (-1);\n\t}\n" % (strname, t.name, im [1]))
			

						out.write ("\treturn (0);\n}\n")
						out.write ("%s *%s__%s_fetch ()\n{\n\tint ret;\n\t%s *rec;\n" % (s, s, im[1], s))
						out.write ("\tret = mysql_stmt_fetch (%s.stmt_%s_%s);\n" % (strname, t.name, im[1]))

						out.write ("\tif (ret == MYSQL_NO_DATA) {\n\t\treturn (NULL);\n\t} else if (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_%s));\n\t\treturn (NULL);\n\t}\n" % (strname, t.name, im[1]))

						out.write ("\trec = %s__new ();\n" % s)

						i = 0

						for f in t.fields:
							f._index = i
							m = f.type.c_mysql ()

							ml = m [2]

							out.write ("\t%s (& %s.res_%s_load [%d], & %s->%s);\n" % (m[5], strname, t.name, i, "rec", f.name))
							i += 1
						out.write ("\treturn (rec);\n}\n")
	
						out.write ("int %s__%s_close ()\n" % (s, im[1]))

						out.write ("{\n\tint ret;\n")

						out.write ("\tret = mysql_stmt_reset (%s.stmt_%s_%s);\n" % (strname, t.name, im[1]))

						out.write ("\tif (ret) {\n\t\tfprintf (stderr, __FILE__\":%%d: error (%%s)\\n\", __LINE__, mysql_stmt_error (%s.stmt_%s_%s));\n\t\treturn (-1);\n\t}\n" % (strname, t.name, im[1]))
						out.write ("\treturn (0);\n}\n")








	def produce_h (self):
		out = file (self.prefix + self.name + ".h","w")
		out.write ("#ifndef __%s%s__\n#define __%s%s__\n" % (self.prefix, self.name, self.prefix, self.name))
		out.write ("#include <mysql/mysql.h>\n\n")
		out.write ("#ifdef __cplusplus\nextern \"C\" {\n#endif\n")

		for t in self.tables:
			s = "%s%s_%s" % (self.prefix, self.name, t.name)
			out.write ("typedef struct %s%s_%s {\n" % (self.prefix, self.name, t.name))

			for f in t.fields:
				out.write ("\t%s %s;\n" % (f.type.c_type (), f.name))

			out.write ("} %s%s_%s;\n" % (self.prefix, self.name, t.name))
		out.write ("int %s%s_init (MYSQL *);\n\n" % (self.prefix, self.name))
		for t in self.tables:
			s = "%s%s_%s" % (self.prefix, self.name, t.name)
			out.write ("%s *%s__new ();\n" % (s, s))
			out.write ("void %s__free (%s*);\n" % (s, s))
			out.write ("int %s__save (%s *record);\n" % (s, s))
			out.write ("int %s__update (%s *record);\n" % (s, s))
			out.write ("int %s__insert (%s *record);\n" % (s, s))
			if len (t.primary) > 0:
				out.write ("int %s__delete (%s *record);\n" % (s, s))
				out.write ("%s *%s__get_by_id (" % (s, s))
				for p in t.primary:
					if not p == t.primary [0]:
						out.write (", ");
					out.write ("%s %s" % (p.type.c_type (), p.name))


				out.write (");\n")
			if spec_tables.has_key (t.name):
				for i in spec_tables [t.name]:
					if i[0] == 'get':
						out.write ("int %s__%s_open (" % (s, i[1]))
						for par in i[2]:
							ty = RType ()
							typ = self.parse_type (par[0], ty)
							out.write ("%s %s" % (ty.c_mysql ()[0], par[1]))
						out.write (");\n")
						out.write ("%s *%s__%s_fetch ();\n" % (s, s, i[1]))
						out.write ("int %s__%s_close ();\n" % (s, i[1]))



			out.write ("\n")

		out.write ("#ifdef __cplusplus\n}\n#endif\n")
		out.write ("#endif\n")
		out.close ()


class RDb_MySQL (RDb):

	def parse_type (self, stype, type):
		prstype = re.compile ("([a-z]*)\\(([0-9]*)\\)")

		if stype.startswith ("bigint"):
			type.type = RType.T_INT
		elif stype.startswith ("INTEGER"):
			type.type = RType.T_INT
		elif stype.startswith ("smallint"):
			type.type = RType.T_INT
		elif stype.startswith ("tinyint"):
			type.type = RType.T_INT
		elif stype.startswith ("enum("):
			type.type = RType.T_INT
		elif stype.startswith ("double unsigned"):
			type.type = RType.T_DOUBLE
		elif stype.startswith ("double"):
			type.type = RType.T_DOUBLE
		elif stype.startswith ("int"):
			type.type = RType.T_INT
		elif stype.startswith ("char"):
			type.type = RType.T_STRING
			m = prstype.match (stype)
			if not m == None:
				type.length = int (m.groups ()[1])
		elif stype.startswith ("varchar"):
			type.type = RType.T_STRING
			m = prstype.match (stype)
			if not m == None:
				type.length = int (m.groups ()[1])
		elif stype.startswith ("tinytext"):
			type.type = RType.T_STRING
			m = prstype.match (stype)
			if not m == None:
				type.length = int (m.groups ()[1])
		elif stype.startswith ("longtext"):
			type.type = RType.T_STRING
			m = prstype.match (stype)
			if not m == None:
				type.length = int (m.groups ()[1])
		elif stype.startswith ("mediumtext"):
			type.type = RType.T_STRING
		elif stype.startswith ("text"):
			type.type = RType.T_STRING
		elif stype.startswith ("mediumblob"):
			type.type = RType.T_DATA
		elif stype.startswith ("tinyblob"):
			type.type = RType.T_DATA
		elif stype.startswith ("varbinary"):
			type.type = RType.T_DATA
		elif stype.startswith ("blob"):
			type.type = RType.T_DATA
		elif stype.startswith ("datetime"):
			type.type = RType.T_TIMESTAMP
		elif stype.startswith ("date"):
			type.type = RType.T_TIMESTAMP
		elif stype.startswith ("timestamp"):
			type.type = RType.T_TIMESTAMP
		else:
			raise "Cannot parse type \'%s\'" % stype


	def load_from_db (self, db):
		ct = db.cursor ()
		cf = db.cursor ()
		ct.execute ("""SHOW TABLES""")
		for tr in ct.fetchall ():
			t = RTable ()
			t.name = tr [0]
			self.tables.append (t)
			cf.execute ("""SHOW FIELDS FROM %s""" % (t.name))
			for fr in cf.fetchall ():
				f = RField ()
				f.name = fr [0]
				self.parse_type (fr[1], f.type)
				t.fields.append (f)
				if fr [3] == 'PRI':
					t.primary.append (f)
		

# db = DB.connect (host = "localhost", user = "root", passwd = "kokosak", db = "wikidb")
db = DB.connect (host = "localhost", user = "root", passwd = "123456", db = spec_database)

rdb = RDb_MySQL ()
rdb.load_from_db (db)
rdb.produce_source ()
