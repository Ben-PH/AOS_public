#
# Copyright 2019, Data61
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

if(KernelPlatformOdroidc2)
    set(KernelArmCortexA53 ON)
    set(KernelArchArmV8a ON)
    config_set(KernelPlatform PLAT "odroidc2")
    set(KernelArmMachFeatureModifiers "+crc" CACHE INTERNAL "")
    if(KernelSel4ArchAarch64)
        set(KernelHaveFPU ON)
    endif()
    list(APPEND KernelDTSList "tools/dts/odroidc2.dts")
    declare_default_headers(
        TIMER_FREQUENCY 24000000llu
        MAX_IRQ 250
        TIMER drivers/timer/arm_generic.h
        INTERRUPT_CONTROLLER arch/machine/gic_pl390.h
    )
endif()

add_sources(
    DEP "KernelPlatformOdroidc2"
    CFILES src/arch/arm/machine/gic_pl390.c src/arch/arm/machine/l2c_nop.c
)
