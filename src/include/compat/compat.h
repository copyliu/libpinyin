#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

#ifdef _WIN32
#include "getline.h"
#include <fcntl.h>
#endif

#include <glib/gstdio.h>
#include <unistd.h> // prevent #define in this header

#ifdef _WIN32
#define open open_compat
inline int open(const gchar* filename, int flags) {
	return g_open(filename, flags | _O_BINARY,
	              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}
inline int open(const gchar* filename, int flags, int mode) {
	return g_open(filename, flags | _O_BINARY, mode);
}
#else
#define open g_open
#endif

#define chmod g_chmod
#define creat g_creat
#define rename g_rename
#define mkdir g_mkdir
#define stat g_stat
#define lstat g_lstat
#define remove g_remove
#define fopen g_fopen
#define freopen g_freopen
#define fsync g_fsync
#define utime g_utime
