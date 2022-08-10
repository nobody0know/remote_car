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

esp_err_t chassis_init(chassis *car_chassis)
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
    ESP_ERROR_CHECK();
    if(pdPASS == xTaskCreate(chassis_task,"chassis",4096,NULL,FB_UPDATE_PRIO,&chassis_handle)){
        return ESP_OK;
    }
    
    return ESP_FAIL;
}

void chassis_task(void)
{
    while(1){
        if(car_chassis.gear==1){
        float car_forward = chassis.accel+chassis.brake;
        if(car_forward<=0)
        car_forward=0;
        chassis.give_current = car_forward*FORWARD_RATE;
        }
        else if(car_chassis.gear==-1){
        float car_back = chassis.accel+chassis.brake;
        if(car_back<=0)
        car_back=0;
        chassis.give_current = -car_back*BACK_RATE;
        }
        vTaskDelay(100);
    }
}