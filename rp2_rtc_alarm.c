#include <time.h>

#include "py/obj.h"
#include "py/runtime.h"

#include "pico/aon_timer.h"

static mp_obj_t callback;
static void callback_func() {
    if (!callback)
        return;

    mp_call_function_0(callback);
}

static mp_obj_t rp2_rtc_alarm_enable_alarm(mp_obj_t dt, mp_obj_t cb) {
    mp_obj_t *items;
    mp_obj_get_array_fixed_n(dt, 8, &items);
    struct tm t = {
        .tm_year = mp_obj_get_int(items[0]) - 1900,
        .tm_mon  = mp_obj_get_int(items[1]) - 1,
        .tm_mday = mp_obj_get_int(items[2]),
        .tm_wday = mp_obj_get_int(items[3]),
        .tm_hour = mp_obj_get_int(items[4]),
        .tm_min  = mp_obj_get_int(items[5]),
        .tm_sec  = mp_obj_get_int(items[6]),
    };
    callback = cb;
    aon_timer_enable_alarm_calendar(&t, callback_func, 0);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(rp2_rtc_alarm_enable_alarm_obj, rp2_rtc_alarm_enable_alarm);

static mp_obj_t rp2_rtc_alarm_disable_alarm() {
    aon_timer_disable_alarm();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(rp2_rtc_alarm_disable_alarm_obj, rp2_rtc_alarm_disable_alarm);

static const mp_rom_map_elem_t rp2_rtc_alarm_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_rtcalarm) },
    { MP_ROM_QSTR(MP_QSTR_enable_alarm), MP_ROM_PTR(&rp2_rtc_alarm_enable_alarm_obj) },
    { MP_ROM_QSTR(MP_QSTR_disable_alarm), MP_ROM_PTR(&rp2_rtc_alarm_disable_alarm_obj) },
};
static MP_DEFINE_CONST_DICT(rp2_rtc_alarm_module_globals, rp2_rtc_alarm_module_globals_table);

// Define module object.
const mp_obj_module_t rp2_rtc_alarm_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&rp2_rtc_alarm_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_rtcalarm, rp2_rtc_alarm_user_cmodule);
