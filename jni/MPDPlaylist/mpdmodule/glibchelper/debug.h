/**  TBD: add STM Header
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__
#include <string.h>
#include <android/log.h>

#ifndef DEBUG
#define LOGAST(TAG, format, args...) do { \
}while(0)
#else
#define LOGAST(TAG, format, args...) do { \
	char msg[2048]; \
	sprintf(msg, "%s (%d) :  ", __FUNCTION__, __LINE__); \
	sprintf( msg + strlen(msg), format, args); \
	__android_log_write(ANDROID_LOG_INFO, TAG, msg); \
	}while(0);
#endif

#define LOGASTW(TAG, format, args...) do { \
	char msg[2048]; \
	sprintf(msg, "%s (%d) :  ", __FUNCTION__, __LINE__); \
	sprintf( msg + strlen(msg), format, args); \
	__android_log_write(ANDROID_LOG_INFO, TAG, msg); \
	}while(0);

#define LOGASTE LOGASTW

#endif /* __DEBUG_H__ */
