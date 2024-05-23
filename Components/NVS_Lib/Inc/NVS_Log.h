#ifndef __NVS_LOG_H__
#define __NVS_LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

#define NVS_DEBUG 1

#if NVS_DEBUG != 0
#define NVS_LOG(...) printf(__VA_ARGS__)
#else
#define NVS_LOG(TEXT, ...)
#endif


#ifdef __cplusplus
}
#endif


#endif /* __NVS_LOG_H__ */
