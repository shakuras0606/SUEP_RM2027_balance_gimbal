cmake_minimum_required(VERSION 3.20)

# Keil target "standard_robot" (MDK-ARM/standard_robot.uvprojx) converted to
# Ninja + arm-none-eabi-gcc.  Source membership, includes and defines follow the
# Keil target; compiler-specific files (GNU startup, GCC FreeRTOS port, newlib
# syscalls) replace their Arm Compiler counterparts, which remain untouched.

project(standard_robot LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# The Keil target controls optimization explicitly (target Level 0, HAL/FreeRTOS/
# USB groups Level 1) and never defines NDEBUG, so CMake build-type defaults must
# not inject -O3/-DNDEBUG on top of the translated flags.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE None CACHE STRING "Build type" FORCE)
endif()
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_C_FLAGS_MINSIZEREL "")
set(CMAKE_ASM_FLAGS_DEBUG "")
set(CMAKE_ASM_FLAGS_RELEASE "")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "")
set(CMAKE_ASM_FLAGS_MINSIZEREL "")

set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SRC_DIR "${PROJECT_DIR}/Src")
set(APP_DIR "${PROJECT_DIR}/application")
set(BSP_DIR "${PROJECT_DIR}/bsp/boards")
set(COMPONENTS_DIR "${PROJECT_DIR}/components")
set(DRIVERS_DIR "${PROJECT_DIR}/Drivers")
set(MIDDLEWARES_DIR "${PROJECT_DIR}/Middlewares")
set(CMAKE_DIR "${PROJECT_DIR}/cmake")

# Cortex-M4F: device is STM32F407IGHx with single-precision FPU (Keil: FPU2,
# "Use FPU").  hard-float matches the Tag_ABI_VFP_args of the prebuilt libraries.
set(TARGET_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Keil optimization intent: target = Level 0 (-O0), HAL/FreeRTOS/USB groups =
# Level 1 (-O1).  Keil debug information is enabled.
set(TARGET_OPT_FLAGS -O0 -g3)
set(GROUP_OPT_FLAGS -O1 -g3)

# --- GNU toolchain support files (replace Arm Compiler counterparts) ---------
set(GCC_SUPPORT_SOURCES
    ${CMAKE_DIR}/startup_stm32f407xx.s
    ${CMAKE_DIR}/syscalls.c
)

# --- Group "Application/User" ------------------------------------------------
set(APP_USER_SOURCES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/gpio.c
    ${SRC_DIR}/freertos.c
    ${SRC_DIR}/adc.c
    ${SRC_DIR}/can.c
    ${SRC_DIR}/crc.c
    ${SRC_DIR}/dma.c
    ${SRC_DIR}/i2c.c
    ${SRC_DIR}/rng.c
    ${SRC_DIR}/rtc.c
    ${SRC_DIR}/spi.c
    ${SRC_DIR}/tim.c
    ${SRC_DIR}/usart.c
    ${SRC_DIR}/usb_device.c
    ${SRC_DIR}/usbd_conf.c
    ${SRC_DIR}/usbd_desc.c
    ${SRC_DIR}/usbd_cdc_if.c
    ${SRC_DIR}/stm32f4xx_it.c
    ${SRC_DIR}/stm32f4xx_hal_msp.c
)

# --- Group "Drivers/CMSIS" ---------------------------------------------------
set(CMSIS_SOURCES
    ${SRC_DIR}/system_stm32f4xx.c
)

# --- Group "Drivers/STM32F4xx_HAL_Driver" ------------------------------------
set(HAL_SOURCES
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
)

# --- Group "Middlewares/FreeRTOS" -------------------------------------------
# portable/RVDS/ARM_CM4F (armasm) is replaced by the official FreeRTOS V10.0.1
# GCC port at portable/GCC/ARM_CM4F, which links no Arm Compiler objects.
set(FREERTOS_SOURCES
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/croutine.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/event_groups.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/list.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/queue.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/stream_buffer.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/tasks.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/timers.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

# --- Group "Middlewares/USB_Device_Library" ---------------------------------
set(USB_SOURCES
    ${MIDDLEWARES_DIR}/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
    ${MIDDLEWARES_DIR}/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    ${MIDDLEWARES_DIR}/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    ${MIDDLEWARES_DIR}/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
)

# --- Group "boards" ----------------------------------------------------------
set(BSP_SOURCES
    ${BSP_DIR}/bsp_adc.c
    ${BSP_DIR}/bsp_buzzer.c
    ${BSP_DIR}/bsp_can.c
    ${BSP_DIR}/bsp_crc32.c
    ${BSP_DIR}/bsp_delay.c
    ${BSP_DIR}/bsp_flash.c
    ${BSP_DIR}/bsp_fric.c
    ${BSP_DIR}/bsp_imu_pwm.c
    ${BSP_DIR}/bsp_i2c.c
    ${BSP_DIR}/bsp_laser.c
    ${BSP_DIR}/bsp_led.c
    ${BSP_DIR}/bsp_rc.c
    ${BSP_DIR}/bsp_rng.c
    ${BSP_DIR}/bsp_spi.c
    ${BSP_DIR}/bsp_usart.c
    ${BSP_DIR}/bsp_uart.c
    ${BSP_DIR}/bsp_pwm.c
)

# --- Group "devices" ---------------------------------------------------------
set(DEVICE_SOURCES
    ${COMPONENTS_DIR}/devices/OLED.c
    ${COMPONENTS_DIR}/devices/BMI088driver.c
    ${COMPONENTS_DIR}/devices/BMI088Middleware.c
    ${COMPONENTS_DIR}/devices/ist8310driver.c
    ${COMPONENTS_DIR}/devices/ist8310driver_middleware.c
)

# --- Group "algorithm" -------------------------------------------------------
# AHRS.lib is linked as a prebuilt Arm Compiler library (see PREBUILT_LIBS
# below).  arm_cortexM4lf_math.lib cannot be linked by GNU ld (its per-section
# ABI attributes conflict, "conflicting CPU architectures"), so the CMSIS-DSP
# functions that the firmware references are rebuilt from the matching
# CMSIS_4 DSP v1.4.5 sources in components/algorithm/cmsis_dsp.
set(ALGORITHM_SOURCES
    ${COMPONENTS_DIR}/algorithm/AHRS_middleware.c
    ${COMPONENTS_DIR}/algorithm/user_lib.c
)

set(CMSIS_DSP_SOURCES
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_common_tables.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_sin_f32.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_cos_f32.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_mat_init_f32.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_mat_add_f32.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_mat_sub_f32.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_mat_mult_f32.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_mat_trans_f32.c
    ${COMPONENTS_DIR}/algorithm/cmsis_dsp/arm_mat_inverse_f32.c
)

# --- Group "support" ---------------------------------------------------------
set(SUPPORT_SOURCES
    ${COMPONENTS_DIR}/support/CRC8_CRC16.c
    ${COMPONENTS_DIR}/support/fifo.c
    ${COMPONENTS_DIR}/support/mem_mang4.c
    ${COMPONENTS_DIR}/support/kalman_filter.c
    ${COMPONENTS_DIR}/support/signal_generator.c
    ${COMPONENTS_DIR}/support/clist.c
)

# --- Group "controller" ------------------------------------------------------
set(CONTROLLER_SOURCES
    ${COMPONENTS_DIR}/controller/pid.c
)

# --- Group "assist" ----------------------------------------------------------
set(ASSIST_SOURCES
    ${APP_DIR}/assist/detect_task.c
    ${APP_DIR}/assist/develop_task.c
    ${APP_DIR}/assist/data_exchange.c
)

# --- Group "chassis" ---------------------------------------------------------
set(CHASSIS_SOURCES
    ${APP_DIR}/chassis/chassis.c
    ${APP_DIR}/chassis/chassis_power_control.c
    ${APP_DIR}/chassis/chassis_task.c
    ${APP_DIR}/chassis/chassis_balance.c
    ${APP_DIR}/chassis/chassis_balance_extras.c
    ${APP_DIR}/chassis/chassis_mecanum.c
    ${APP_DIR}/chassis/chassis_omni.c
    ${APP_DIR}/chassis/chassis_steering.c
)

# --- Group "communication" ---------------------------------------------------
set(COMMUNICATION_SOURCES
    ${APP_DIR}/communication/communication_task.c
    ${APP_DIR}/communication/communication.c
    ${APP_DIR}/communication/usb_task.c
)

# --- Group "custom_controller" ----------------------------------------------
set(CUSTOM_CONTROLLER_SOURCES
    ${APP_DIR}/custom_controller/custom_controller.c
    ${APP_DIR}/custom_controller/custom_controller_task.c
    ${APP_DIR}/custom_controller/custom_controller_penguin_mini.c
    ${APP_DIR}/custom_controller/custom_controller_engineer.c
    ${APP_DIR}/custom_controller/custom_controller_connect.c
)

# --- Group "gimbal" ----------------------------------------------------------
set(GIMBAL_SOURCES
    ${APP_DIR}/gimbal/gimbal.c
    ${APP_DIR}/gimbal/gimbal_task.c
    ${APP_DIR}/gimbal/gimbal_yaw_pitch_direct.c
    ${APP_DIR}/gimbal/gimbal_virtual.c
)

# --- Group "IMU" -------------------------------------------------------------
set(IMU_SOURCES
    ${APP_DIR}/IMU/IMU_task.c
    ${APP_DIR}/IMU/IMU_solve.c
)

# --- Group "mechanical_arm" --------------------------------------------------
set(MECHANICAL_ARM_SOURCES
    ${APP_DIR}/mechanical_arm/mechanical_arm.c
    ${APP_DIR}/mechanical_arm/mechanical_arm_task.c
    ${APP_DIR}/mechanical_arm/mechanical_arm_penguin_mini.c
    ${APP_DIR}/mechanical_arm/mechanical_arm_engineer.c
)

# --- Group "music" -----------------------------------------------------------
set(MUSIC_SOURCES
    ${APP_DIR}/music/music_task.c
    ${APP_DIR}/music/music.c
    ${APP_DIR}/music/music_you.c
    ${APP_DIR}/music/music_unity.c
    ${APP_DIR}/music/music_canon.c
    ${APP_DIR}/music/music_castle_in_the_sky.c
    ${APP_DIR}/music/music_start.c
    ${APP_DIR}/music/music_referee.c
    ${APP_DIR}/music/music_error.c
    ${APP_DIR}/music/music_see_you_again.c
    ${APP_DIR}/music/music_hao_yun_lai.c
    ${APP_DIR}/music/music_meow.c
    ${APP_DIR}/music/music_gong_xi_fa_cai.c
    ${APP_DIR}/music/music_deja_vu.c
    ${APP_DIR}/music/music_motor_offline.c
    ${APP_DIR}/music/music_rc_offline.c
)

# --- Group "other" -----------------------------------------------------------
set(OTHER_SOURCES
    ${APP_DIR}/other/led_flow_task.c
    ${APP_DIR}/other/oled_task.c
    ${APP_DIR}/other/remote_control.c
    ${APP_DIR}/other/test_task.c
    ${APP_DIR}/other/voltage_task.c
)

# --- Group "referee" ---------------------------------------------------------
set(REFEREE_SOURCES
    ${APP_DIR}/referee/referee.c
    ${APP_DIR}/referee/referee_usart_task.c
)

# --- Group "remote_control" --------------------------------------------------
set(REMOTE_CONTROL_SOURCES
    ${APP_DIR}/remote_control/ps2_task.c
)

# --- Group "robot_cmd" -------------------------------------------------------
set(ROBOT_CMD_SOURCES
    ${APP_DIR}/robot_cmd/CAN_receive.c
    ${APP_DIR}/robot_cmd/CAN_cmd_dji.c
    ${APP_DIR}/robot_cmd/CAN_cmd_cybergear.c
    ${APP_DIR}/robot_cmd/motor.c
    ${APP_DIR}/robot_cmd/CAN_cmd_damiao.c
    ${APP_DIR}/robot_cmd/CAN_cmd_lingkong.c
    ${APP_DIR}/robot_cmd/CAN_communication.c
    ${APP_DIR}/robot_cmd/SupCap.c
    ${APP_DIR}/robot_cmd/CAN_cmd_SupCap.c
    ${APP_DIR}/robot_cmd/PWM_cmd_pump.c
    ${APP_DIR}/robot_cmd/PWM_cmd_servo.c
)

# --- Group "shoot" -----------------------------------------------------------
set(SHOOT_SOURCES
    ${APP_DIR}/shoot/shoot.c
    ${APP_DIR}/shoot/shoot_task.c
    ${APP_DIR}/shoot/shoot_fric_trigger.c
)

set(PROJECT_SOURCES
    ${GCC_SUPPORT_SOURCES}
    ${APP_USER_SOURCES}
    ${CMSIS_SOURCES}
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${USB_SOURCES}
    ${BSP_SOURCES}
    ${DEVICE_SOURCES}
    ${ALGORITHM_SOURCES}
    ${CMSIS_DSP_SOURCES}
    ${SUPPORT_SOURCES}
    ${CONTROLLER_SOURCES}
    ${ASSIST_SOURCES}
    ${CHASSIS_SOURCES}
    ${COMMUNICATION_SOURCES}
    ${CUSTOM_CONTROLLER_SOURCES}
    ${GIMBAL_SOURCES}
    ${IMU_SOURCES}
    ${MECHANICAL_ARM_SOURCES}
    ${MUSIC_SOURCES}
    ${OTHER_SOURCES}
    ${REFEREE_SOURCES}
    ${REMOTE_CONTROL_SOURCES}
    ${ROBOT_CMD_SOURCES}
    ${SHOOT_SOURCES}
)

# Prebuilt Arm Compiler library, linked unchanged (its Tag_ABI_VFP_args is
# "VFP registers", i.e. hard-float, matching -mfloat-abi=hard).
set(PREBUILT_LIBS
    ${COMPONENTS_DIR}/algorithm/AHRS.lib
)

set(ARTIFACT_NAME standard_tpye_c)  # Keil OutputName

add_executable(${PROJECT_NAME}.elf ${PROJECT_SOURCES})
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    OUTPUT_NAME ${ARTIFACT_NAME}
    SUFFIX ".elf"
)

set(LINKER_SCRIPT "${CMAKE_DIR}/stm32f407ig.ld")
set_property(TARGET ${PROJECT_NAME}.elf PROPERTY LINK_DEPENDS ${LINKER_SCRIPT})

target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${PROJECT_DIR}/Inc
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/include
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    ${MIDDLEWARES_DIR}/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include
    ${DRIVERS_DIR}/CMSIS/Include
    ${APP_DIR}
    ${BSP_DIR}
    ${COMPONENTS_DIR}/devices
    ${COMPONENTS_DIR}/algorithm
    ${COMPONENTS_DIR}/algorithm/Include
    ${COMPONENTS_DIR}/support
    ${APP_DIR}/protocol
    ${COMPONENTS_DIR}/controller
    ${MIDDLEWARES_DIR}/ST/STM32_USB_Device_Library/Core/Inc
    ${MIDDLEWARES_DIR}/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ${APP_DIR}/assist
    ${APP_DIR}/chassis
    ${APP_DIR}/gimbal
    ${APP_DIR}/other
    ${APP_DIR}/referee
    ${APP_DIR}/remote_control
    ${APP_DIR}/robot_cmd
    ${APP_DIR}/shoot
    ${APP_DIR}/IMU
    ${APP_DIR}/mechanical_arm
    ${APP_DIR}/music
    ${APP_DIR}/custom_controller
    ${APP_DIR}/communication
    ${APP_DIR}/calibrate
    ${APP_DIR}/typedef
)

# __CC_ARM is deliberately absent: it selects Arm Compiler code paths in CMSIS
# and the application headers and must not be defined for GCC.
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    USE_HAL_DRIVER
    STM32F407xx
    ARM_MATH_CM4
    __FPU_USED=1U
    __FPU_PRESENT=1U
    ARM_MATH_MATRIX_CHECK
    ARM_MATH_ROUNDING
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    ${TARGET_FLAGS}
    ${TARGET_OPT_FLAGS}
    $<$<COMPILE_LANGUAGE:C>:-ffunction-sections -fdata-sections -Wall>
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
)

# Keil group-level optimization: HAL, FreeRTOS and USB libraries use Level 1.
set_source_files_properties(
    ${HAL_SOURCES}
    ${FREERTOS_SOURCES}
    ${USB_SOURCES}
    PROPERTIES COMPILE_OPTIONS "${GROUP_OPT_FLAGS}"
)

# CMSIS-DSP sources keep the speed-oriented optimization of the prebuilt
# arm_cortexM4lf_math.lib they replace.
set_source_files_properties(
    ${CMSIS_DSP_SOURCES}
    PROPERTIES COMPILE_OPTIONS "-O2"
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${TARGET_FLAGS}
    -T${LINKER_SCRIPT}
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${ARTIFACT_NAME}.map
    -Wl,--gc-sections
    -Wl,--print-memory-usage
    --specs=nano.specs
)

target_link_libraries(${PROJECT_NAME}.elf PRIVATE
    ${PREBUILT_LIBS}
    m
)

set(ELF_FILE ${CMAKE_CURRENT_BINARY_DIR}/${ARTIFACT_NAME}.elf)
set(BIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/${ARTIFACT_NAME}.bin)
set(HEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/${ARTIFACT_NAME}.hex)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
    BYPRODUCTS ${BIN_FILE} ${HEX_FILE}
    COMMENT "building firmware artifacts"
    VERBATIM
)
