#include <stdio.h>
#include <string.h>

#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"

STATIC mp_obj_t callback;
STATIC void callback_func() {
    if (!callback)
        return;

    mp_call_function_0(callback);
}

STATIC mp_obj_t rp2_rtc_alarm_set_alarm(mp_obj_t dt, mp_obj_t cb) {
    mp_obj_t *items;
    mp_obj_get_array_fixed_n(dt, 7, &items);
    datetime_t t = {
        .year = mp_obj_get_int(items[0]),
        .month = mp_obj_get_int(items[1]),
        .day = mp_obj_get_int(items[2]),
        .dotw = mp_obj_get_int(items[3]),
        .hour = mp_obj_get_int(items[4]),
        .min = mp_obj_get_int(items[5]),
        .sec = mp_obj_get_int(items[6]),
    };
    callback = cb;
    rtc_set_alarm(&t, callback_func);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(rp2_rtc_alarm_set_alarm_obj, rp2_rtc_alarm_set_alarm);

STATIC mp_obj_t rp2_rtc_alarm_enable_alarm() {
    rtc_enable_alarm();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(rp2_rtc_alarm_enable_alarm_obj, rp2_rtc_alarm_enable_alarm);

STATIC mp_obj_t rp2_rtc_alarm_disable_alarm() {
    rtc_disable_alarm();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(rp2_rtc_alarm_disable_alarm_obj, rp2_rtc_alarm_disable_alarm);

STATIC const mp_rom_map_elem_t rp2_rtc_alarm_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_rtcalarm) },
    { MP_ROM_QSTR(MP_QSTR_set_alarm), MP_ROM_PTR(&rp2_rtc_alarm_set_alarm_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable_alarm), MP_ROM_PTR(&rp2_rtc_alarm_enable_alarm_obj) },
    { MP_ROM_QSTR(MP_QSTR_disable_alarm), MP_ROM_PTR(&rp2_rtc_alarm_disable_alarm_obj) },
};
STATIC MP_DEFINE_CONST_DICT(rp2_rtc_alarm_module_globals, rp2_rtc_alarm_module_globals_table);

// Define module object.
const mp_obj_module_t rp2_rtc_alarm_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&rp2_rtc_alarm_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_rtcalarm, rp2_rtc_alarm_user_cmodule);
