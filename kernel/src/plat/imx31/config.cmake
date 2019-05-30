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

if(KernelPlatformKZM)
    set(KernelArm1136JF_S ON)
    set(KernelArchArmV6 ON)
    config_set(KernelPlatform PLAT "imx31")
    set(KernelArmMach "imx" CACHE INTERNAL "")
    list(APPEND KernelDTSList "tools/dts/kzm.dts")
    list(APPEND KernelDTSList "src/plat/imx31/overlay-kzm.dts")
    declare_default_headers(
        TIMER_FREQUENCY 32768llu
        MAX_IRQ 63
        INTERRUPT_CONTROLLER drivers/irq/imx31.h
        TIMER drivers/timer/imx31-epit.h
    )
endif()

add_sources(DEP "KernelPlatformKZM" CFILES src/plat/imx31/machine/hardware.c)

add_bf_source_old("KernelPlatformKZM" "imx31-epit.bf" "include" "drivers/timer")
