#ifndef __NVS_CONFIG_H__
#define __NVS_CONFIG_H__

#include <cstdint>

// Defaults for projects that compile the sources directly.
#ifndef NVS_CONF_DEBUG
#define NVS_CONF_DEBUG 1
#endif

#ifndef NVS_CONF_MAXBINARYCELLS_COUNT
#define NVS_CONF_MAXBINARYCELLS_COUNT 2
#endif

#ifndef NVS_CONF_TYPEOF_STATE
#define NVS_CONF_TYPEOF_STATE uint32_t
#endif

#ifndef NVS_CONF_MAXSIZE_OF_CELL_VALUES
#define NVS_CONF_MAXSIZE_OF_CELL_VALUES 8
#endif

#ifndef NVS_CONF_USE_STRING_KEY
#define NVS_CONF_USE_STRING_KEY 1
#endif

#if NVS_CONF_USE_STRING_KEY && !defined(NVS_CONF_KEY_SIZE)
#define NVS_CONF_KEY_SIZE 14
#endif

#ifndef NVS_CONF_TAG_START_VALUE
#define NVS_CONF_TAG_START_VALUE 0x35353535
#endif

#ifndef NVS_CONF_ERASED_STATE_VALUE
#define NVS_CONF_ERASED_STATE_VALUE 0xFFFFFFFF
#endif

#ifndef NVS_CONF_VALID_STATE_VALUE
#define NVS_CONF_VALID_STATE_VALUE 0xAAAAAAAA
#endif

#ifndef NVS_CONF_RELEASED_STATE_VALUE
#define NVS_CONF_RELEASED_STATE_VALUE 0x00000000
#endif


/************************************/

#define NVS_CONF_TYPEOF_TAG NVS_CONF_TYPEOF_STATE

typedef NVS_CONF_TYPEOF_TAG NVS_Tag_t;

typedef NVS_CONF_TYPEOF_STATE NVS_State_t;

#if NVS_CONF_USE_STRING_KEY != 0
typedef const char* NVS_Key_t;
#else
typedef uint16_t NVS_Key_t;
#endif



#endif /* __NVS_CONFIG_H__ */
