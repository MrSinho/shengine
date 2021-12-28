#include "shCheck.h"
#include <stdlib.h>

#ifndef NDEBUG
#include <stdio.h>
#endif//NDEBUG

int shCheckResult(int result, ShErrorType type, const char* failingmsg) {
	if (!result) {
		if (type == SH_FATAL_ERROR) {
#ifndef NDEBUG
			printf("ShFatalError: %s\n", failingmsg);
#endif//NDEBUG
#ifdef _WIN32
			system("pause");
#endif//WIN32
			exit(-1);
		}
#ifndef NDEBUG
		printf("ShWarning: %s\n", failingmsg);
#endif//NDEBUG
	}
	return result;
}

void shCheckValue(const int result, const int desiredValue, const char* errormsg) {
	if (result != desiredValue) {
		perror(errormsg);
		exit(EXIT_FAILURE);
	}
}


