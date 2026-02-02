#include "time_sync_manager.h"

#include <esp_matter.h>
#include <esp_matter_cluster.h>
#include <esp_matter_endpoint.h>
#include <system/SystemClock.h>

static bool s_time_sync_ready = false;
static bool s_time_sync_cluster_ready = false;
static time_sync_ready_cb_t s_ready_cb = nullptr;

static bool get_utc_seconds(int64_t &out_sec)
{
    chip::System::Clock::Microseconds64 utc;
    if (chip::System::SystemClock().GetClock_RealTime(utc) != CHIP_NO_ERROR) {
        return false;
    }
    out_sec = static_cast<int64_t>(utc.count() / 1000000);
    return true;
}

void time_sync_init(void)
{
    esp_matter::node_t *node = esp_matter::node::get();
    if (!node) return;

    esp_matter::endpoint_t *root = esp_matter::endpoint::get(node, 0);
    if (!root) return;

    esp_matter::cluster::time_synchronization::config_t cfg;
    esp_matter::cluster_t *cluster =
        esp_matter::cluster::time_synchronization::create(root, &cfg, esp_matter::CLUSTER_FLAG_SERVER);
    s_time_sync_cluster_ready = (cluster != nullptr);
}

void time_sync_poll(void)
{
    if (!s_time_sync_cluster_ready || s_time_sync_ready) return;

    int64_t utc_sec = 0;
    if (get_utc_seconds(utc_sec)) {
        s_time_sync_ready = true;
        if (s_ready_cb) {
            s_ready_cb(utc_sec);
        }
    }
}

bool time_sync_has_time(void)
{
    if (!s_time_sync_cluster_ready) return false;
    int64_t utc_sec = 0;
    return get_utc_seconds(utc_sec);
}

bool time_sync_now_utc(int64_t *out_utc_sec)
{
    if (!out_utc_sec) return false;
    int64_t utc_sec = 0;
    if (!get_utc_seconds(utc_sec)) return false;
    *out_utc_sec = utc_sec;
    return true;
}

void time_sync_set_ready_callback(time_sync_ready_cb_t cb)
{
    s_ready_cb = cb;
}
