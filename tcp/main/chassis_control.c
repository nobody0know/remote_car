/*
 * @Author: nobody0know 61100216+nobody0know@users.noreply.github.com
 * @Date: 2022-08-10 16:48:57
 * @LastEditors: nobody0know 61100216+nobody0know@users.noreply.github.com
 * @LastEditTime: 2022-08-10 20:44:58
 * @FilePath: \tcp\main\chassis_control.c
 * @Description: 
 * 
 * Copyright (c) 2022 by nobody0know 61100216+nobody0know@users.noreply.github.com, All Rights Reserved. 
 */
#include "chassis_control.h"
#include "can.h"
#include "pid.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "mqtt.h"

PID_TypeDef motor_pid;
chassis car_chassis[4];
esp_err_t chassis_init()
{
    ESP_ERROR_CHECK(can_init());
    pid_param_init(&motor_pid,
                    CHASSIS_PID_MAXOUT,
                    CHASSIS_PID_INTERGRAL_LIMIT,
                    CHASSIS_PID_DEADBAND,
                    CHASSIS_PID_MAX_ERR,
                    CHASSIS_PID_FIRST_TARGET,
                    CHASSIS_PID_P,
                    CHASSIS_PID_I,
                    CHASSIS_PID_D);
    static TaskHandle_t chassis_handle;
    if(pdPASS == xTaskCreate(chassis_task,"chassis",4096,NULL,FB_UPDATE_PRIO,&chassis_handle)){
        return ESP_OK;
    }
    
    return ESP_FAIL;
}

void chassis_task(void* n)
{
    while(1){
        car_chassis[0].give_current = (acc_message+bre_message)*0.8*FORWARD_RATE-tur_message*200;
        car_chassis[1].give_current = (acc_message+bre_message)*1.2*FORWARD_RATE+tur_message*200;
        car_chassis[2].give_current = (acc_message+bre_message)*1.2*FORWARD_RATE+tur_message*200;
        car_chassis[3].give_current = (acc_message+bre_message)*0.8*FORWARD_RATE-tur_message*200;
        printf("set = %d\n",(acc_message+bre_message)*FORWARD_RATE);
        set_moto_current(-car_chassis[0].give_current,car_chassis[1].give_current,car_chassis[2].give_current,-car_chassis[3].give_current);
        vTaskDelay(20);
    }
}