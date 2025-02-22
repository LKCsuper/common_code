#ifndef __FATFS_WRAPPER_H__
#define __FATFS_WRAPPER_H__

#include <stdint.h>

extern int fs_format(void);

extern int fs_init(void);

extern int fs_init_ex(void);

extern uint32_t zayk_file_create(uint32_t cmd,void *pvpack, uint32_t vidx);

extern uint32_t zayk_file_read(uint32_t cmd,void *pvpack, uint32_t vidx);

extern uint32_t zayk_file_write(uint32_t cmd,void *pvpack, uint32_t vidx);

extern uint32_t zyak_file_delete(uint32_t cmd,void *pvpack, uint32_t vidx);

extern uint32_t zayk_file_enum(uint32_t cmd,void *pvpack, uint32_t vidx);

extern uint32_t zayk_file_length_get(uint32_t cmd,void *pvpack, uint32_t vidx);


#endif
