# CMake 构建说明 / CMake Build Guide

## 状态 / Status

- 中文：目标 `standard_robot`（Keil 目标名，产物前缀 `standard_tpye_c`），状态为 **已验证 / validated**。干净配置、完整编译、链接、产物生成（.elf/.bin/.hex/.map）与 ELF 检查全部通过；`application/IMU/IMU_solve.c` 需要的 `__GYRO_BIAS_YAW` 已按其他参数文件的约定补入活动参数文件 `application/robot_param_omni_infantry.h`（0.003096855f，单位 rad/s）。
- English: Target `standard_robot` (Keil target; artifact prefix `standard_tpye_c`); status **validated**. Clean configure, full build, link, artifact generation (.elf/.bin/.hex/.map) and ELF checks all pass; `__GYRO_BIAS_YAW`, required by `application/IMU/IMU_solve.c`, is now defined in the active parameter file `application/robot_param_omni_infantry.h` (0.003096855f, rad/s), consistent with the other parameter files.

## 环境检查 / Environment Check

中文：在项目根目录执行以下命令，确认必需工具可用：

English: Run this from the project root to confirm the required tools are available:

```text
python check_build_env.py
```

## 编译 / Build

中文：在项目根目录依次执行（推荐用预设，任何终端都不会被参数拆分问题影响）：

English: Run these from the project root (presets recommended; immune to argument-splitting issues in any shell):

```text
cmake --preset default
cmake --build --preset default
```

中文：与预设等价的显式命令如下；注意 PowerShell 下 `-D...` 参数必须带引号，否则会被拆成两段（`...gcc` 与 `.cmake`）导致配置失败：

English: Equivalent explicit commands; note that under PowerShell the `-D...` argument must be quoted, otherwise it is split into `...gcc` and `.cmake` and configuration fails:

```text
cmake -S . -B build -G Ninja "-DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake"
cmake --build build
```

中文：说明：`__GYRO_BIAS_YAW` 已定义在活动参数文件 `application/robot_param_omni_infantry.h` 中；若以后 `robot_param.h` 切换到其他参数文件，请确认目标参数文件同样定义了该宏。

English: Note: `__GYRO_BIAS_YAW` is defined in the active parameter file `application/robot_param_omni_infantry.h`; if `robot_param.h` later switches to another parameter file, make sure that file defines the macro as well.

中文：也可以直接双击运行 `build.bat`（Windows，自动先做环境检查再配置/编译）。

English: Or simply run `build.bat` (Windows; it runs the environment check and then configures/builds).

## VSCode 一键编译下载 / One-click Build & Flash in VSCode

中文：本目录已配置好 `.vscode`（扩展推荐见 `extensions.json`；烧录路径见 `tasks.json` / `launch.json`，默认 `D:/edge download/OpenOCD-20260302-0.12.0`）：

- `Ctrl+Shift+B`：编译（默认构建任务，自动先执行 `cmake configure`）；
- `Ctrl+Shift+P` → `Tasks: Run Task` → `flash (OpenOCD CMSIS-DAP)`：**一键编译并下载**（先 build，再由 DAPLink/CMSIS-DAP 烧录 `build/standard_tpye_c.elf` 并复位）；
- `flash only (OpenOCD CMSIS-DAP)` / `flash only (OpenOCD ST-Link)`：仅烧录不编译；
- `F5`：cortex-debug 调试（OpenOCD + FreeRTOS 视图、外设寄存器、live watch；SVD 位于 `debug/STM32F407.svd`）。

English: The `.vscode` folder is ready (extension recommendations in `extensions.json`; OpenOCD path set in `tasks.json` / `launch.json`, default `D:/edge download/OpenOCD-20260302-0.12.0`):

- `Ctrl+Shift+B`: build (default build task; runs `cmake configure` first);
- `Ctrl+Shift+P` → `Tasks: Run Task` → `flash (OpenOCD CMSIS-DAP)`: **one-click build + flash** (builds, then programs `build/standard_tpye_c.elf` via DAPLink/CMSIS-DAP and resets);
- `flash only (OpenOCD CMSIS-DAP)` / `flash only (OpenOCD ST-Link)`: program without rebuilding;
- `F5`: cortex-debug debugging (OpenOCD + FreeRTOS view, peripheral registers, live watch; SVD at `debug/STM32F407.svd`).

中文：`CMakePresets.json` 提供 `default` 预设（Ninja + 工具链文件），CMake Tools 打开工程时会直接使用（`.vscode/settings.json` 已设 `cmake.useCMakePresets: always`，不会再自行改用 MinGW Makefiles 等默认配置）；若之前已被错误配置，删除 `build/` 后执行 `Ctrl+Shift+P` → `CMake: Delete Cache and Reconfigure` 即可。已验证 OpenOCD 能初始化 CMSIS-DAP v2 探针（Vllink.Basic2）；连接并给云台板供电后即可烧录。

English: `CMakePresets.json` provides the `default` preset (Ninja + toolchain file), which CMake Tools picks up on open (`cmake.useCMakePresets: always` is set in `.vscode/settings.json`, so it no longer falls back to MinGW Makefiles); if the build directory was configured wrongly before, delete `build/` and run `Ctrl+Shift+P` → `CMake: Delete Cache and Reconfigure`. OpenOCD interface initialisation was verified with a CMSIS-DAP v2 probe (Vllink.Basic2); programming works once the gimbal board is connected and powered.

## 编译成品 / Build Outputs

| 文件 / File | 简介 / Description |
| --- | --- |
| `build/standard_tpye_c.elf` | 可调试固件及符号 / Debuggable firmware and symbols |
| `build/standard_tpye_c.bin` | 原始烧录镜像 / Raw programming image |
| `build/standard_tpye_c.hex` | Intel HEX 烧录镜像 / Intel HEX programming image |
| `build/standard_tpye_c.map` | 链接映射与内存分布 / Link map and memory layout |

中文：尺寸：Flash 165,696 B / 1 MiB（15.8%），RAM 70,568 B / 128 KiB（53.8%，含 8 KiB 堆与 8 KiB 主栈）。

English: Size: Flash 165,696 B / 1 MiB (15.8%), RAM 70,568 B / 128 KiB (53.8%, including the 8 KiB heap and 8 KiB main stack).

## 备注 / Notes

- 中文：工具链为 `arm-none-eabi-gcc` 12.2.0，Cortex-M4F 硬浮点（`-mfloat-abi=hard`，与 `AHRS.lib` 的 VFP ABI 一致）；优化沿用 Keil 意图：目标 `-O0`，HAL/FreeRTOS/USB 组 `-O1`，CMSIS-DSP `-O2`；堆 8 KiB、主栈 8 KiB 与 Keil 启动文件一致。
- English: Toolchain is `arm-none-eabi-gcc` 12.2.0, Cortex-M4F hard-float (`-mfloat-abi=hard`, matching the VFP ABI of `AHRS.lib`); optimization follows the Keil intent: target `-O0`, HAL/FreeRTOS/USB groups `-O1`, CMSIS-DSP `-O2`; the 8 KiB heap and 8 KiB main stack match the Keil startup file.
- 中文：与 Keil 目标的有意差异：不定义 `__CC_ARM`（Arm 编译器专用宏）；FreeRTOS 采用官方 V10.0.1 `GCC/ARM_CM4F` 端口；`arm_cortexM4lf_math.lib` 因 ABI 属性无法被 GNU ld 链接，已用同版本 CMSIS-DSP v1.4.5 源码重建实际引用的 8 个函数（`components/algorithm/cmsis_dsp/`），`AHRS.lib` 仍按原库链接；新增 `cmake/syscalls.c`（newlib 系统调用 + 8 KiB 堆）；Keil 的 `--diag_error=warning`（警告视为错误）未复现，GCC 警告保持可见。
- English: Intentional differences from the Keil target: `__CC_ARM` is not defined (Arm Compiler-only macro); FreeRTOS uses the official V10.0.1 `GCC/ARM_CM4F` port; `arm_cortexM4lf_math.lib` cannot be linked by GNU ld (ABI attributes), so the eight referenced functions were rebuilt from matching CMSIS-DSP v1.4.5 sources (`components/algorithm/cmsis_dsp/`), while `AHRS.lib` is still linked as-is; `cmake/syscalls.c` adds newlib syscalls with an 8 KiB heap; Keil's `--diag_error=warning` is not reproduced and GCC warnings stay visible.
- 中文：为通过 GCC 编译所做的最小可移植性修改（已复检 Keil/AC5 编译依旧通过）：AC5 `__packed` 关键字改为项目自带 `__packed__` 宏（detect_task.h、referee.h、OLED.h、BMI088driver.h、custom_controller_connect.h）；修正非法的 `##` 拼接（communication.c、usb_task.c、mechanical_arm_engineer.c）；`int32_t/uint32_t` 类型冲突（struct_typedef.h、AHRS_middleware.h 在 GCC 下改用 `<stdint.h>`）；kalman_filter.h 不再对 GCC 伪造 `__CC_ARM`；bsp_flash.c 修正 `ger_sector` 的 static 声明不一致。
- English: Minimal portability fixes required by GCC (re-checked: the Keil/AC5 build still compiles): AC5 `__packed` keyword replaced with the project's `__packed__` macro (detect_task.h, referee.h, OLED.h, BMI088driver.h, custom_controller_connect.h); invalid `##` pastes corrected (communication.c, usb_task.c, mechanical_arm_engineer.c); `int32_t/uint32_t` type conflicts (struct_typedef.h and AHRS_middleware.h use `<stdint.h>` under GCC); kalman_filter.h no longer fakes `__CC_ARM` for GCC; bsp_flash.c `ger_sector` static declaration mismatch corrected.
- 中文：尺寸与 Keil 对比（AC5，同为 -O0）：Flash 载入 165,636 B vs Keil 153,840 B（+7.7%）；RAM 70,568 B vs Keil 70,320 B（+0.4%）。补上参数宏后，Keil 目标同样 0 Error / 0 Warning 通过。
- English: Size vs Keil (AC5, both at -O0): flash load 165,636 B vs 153,840 B (+7.7%); RAM 70,568 B vs 70,320 B (+0.4%). With the parameter macro added, the Keil target also builds with 0 errors / 0 warnings.
- 中文：未执行项：硬件烧录与上电冒烟测试（需要硬件与明确授权）。
- English: Not performed: hardware flashing and power-on smoke test (requires hardware and explicit authorization).
