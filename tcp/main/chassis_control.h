/*
 * @Author: nobody0know 61100216+nobody0know@users.noreply.github.com
 * @Date: 2022-08-10 16:49:34
 * @LastEditors: nobody0know 61100216+nobody0know@users.noreply.github.com
 * @LastEditTime: 2022-08-10 20:32:40
 * @FilePath: \tcp\main\chassis_control.h
 * @Description: 
 * 
 * Copyright (c) 2022 by nobody0know 61100216+nobody0know@users.noreply.github.com, All Rights Reserved. 
 */
#include "stdint.h"
#include "esp_err.h"
#define CAR_CONTROL_MODE 1//1为油门刹车的开环模式，0为pid控制的闭环模式

#define CHASSIS_PID_P 10.0f
#define CHASSIS_PID_I 0.5f
#define CHASSIS_PID_D 0.5f
#define CHASSIS_PID_MAXOUT 16000
#define CHASSIS_PID_INTERGRAL_LIMIT 1000
#define CHASSIS_PID_DEADBAND 10
#define CHASSIS_PID_MAX_ERR 800
#define CHASSIS_PID_FIRST_TARGET 0
#define ACCELERATE_RATE 2000
#define BRAKE_RATE 100
#define FORWARD_RATE 300
#define BACK_RATE 300
typedef struct
{
    float vx_set;//前进速度，pid用
    float accel;//油门
    float brake;//刹车
    float vx;
    int vy_set;
    int vy;
    int yaw_set;
    int pitch_set;
    int16_t give_current;
    int16_t wheel_rpm;
    int16_t wheel_rpm_get;
}chassis;


extern chassis car_chassis[4];
void chassis_task(void* n);
esp_err_t chassis_init();