/**
  ****************************(C) COPYRIGHT 2024 Polarbear****************************
  * @file       can_cmd_lingkong.c/h
  * @brief      CAN发送函数，通过CAN信号控制瓴控电机 MF9025 / MG4005-i10.
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     May-16-2024     Penguin         1. 完成。
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2024 Polarbear****************************
  */
#include "CAN_cmd_lingkong.h"

#include "bsp_can.h"
#include "stm32f4xx_hal.h"
#include "user_lib.h"

#define STDID_OFFESET ((uint16_t)0x140)

#define TORQUE_COEFFICIENT 0.32f                  // (N*m/A)转矩系数(MF9025)
#define CURRENT_TO_MULTICONTROL 62.5f             // (2000/32)(1/A)电流转换为控制量
#define CURRENT_TO_MF_CONTROL 124.1212121212121f  // (2048/16.5)(1/A)电流转换为控制量
#define CURRENT_TO_MG_CONTROL 62.0606060606061f   // (2048/33)(1/A)电流转换为控制量(MG系列)
#define LK_SPEED_CONTROL_LIMIT 600000.0f          // (0.01dps)速度指令上限(协议 Max Speed 量程)

static CanCtrlData_s CAN_CTRL_DATA = {
    .tx_header.IDE = CAN_ID_STD,
    .tx_header.RTR = CAN_RTR_DATA,
    .tx_header.DLC = 8,
};

/*-------------------- Private functions --------------------*/

/**
 * @brief        将电流(A)转换为转矩电流控制量(-2048~2048)
 * @param[in]    p_motor 电机结构体
 * @param[in]    current 电机侧转矩电流(A)
 * @return       转矩电流控制量
 * @note         MF 系列 2048 对应 16.5A，MG 系列 2048 对应 33A
 */
static int16_t LkCurrentToControl(const Motor_s * p_motor, float current)
{
    float max_current;
    float min_current;
    float current_to_control;
    int16_t max_iq;
    int16_t min_iq;

    if (p_motor->type == MG_4005) {
        max_current = MG_MAX_CONTROL_CURRENT;
        min_current = MG_MIN_CONTROL_CURRENT;
        current_to_control = CURRENT_TO_MG_CONTROL;
        max_iq = MG_MAX_CONTROL_IQ;
        min_iq = MG_MIN_CONTROL_IQ;
    } else {
        max_current = LK_MAX_MF_CONTROL_CURRENT;
        min_current = LK_MIN_MF_CONTROL_CURRENT;
        current_to_control = CURRENT_TO_MF_CONTROL;
        max_iq = LK_MAX_MF_CONTROL_IQ;
        min_iq = LK_MIN_MF_CONTROL_IQ;
    }

    return int16_constrain(
        (int16_t)(fp32_constrain(current, min_current, max_current) * current_to_control), min_iq,
        max_iq);
}

/**
 * @brief        获取电机转矩系数(N*m/A)
 * @param[in]    p_motor 电机结构体
 * @return       转矩系数
 */
static float LkGetTorqueCoefficient(const Motor_s * p_motor)
{
    return (p_motor->type == MG_4005) ? MG4005_TORQUE_COEFFICIENT : TORQUE_COEFFICIENT;
}

/**
 * @brief        获取电机转矩限幅(N*m)
 * @param[in]    p_motor     电机结构体
 * @param[out]   min_torque  最小转矩
 * @param[out]   max_torque  最大转矩
 */
static void LkGetTorqueLimit(
    const Motor_s * p_motor, float * min_torque, float * max_torque)
{
    if (p_motor->type == MG_4005) {
        *min_torque = MG4005_MIN_TORQUE;
        *max_torque = MG4005_MAX_TORQUE;
    } else {
        *min_torque = LK_MIN_MF_TORQUE;
        *max_torque = LK_MAX_MF_TORQUE;
    }
}

/**
 * @brief        电机失能
 * @param[in]    hcan     指向CAN_HandleTypeDef结构的指针
 * @param[in]    motor_id 电机ID，指定目标电机
 */
static void DisableMotor(hcan_t * hcan, uint16_t motor_id)
{
    CAN_CTRL_DATA.hcan = hcan;

    CAN_CTRL_DATA.tx_header.StdId = motor_id + STDID_OFFESET;

    CAN_CTRL_DATA.tx_data[0] = 0x80;
    CAN_CTRL_DATA.tx_data[1] = 0x00;
    CAN_CTRL_DATA.tx_data[2] = 0x00;
    CAN_CTRL_DATA.tx_data[3] = 0x00;
    CAN_CTRL_DATA.tx_data[4] = 0x00;
    CAN_CTRL_DATA.tx_data[5] = 0x00;
    CAN_CTRL_DATA.tx_data[6] = 0x00;
    CAN_CTRL_DATA.tx_data[7] = 0x00;

    CAN_SendTxMessage(&CAN_CTRL_DATA);
}

/**
 * @brief        停止电机
 * @param[in]    hcan     指向CAN_HandleTypeDef结构的指针
 * @param[in]    motor_id 电机ID，指定目标电机
 */
static void StopMotor(hcan_t * hcan, uint16_t motor_id)
{
    CAN_CTRL_DATA.hcan = hcan;

    CAN_CTRL_DATA.tx_header.StdId = motor_id + STDID_OFFESET;

    CAN_CTRL_DATA.tx_data[0] = 0x81;
    CAN_CTRL_DATA.tx_data[1] = 0x00;
    CAN_CTRL_DATA.tx_data[2] = 0x00;
    CAN_CTRL_DATA.tx_data[3] = 0x00;
    CAN_CTRL_DATA.tx_data[4] = 0x00;
    CAN_CTRL_DATA.tx_data[5] = 0x00;
    CAN_CTRL_DATA.tx_data[6] = 0x00;
    CAN_CTRL_DATA.tx_data[7] = 0x00;

    CAN_SendTxMessage(&CAN_CTRL_DATA);
}

/**
 * @brief        电机使能
 * @param[in]    hcan     指向CAN_HandleTypeDef结构的指针
 * @param[in]    motor_id 电机ID，指定目标电机
 */
static void EnableMotor(hcan_t * hcan, uint16_t motor_id)
{
    CAN_CTRL_DATA.hcan = hcan;

    CAN_CTRL_DATA.tx_header.StdId = motor_id + STDID_OFFESET;

    CAN_CTRL_DATA.tx_data[0] = 0x88;
    CAN_CTRL_DATA.tx_data[1] = 0x00;
    CAN_CTRL_DATA.tx_data[2] = 0x00;
    CAN_CTRL_DATA.tx_data[3] = 0x00;
    CAN_CTRL_DATA.tx_data[4] = 0x00;
    CAN_CTRL_DATA.tx_data[5] = 0x00;
    CAN_CTRL_DATA.tx_data[6] = 0x00;
    CAN_CTRL_DATA.tx_data[7] = 0x00;

    CAN_SendTxMessage(&CAN_CTRL_DATA);
}

/**
 * @brief        单电机转矩闭环控制命令
 * @param[in]    hcan      指向CAN_HandleTypeDef结构的指针
 * @param[in]    motor_id  电机ID，指定目标电机
 * @param[in]    iqControl 转矩电流 -2048~2048
 */
static void SingleTorqueControl(hcan_t * hcan, uint16_t motor_id, int16_t iqControl)
{
    CAN_CTRL_DATA.hcan = hcan;

    CAN_CTRL_DATA.tx_header.StdId = motor_id + STDID_OFFESET;

    CAN_CTRL_DATA.tx_data[0] = 0xA1;
    CAN_CTRL_DATA.tx_data[1] = 0x00;
    CAN_CTRL_DATA.tx_data[2] = 0x00;
    CAN_CTRL_DATA.tx_data[3] = 0x00;
    CAN_CTRL_DATA.tx_data[4] = *(uint8_t *)(&iqControl);
    CAN_CTRL_DATA.tx_data[5] = *((uint8_t *)(&iqControl) + 1);
    CAN_CTRL_DATA.tx_data[6] = 0x00;
    CAN_CTRL_DATA.tx_data[7] = 0x00;

    CAN_SendTxMessage(&CAN_CTRL_DATA);
}

/**
 * @brief        单电机速度闭环控制命令
 * @param[in]    hcan      指向CAN_HandleTypeDef结构的指针
 * @param[in]    motor_id  电机ID，指定目标电机
 * @param[in]    speedControl 速度控制值(0.01dps/LSB)
 * @param[in]    iqControlLimit 速度环内部转矩电流限幅(-2048~2048)
 */
static void SingleSpeedControl(
    hcan_t * hcan, uint16_t motor_id, int32_t speedControl, int16_t iqControlLimit)
{
    CAN_CTRL_DATA.hcan = hcan;

    CAN_CTRL_DATA.tx_header.StdId = motor_id + STDID_OFFESET;

    CAN_CTRL_DATA.tx_data[0] = 0xA2;
    CAN_CTRL_DATA.tx_data[1] = 0x00;
    // DATA[2..3] 为速度环转矩电流限幅(与 0xA1 的 iq 同量纲)，必须填写
    CAN_CTRL_DATA.tx_data[2] = *(uint8_t *)(&iqControlLimit);
    CAN_CTRL_DATA.tx_data[3] = *((uint8_t *)(&iqControlLimit) + 1);
    CAN_CTRL_DATA.tx_data[4] = *(uint8_t *)(&speedControl);
    CAN_CTRL_DATA.tx_data[5] = *((uint8_t *)(&speedControl) + 1);
    CAN_CTRL_DATA.tx_data[6] = *((uint8_t *)(&speedControl) + 2);
    CAN_CTRL_DATA.tx_data[7] = *((uint8_t *)(&speedControl) + 3);

    CAN_SendTxMessage(&CAN_CTRL_DATA);
}

/**
 * @brief        多电机转矩闭环控制命令
 * @param[in]    hcan        指向CAN_HandleTypeDef结构的指针
 * @param[in]    iqControl_1 转矩电流 -2000\\~2000
 * @param[in]    iqControl_2 转矩电流 -2000\\~2000
 * @param[in]    iqControl_3 转矩电流 -2000\\~2000
 * @param[in]    iqControl_4 转矩电流 -2000\\~2000
 */
static void MultipleTorqueControl(
    hcan_t * hcan, int16_t iqControl_1, int16_t iqControl_2, int16_t iqControl_3,
    int16_t iqControl_4)
{
    CAN_CTRL_DATA.hcan = hcan;

    CAN_CTRL_DATA.tx_header.StdId = 0x280;

    CAN_CTRL_DATA.tx_data[0] = *(uint8_t *)(&iqControl_1);
    CAN_CTRL_DATA.tx_data[1] = *((uint8_t *)(&iqControl_1) + 1);
    CAN_CTRL_DATA.tx_data[2] = *(uint8_t *)(&iqControl_2);
    CAN_CTRL_DATA.tx_data[3] = *((uint8_t *)(&iqControl_2) + 1);
    CAN_CTRL_DATA.tx_data[4] = *(uint8_t *)(&iqControl_3);
    CAN_CTRL_DATA.tx_data[5] = *((uint8_t *)(&iqControl_3) + 1);
    CAN_CTRL_DATA.tx_data[6] = *(uint8_t *)(&iqControl_4);
    CAN_CTRL_DATA.tx_data[7] = *((uint8_t *)(&iqControl_4) + 1);

    CAN_SendTxMessage(&CAN_CTRL_DATA);
}

/*-------------------- Check functions --------------------*/

/**
 * @brief      获取can总线句柄
 * @param[in]  motor 电机结构体
 * @return     can总线句柄
 * @note       获取电机结构体中的can号，返回对应的can总线句柄，同时检测电机类型是否为瓴控电机
 */
static hcan_t * GetHcanPoint(Motor_s * motor)
{
    if (motor->type != MF_9025 && motor->type != MG_4005) return NULL;

    if (motor->can == 1)
        return &hcan1;
    else if (motor->can == 2)
        return &hcan2;

    return NULL;
}

/*-------------------- User functions --------------------*/

void LkDisable(Motor_s * p_motor)
{
    hcan_t * hcan = GetHcanPoint(p_motor);
    if (hcan == NULL) return;

    DisableMotor(hcan, p_motor->id);
}

void LkStop(Motor_s * p_motor)
{
    hcan_t * hcan = GetHcanPoint(p_motor);
    if (hcan == NULL) return;

    StopMotor(hcan, p_motor->id);
}

void LkEnable(Motor_s * p_motor)
{
    hcan_t * hcan = GetHcanPoint(p_motor);
    if (hcan == NULL) return;

    EnableMotor(hcan, p_motor->id);
}

void LkSingleTorqueControl(Motor_s * p_motor)
{
    hcan_t * hcan = GetHcanPoint(p_motor);
    if (hcan == NULL) return;

    float min_torque;
    float max_torque;
    LkGetTorqueLimit(p_motor, &min_torque, &max_torque);

    float torque = fp32_constrain(p_motor->set.tor, min_torque, max_torque);
    float current = torque / LkGetTorqueCoefficient(p_motor);

    SingleTorqueControl(hcan, p_motor->id, LkCurrentToControl(p_motor, current));
}

/**
 * @brief        单电机转矩电流闭环控制命令(0xA1)，控制量取自 p_motor->set.curr(单位 A)
 * @param[in]    p_motor 电机结构体
 * @note         0xA1 不受上位机 Max Torque Current 限制，电流在驱动内按电机系列限幅
 */
void LkSingleCurrentControl(Motor_s * p_motor)
{
    hcan_t * hcan = GetHcanPoint(p_motor);
    if (hcan == NULL) return;

    SingleTorqueControl(hcan, p_motor->id, LkCurrentToControl(p_motor, p_motor->set.curr));
}

void LkSingleSpeedControl(Motor_s * p_motor)
{
    hcan_t * hcan = GetHcanPoint(p_motor);
    if (hcan == NULL) return;

    // (rad/s) -> (0.01dps)，实际转速还会被电机内部 Max Speed 参数限幅
    int32_t speed_control = (int32_t)fp32_constrain(
        p_motor->set.vel * RAD_TO_DEGREE * 100.0f, -LK_SPEED_CONTROL_LIMIT, LK_SPEED_CONTROL_LIMIT);

    // 速度环内部转矩电流限幅取电机系列允许的最大值
    int16_t iq_limit =
        (p_motor->type == MG_4005) ? (int16_t)MG_MAX_CONTROL_IQ : (int16_t)LK_MAX_MF_CONTROL_IQ;

    SingleSpeedControl(hcan, p_motor->id, speed_control, iq_limit);
}

void LkMultipleTorqueControl(
    uint8_t can, float torque_1, float torque_2, float torque_3, float torque_4)
{
    hcan_t * hcan = NULL;
    if (can == 1)
        hcan = &hcan1;
    else if (can == 2)
        hcan = &hcan2;

    if (hcan == NULL) return;

    int16_t iqControl[4];
    iqControl[0] = int16_constrain(
        (torque_1 / TORQUE_COEFFICIENT) * CURRENT_TO_MULTICONTROL, LK_MIN_MULTICONTROL_IQ,
        LK_MAX_MULTICONTROL_IQ);
    iqControl[1] = int16_constrain(
        (torque_2 / TORQUE_COEFFICIENT) * CURRENT_TO_MULTICONTROL, LK_MIN_MULTICONTROL_IQ,
        LK_MAX_MULTICONTROL_IQ);
    iqControl[2] = int16_constrain(
        (torque_3 / TORQUE_COEFFICIENT) * CURRENT_TO_MULTICONTROL, LK_MIN_MULTICONTROL_IQ,
        LK_MAX_MULTICONTROL_IQ);
    iqControl[3] = int16_constrain(
        (torque_4 / TORQUE_COEFFICIENT) * CURRENT_TO_MULTICONTROL, LK_MIN_MULTICONTROL_IQ,
        LK_MAX_MULTICONTROL_IQ);

    MultipleTorqueControl(hcan, iqControl[0], iqControl[1], iqControl[2], iqControl[3]);
}

void LkMultipleIqControl(
    uint8_t can, int16_t iqControl_1, int16_t iqControl_2, int16_t iqControl_3, int16_t iqControl_4)
{
    hcan_t * hcan = NULL;
    if (can == 1)
        hcan = &hcan1;
    else if (can == 2)
        hcan = &hcan2;

    if (hcan == NULL) return;

    int16_t current[4];
    current[0] = int16_constrain(iqControl_1, -2000, 2000);
    current[1] = int16_constrain(iqControl_2, -2000, 2000);
    current[2] = int16_constrain(iqControl_3, -2000, 2000);
    current[3] = int16_constrain(iqControl_4, -2000, 2000);

    MultipleTorqueControl(hcan, current[0], current[1], current[2], current[3]);
}
/************************ END OF FILE ************************/
