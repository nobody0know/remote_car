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
static float wheel_rpm_ratio = 65.21;
PID_TypeDef motor_pid;
chassis car_chassis[4];
extern chassis car_chassis[4];
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
    if(pdPASS == xTaskCreatePinnedToCore(chassis_task,"chassis",4096,NULL,9,&chassis_handle,1)){
        return ESP_OK;
    }
    
    return ESP_FAIL;
}

void chassis_task(void* n)
{
    while(1){
        car_chassis[0].wheel_rpm = (-vy_message-vx_message+wz_message);
        car_chassis[1].wheel_rpm= (-vy_message+vx_message+wz_message);
        car_chassis[2].wheel_rpm= (vy_message+vx_message-wz_message);
        car_chassis[3].wheel_rpm= (vy_message-vx_message-wz_message);

        for (int i = 0; i < 4; i++)
        {
            //printf("chassis:  wheel_rpm_set[%d] = %d\n",i,car_chassis[i].wheel_rpm);
            car_chassis[i].give_current = pid_cal(&motor_pid,moto_chassis[i].speed_rpm,car_chassis[i].wheel_rpm);
        }
        set_moto_current(car_chassis[0].give_current,car_chassis[1].give_current,car_chassis[2].give_current,car_chassis[3].give_current);
        vTaskDelay(5);
    }
}