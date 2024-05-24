#ifndef __NVS_CONFIG_H__
#define __NVS_CONFIG_H__

#include <cstdint>

#define NVS_CONF_DEBUG 1

#define NVS_CONF_MAXBINARYCELLS_COUNT 2

#define NVS_CONF_KEY_SIZE 14

#define NVS_CONF_TYPEOF_TAG uint32_t

#define NVS_CONF_TYPEOF_STATE uint32_t

#define NVS_CONF_MAXSIZE_OF_CELL_VALUES 8


/************************************/
typedef NVS_CONF_TYPEOF_TAG NVS_Tag_t;

typedef NVS_CONF_TYPEOF_STATE NVS_State_t;


#endif /* __NVS_CONFIG_H__ */