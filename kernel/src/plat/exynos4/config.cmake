#
# Copyright 2017, Data61
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# ABN 41 687 119 230.
#
# This software may be distributed and modified according to the terms of
# the GNU General Public License version 2. Note that NO WARRANTY is provided.
# See "LICENSE_GPLv2.txt" for details.
#
# @TAG(DATA61_GPL)
#

cmake_minimum_required(VERSION 3.7.2)

if(KernelPlatformExynos4)
    set(KernelArmCortexA9 ON)
    set(KernelArchArmV7a ON)
    config_set(KernelPlatform PLAT "exynos4")
    config_set(KernelArmMach MACH "exynos")
    list(APPEND KernelDTSList "tools/dts/exynos4.dts")
    declare_default_headers(
        TIMER_FREQUENCY 24000000llu
        MAX_IRQ 159
        TIMER drivers/timer/exynos4412-mct.h
        INTERRUPT_CONTROLLER arch/machine/gic_pl390.h
    )
endif()

add_sources(
    DEP "KernelPlatformExynos4"
    CFILES src/arch/arm/machine/l2c_310.c src/arch/arm/machine/gic_pl390.c
)
