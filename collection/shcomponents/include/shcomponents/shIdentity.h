#ifndef SH_IDENTITY_H
#define SH_IDENTITY_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#define SH_IDENTITY_STR256_LENGTH 32



typedef struct ShIdentity {
	char    name   [SH_IDENTITY_STR256_LENGTH];
	char    tag    [SH_IDENTITY_STR256_LENGTH];
	char    subtag [SH_IDENTITY_STR256_LENGTH];
	uint8_t active;
} ShIdentity;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif // SH_IDENTITY_H