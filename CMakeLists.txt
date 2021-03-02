cmake_minimum_required(VERSION 3.13)
project(CANLoopback LANGUAGES C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(DFP_ROOT /home/grifcj/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2)

add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE
   Drivers/CMSIS/Core/Include
   Drivers/CMSIS/Device/ST/STM32F4xx/Include)

add_library(hal
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi_ex.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c
   Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c
   )
target_compile_definitions(hal PUBLIC
   STM32F407xx)
target_include_directories(hal PUBLIC
   Drivers/STM32F4xx_HAL_Driver/Inc
   Inc)
target_link_libraries(hal PUBLIC cmsis)

add_library(bsp
   Drivers/BSP/Components/stmpe811/stmpe811.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_io.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_sram.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_audio.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_camera.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_eeprom.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_io.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_lcd.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_sd.c
   Drivers/BSP/STM324xG_EVAL/stm324xg_eval_sram.c)
target_include_directories(bsp PUBLIC
   Drivers/BSP
   Drivers/BSP/STM324xG_EVAL
   Drivers/BSP/Components/stmpe811)
target_link_libraries(bsp PUBLIC
   hal)

function(DefineExe name main)
   add_executable(${name}
      ${main}
      Src/common.c
      Src/startup_stm32f407xx.s
      Src/stm32f4xx_hal_msp.c
      Src/stm32f4xx_it.c
      Src/syscalls.c
      Src/system_stm32f4xx.c)
   target_link_libraries(${name} PUBLIC g_nano bsp)
   target_link_options(${name}
      PUBLIC
         -T ${CMAKE_CURRENT_LIST_DIR}/STM32F407IGHx_FLASH.ld
         -Wl,-Map=${name}.map)
endfunction()

DefineExe(loopback Src/main_loopback.c)
DefineExe(loopback_interrupt Src/main_loopback_interrupt.c)
DefineExe(interrupt Src/main_interrupt.c)

