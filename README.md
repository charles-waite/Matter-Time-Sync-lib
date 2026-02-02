# Matter Time Sync Manager (ESP32)

Small ESP-IDF component to expose Matter Time Synchronization (Thread) as a simple API.

## What it does
- Creates the Matter Time Synchronization cluster on the root endpoint (endpoint 0).
- Exposes helpers to check if time is valid and to read UTC seconds.
- Optional one-time callback when time becomes available.

## Requirements
Enable this in your app config (no Wi-Fi required; just enables the clock API):

```
CONFIG_ENABLE_SNTP_TIME_SYNC=y
```

Then reconfigure or set-target to apply.

## Usage
```cpp
#include "time_sync_manager.h"

void setup() {
  // Create cluster before Matter.begin()
  time_sync_init();
  Matter.begin();
}

void loop() {
  time_sync_poll();

  int64_t utcSec = 0;
  if (time_sync_now_utc(&utcSec)) {
    // use utcSec
  }
}
```

## Notes
- Controllers may cache descriptors. Recommission after adding the cluster.
- If time never arrives, your controller may not send SetUTCTime.

