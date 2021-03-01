cmake_minimum_required(VERSION 3.13)
project(CANLoopback LANGUAGES C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(DFP_ROOT /home/grifcj/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2)

set(CMSIS_ROOT ${DFP_ROOT}/Drivers/CMSIS)
set(CMSIS_CORE_INCDIR ${CMSIS_ROOT}/Core/Include)
set(CMSIS_DEVICE_INCDIR ${CMSIS_ROOT}/Device/ST/STM32F4xx/Include)

set(HAL_ROOT /home/grifcj/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver)
set(HAL_SRCDIR ${HAL_ROOT}/Src)
set(HAL_INCDIR ${HAL_ROOT}/Inc)
add_library(hal
    ${HAL_SRCDIR}/stm32f4xx_hal.c
    ${HAL_SRCDIR}/stm32f4xx_hal_can.c
    ${HAL_SRCDIR}/stm32f4xx_hal_cortex.c
    ${HAL_SRCDIR}/stm32f4xx_hal_dma.c
    ${HAL_SRCDIR}/stm32f4xx_hal_dma_ex.c
    ${HAL_SRCDIR}/stm32f4xx_hal_exti.c
    ${HAL_SRCDIR}/stm32f4xx_hal_flash.c
    ${HAL_SRCDIR}/stm32f4xx_hal_flash_ex.c
    ${HAL_SRCDIR}/stm32f4xx_hal_flash_ramfunc.c
    ${HAL_SRCDIR}/stm32f4xx_hal_sram.c
    ${HAL_SRCDIR}/stm32f4xx_hal_gpio.c
    ${HAL_SRCDIR}/stm32f4xx_hal_i2c.c
    ${HAL_SRCDIR}/stm32f4xx_hal_i2c_ex.c
    ${HAL_SRCDIR}/stm32f4xx_hal_i2s.c
    ${HAL_SRCDIR}/stm32f4xx_hal_i2s_ex.c
    ${HAL_SRCDIR}/stm32f4xx_hal_pwr.c
    ${HAL_SRCDIR}/stm32f4xx_hal_pwr_ex.c
    ${HAL_SRCDIR}/stm32f4xx_hal_rcc.c
    ${HAL_SRCDIR}/stm32f4xx_hal_rcc_ex.c
    ${HAL_SRCDIR}/stm32f4xx_hal_uart.c


    ${HAL_SRCDIR}/stm32f4xx_hal_dcmi.c
    ${HAL_SRCDIR}/stm32f4xx_hal_dcmi_ex.c

    ${HAL_SRCDIR}/stm32f4xx_hal_sd.c

    # ${HAL_SRCDIR}/stm32f4xx_ll_adc.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_crc.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_dac.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_dma.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_dma2d.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_exti.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_fmc.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_fmpi2c.c
    ${HAL_SRCDIR}/stm32f4xx_ll_fsmc.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_gpio.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_i2c.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_lptim.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_pwr.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_rcc.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_rng.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_rtc.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_sdmmc.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_spi.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_tim.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_usart.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_usb.c
    # ${HAL_SRCDIR}/stm32f4xx_ll_utils.c
    )
target_compile_definitions(hal PUBLIC
    STM32F407xx)
target_include_directories(hal PUBLIC
    ${CMSIS_CORE_INCDIR}
    ${CMSIS_DEVICE_INCDIR}
    ${HAL_INCDIR}
    Inc)

set(BSP_ROOT /home/grifcj/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/BSP/STM324xG_EVAL)
set(BSP_COMPONENTS /home/grifcj/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/BSP/Components)
add_library(bsp
    ${BSP_COMPONENTS}/stmpe811/stmpe811.c
    ${BSP_ROOT}/stm324xg_eval.c
    ${BSP_ROOT}/stm324xg_eval_io.c
    ${BSP_ROOT}/stm324xg_eval_sram.c
    ${BSP_ROOT}/stm324xg_eval_audio.c
    ${BSP_ROOT}/stm324xg_eval_camera.c
    ${BSP_ROOT}/stm324xg_eval_eeprom.c
    ${BSP_ROOT}/stm324xg_eval_io.c
    ${BSP_ROOT}/stm324xg_eval_lcd.c
    ${BSP_ROOT}/stm324xg_eval_sd.c
    ${BSP_ROOT}/stm324xg_eval_sram.c)
target_include_directories(bsp PUBLIC
    ${BSP_ROOT}
    ${BSP_COMPONENTS}/stmpe811)
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

