/*
 * @Author: nobody0know 61100216+nobody0know@users.noreply.github.com
 * @Date: 2022-08-10 16:12:47
 * @LastEditors: nobody0know 61100216+nobody0know@users.noreply.github.com
 * @LastEditTime: 2022-08-10 20:24:31
 * @FilePath: \tcp\main\pid.h
 * @Description: 
 * 
 * Copyright (c) 2022 by nobody0know 61100216+nobody0know@users.noreply.github.com, All Rights Reserved. 
 */
#ifndef _PID_H_
#define _PID_H_

#include "stdint.h"

typedef struct _PID_TypeDef
{
	
	float target;							//目标值====
	float lastNoneZeroTarget;
	float kp;
	float ki;
	float kd;
	
	float   measure;			//测量值
	float   err;				//误差
	float   last_err;      		//上次误差
	
	float pout;
	float iout;
	float dout;
	
	float output;				//本次输出====
	//float last_output;			//上次输出
	
	float MaxOutput;			//输出限幅=====
	float IntegralLimit;		//积分限幅=====
	float DeadBand;			    //死区（绝对值）=====
	//float ControlPeriod;		//控制周期====
	float Max_Err;				//最大误差====
	
	uint32_t thistime;
	uint32_t lasttime;
	uint8_t dtime;	
	
}PID_TypeDef;



extern void pid_param_init(PID_TypeDef * pid, 
	uint16_t maxout,
	uint16_t intergral_limit,
	float deadband,
	int16_t  max_err,
	int16_t  target,
	float 	kp, 
	float 	ki, 
	float 	kd
    );



void pid_reset(PID_TypeDef * pid, float kp, float ki, float kd);

void pid_target(PID_TypeDef * pid, uint16_t new_target);

extern float pid_cal(PID_TypeDef* pid, int16_t measure, int16_t target);

















#endif