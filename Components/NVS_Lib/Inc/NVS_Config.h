#ifndef __NVS_CONFIG_H__
#define __NVS_CONFIG_H__

#include <cstdint>

#define NVS_CONF_DEBUG 1

#define NVS_CONF_MAXBINARYCELLS_COUNT 2

#define NVS_CONF_KEY_SIZE 14

#define NVS_CONF_TYPEOF_TAG uint32_t

#define NVS_CONF_TYPEOF_STATE uint32_t

#define NVS_CONF_MAXSIZE_OF_CELL_VALUES 8

#define NVS_CONF_USE_STRING_KEY 0

#define NVS_CONF_TAG_START_VALUE 0x35353535

#define NVS_CONF_ERASED_STATE_VALUE 0xFFFFFFFF

#define NVS_CONF_VALID_STATE_VALUE 0xAAAAAAAA

#define NVS_CONF_RELEASED_STATE_VALUE 0x00000000


/************************************/
typedef NVS_CONF_TYPEOF_TAG NVS_Tag_t;

typedef NVS_CONF_TYPEOF_STATE NVS_State_t;

#if NVS_CONF_USE_STRING_KEY == 1
typedef const char* NVS_Key_t;
#else
typedef uint16_t NVS_Key_t;
#endif



#endif /* __NVS_CONFIG_H__ */
