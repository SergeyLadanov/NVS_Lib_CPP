#ifndef __NVS_LOG_H__
#define __NVS_LOG_H__

#include "stdio.h"
#include "NVS_Config.h"
#include "NVS_Format.h"


#ifdef __cplusplus
extern "C"
{
#endif


#if NVS_CONF_DEBUG != 0
#define NVS_LOG(...) NVS_PRINTF(__VA_ARGS__)
#else
#define NVS_LOG(TEXT, ...)
#endif


#ifdef __cplusplus
}
#endif


#endif /* __NVS_LOG_H__ */
