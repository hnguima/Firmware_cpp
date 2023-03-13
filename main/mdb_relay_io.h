
#ifndef __MDB_RELAY_IO__
#define __MDB_RELAY_IO__

#include <esp_err.h>

#define ENABLE_3V 0
#define GPIO_OUTPUT_PIN_SEL (1ULL << ENABLE_3V)

#ifdef __cplusplus
extern "C"
{
#endif

  esp_err_t mdb_relay_io_init();

#ifdef __cplusplus
}
#endif

#endif // __MDB_RELAY_IO__
