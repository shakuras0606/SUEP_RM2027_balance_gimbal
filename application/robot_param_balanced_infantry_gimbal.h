/**
  * @file       robot_param_balanced_infantry_gimbal.h
  * @brief      这里是平衡步兵机器人云台部分参数配置文件，包括物理参数、PID参数等
  */

#ifndef INCLUDED_ROBOT_PARAM_H
#define INCLUDED_ROBOT_PARAM_H
#include "robot_typedef.h"

#define GIMBAL_TYPE GIMBAL_YAW_PITCH_DIRECT  // 选择云台类型
#define SHOOT_TYPE SHOOT_FRIC_TRIGGER        // 选择发射机构类型

// clang-format off
#define __SELF_BOARD_ID C_BOARD_BALANCE_GIMBAL  // 本板ID
#define __GYRO_BIAS_YAW  -0.00017f           // 陀螺仪零飘，单位rad/s0.003096855f

#define __CONTROL_LINK_RC  CL_RC_DIRECT   // 控制链路选择：RC直连
#define __CONTROL_LINK_KM  CL_KM_RC      // 控制链路选择：键鼠数据

/*******************************************************************************/
/* Gimbal                                                                      */
/*******************************************************************************/
//gimbal_init-------------------------------
#define GIMBAL_INIT_TIME (uint32_t)1000

// gimbal can
#define GIMBAL_CAN 1
#define GIMBAL_STDID 0x1FE

//mouse sensitivity ---------------------
#define MOUSE_SENSITIVITY (0.5f)

//remote controller sensitivity ---------------------
#define REMOTE_CONTROLLER_SENSITIVITY  ( 100000.0f)
#define REMOTE_CONTROLLER_MAX_DEADLINE ( 20.0f)
#define REMOTE_CONTROLLER_MIN_DEADLINE (-20.0f)
//motor parameters ---------------------
//电机id
#define GIMBAL_DIRECT_YAW_ID   ((uint8_t)3)
#define GIMBAL_DIRECT_PITCH_ID ((uint8_t)2)

//电机can口
#define GIMBAL_DIRECT_YAW_CAN   ((uint8_t)2)
#define GIMBAL_DIRECT_PITCH_CAN ((uint8_t)1)

//电机种类
#define GIMBAL_DIRECT_YAW_MOTOR_TYPE   ((MotorType_e)DJI_M6020)
#define GIMBAL_DIRECT_PITCH_MOTOR_TYPE ((MotorType_e)DJI_M6020)

//旋转方向
#define GIMBAL_DIRECT_YAW_DIRECTION   ( -1)
#define GIMBAL_DIRECT_PITCH_DIRECTION ( -1)

//减速比
#define GIMBAL_DIRECT_YAW_REDUCTION_RATIO   (1)
#define GIMBAL_DIRECT_PITCH_REDUCTION_RATIO (1)

//电机运行模式
#define GIMBAL_DIRECT_YAW_MODE   (DJI_CURRENT_MODE)
#define GIMBAL_DIRECT_PITCH_MODE (DJI_CURRENT_MODE)

//physical parameters ---------------------
#define GIMBAL_UPPER_LIMIT_PITCH ( 0.60f)
#define GIMBAL_LOWER_LIMIT_PITCH (-0.40f)

//电机角度中值设置
#define GIMBAL_DIRECT_PITCH_MID (-2.08f)  //云台初始化正对齐的时候使用的pitch轴正中心量
#define GIMBAL_DIRECT_YAW_MID   (-1.05f)     //云台初始化正对齐的时候使用的yaw轴正中心量

//PID parameters ---------------------
//YAW ANGLE
#define KP_GIMBAL_YAW_ANGLE       (25.0f)//28
#define KI_GIMBAL_YAW_ANGLE       (0.0f)
#define KD_GIMBAL_YAW_ANGLE       (10.0f)
#define MAX_IOUT_GIMBAL_YAW_ANGLE (0.0f)
#define MAX_OUT_GIMBAL_YAW_ANGLE  (20.0f)
//VELOCITY:角速度
#define KP_GIMBAL_YAW_VELOCITY       (3000.0f)//5000
#define KI_GIMBAL_YAW_VELOCITY       (10.0f)
#define KD_GIMBAL_YAW_VELOCITY       (0.0f)
#define MAX_IOUT_GIMBAL_YAW_VELOCITY (2000.0f)
#define MAX_OUT_GIMBAL_YAW_VELOCITY  (25000.0f)

//PITCH ANGLE
#define KP_GIMBAL_PITCH_ANGLE       (27.0f)//30
#define KI_GIMBAL_PITCH_ANGLE       (0.0f)
#define KD_GIMBAL_PITCH_ANGLE       (10.0f)
#define MAX_IOUT_GIMBAL_PITCH_ANGLE (0.0f)
#define MAX_OUT_GIMBAL_PITCH_ANGLE  (20.0f)
//VELOCITY:角速度
#define KP_GIMBAL_PITCH_VELOCITY       (3000.0f)
#define KI_GIMBAL_PITCH_VELOCITY       (15.0f)
#define KD_GIMBAL_PITCH_VELOCITY       (0.0f)
#define MAX_IOUT_GIMBAL_PITCH_VELOCITY (3000.0f)
#define MAX_OUT_GIMBAL_PITCH_VELOCITY  (25000.0f)


/*******************************************************************************/
/* Shoot                                                                       */
/*******************************************************************************/
//physical parameters ---------------------
#define FRIC_RADIUS 0.03f  // (m)摩擦轮半径
#define BULLET_NUM 10       // 定义拨弹盘容纳弹丸个数
#define GUN_NUM 1          // 定义枪管个数（一个枪管2个摩擦轮）
#define TRIGGER_REDUCTION_RATIO 1.0f   // 拨弹盘额外齿轮减速比(不含电机内部减速比)
  
/*MOTOR paramters --------------------*/

//电机种类
// MG_4005 = 瓴控 MG4005-i10（10:1 行星减速，CAN 0x140+ID 单电机命令族）
// 若换回大疆 M2006，需同时恢复文件末尾注释中的 M2006 参数
#define TRIGGER_MOTOR_TYPE ((MotorType_e)MG_4005)
#define FRIC_MOTOR_TYPE ((MotorType_e)DJI_M3508)

//拨弹盘电机内部减速比(瓴控 MG4005-i10 为 10:1，电机轴:输出轴)
#define TRIGGER_MOTOR_REDUCTION_RATIO (10.0f)

//拨弹盘电机安装方向(1/-1，取使拨弹盘正转方向对应角度增加)
#define TRIGGER_MOTOR_DIRECTION (1)

//电机ID
#define TRIGGER_MOTOR_ID 2
#define FRIC_MOTOR_R_ID 3
#define FRIC_MOTOR_L_ID 6

//电机can口
#define TRIGGER_MOTOR_CAN 2
#define FRIC_MOTOR_R_CAN 1
#define FRIC_MOTOR_L_CAN 1

//电机std_id
#define STD_ID 0x200

//单环拨弹速度(rad/s，电机轴；MG4005-i10 电机侧最高约 335rad/s)
#define TRIGGER_SPEED               (200.0f)
//摩擦轮速度
#define FRIC_R_SPEED                  (666.0f) 
#define FRIC_L_SPEED                  (-666.0f) 
#define FRIC_SPEED_LIMIT            (600.0f) 

//电机反馈码盘值范围
#define HALF_ECD_RANGE              4096
#define ECD_RANGE                   8191

//电机rpm 变化成 旋转速度的比例
#define MOTOR_RPM_TO_SPEED          0.00290888208665721596153948461415f
#define MOTOR_ECD_TO_ANGLE          0.000021305288720633905968306772076277f
#define FULL_COUNT                  18

/*BLOCK&REVERSE parameters------------*/

//初版   看门狗防堵转(速度单位: rad/s，电机轴)
#define BLOCK_TRIGGER_SPEED         20.0f
#define BLOCK_TIME                  1000
#define REVERSE_TIME                1250
#define REVERSE_SPEED               (-100.0f)  

/*MIT parameters ---------------------*/

#define TRIGGER_SPEED_MIT_KD (0.0f)  //  (none)

/*PID parameters ---------------------*/
// 注意: 以下拨弹轮 PID 按当前 TRIGGER_MOTOR_TYPE = MG_4005 整定
// (速度环输出为转矩电流 A，角度环输出为电机轴转速 rad/s)，需实车继续整定。
// 若切回 DJI_M2006，恢复其原始参数:
//   TRIGGER_SPEED = 350, BLOCK_TRIGGER_SPEED = 5, REVERSE_SPEED = -20
//   SPEED_PID: KP=100  KI=0.5 KD=0.1 MAX_OUT=10000 MAX_IOUT=1000
//   ANGEL_PID: KP=20   KI=0.5 KD=0.0 MAX_OUT=10000 MAX_IOUT=1000

//拨弹轮电机PID速度环：输入电机轴转速(rad/s)，输出转矩电流(A)
#define TRIGGER_SPEED_PID_KP (0.1f)
#define TRIGGER_SPEED_PID_KI (0.0005f)
#define TRIGGER_SPEED_PID_KD (0.0001f)

#define TRIGGER_SPEED_PID_MAX_OUT (5.0f)   // (A)软件限流，MG4005-i10 满量程 ±33A
#define TRIGGER_SPEED_PID_MAX_IOUT (1.0f)  // (A)

// 单发模式 拨弹轮电机PID角度环：输入电机轴角度误差(rad)，输出电机轴转速期望(rad/s)
#define TRIGGER_ANGEL_PID_KP (20.0f)
#define TRIGGER_ANGEL_PID_KI (0.5f)
#define TRIGGER_ANGEL_PID_KD (0.0f)

#define TRIGGER_ANGEL_PID_MAX_OUT (350.0f)  // (rad/s)电机轴转速上限
#define TRIGGER_ANGEL_PID_MAX_IOUT (50.0f)  // (rad/s)

//摩擦轮电机PID
#define FRIC_SPEED_PID_KP (200.0f)
#define FIRC_SPEED_PID_KI (1.0f)
#define FRIC_SPEED_PID_KD (1.0f)

#define FRIC_PID_MAX_OUT (16000.0f)
#define FRIC_PID_MAX_IOUT (1000.0f)

#define SHOOT_HEAT_REMAIN_VALUE     80//89

// clang-format on
#endif /* INCLUDED_ROBOT_PARAM_H */
/*------------------------------ End of File ------------------------------*/
