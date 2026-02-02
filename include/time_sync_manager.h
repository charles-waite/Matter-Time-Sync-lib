#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize Time Synchronization cluster on endpoint 0.
void time_sync_init(void);

// Poll for time availability; fires callback once when time becomes valid.
void time_sync_poll(void);

// Returns true if system time is available.
bool time_sync_has_time(void);

// Writes current UTC seconds into out_utc_sec. Returns false if time not available.
bool time_sync_now_utc(int64_t *out_utc_sec);

// Optional: call once when time first becomes available.
typedef void (*time_sync_ready_cb_t)(int64_t utc_sec);
void time_sync_set_ready_callback(time_sync_ready_cb_t cb);

#ifdef __cplusplus
}
#endif
