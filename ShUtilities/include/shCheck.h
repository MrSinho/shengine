#ifndef SH_CHECK_H
#define SH_CHECK_H

typedef enum ShErrorType {
	SH_FATAL_ERROR	= 0,
	SH_WARNING		= 1,
} ShErrorType;

extern int shCheckResult(int result, ShErrorType type, const char* failingmsg);

extern void shCheckValue(const int result, const int desiredValue, const char* errormsg);

#endif//SH_CHECK_H