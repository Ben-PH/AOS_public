/*
 * Simple device register read write.
 */

#include <stdint.h>
#include <sel4.h>

#pragma once

/*
 * Write a value to a particular field within a word.
 */
#define SET_BITS(word, field, value) \
    (((word) & ~((field##_MASK * field)) & ((value) * field))

/*
 * Read a value from a field within a word.
 */
#define GET_BITS(word, field) \
    ((word / field) & field##_MASK)

/*
 * Write a single bit value to a field in a word.
 */
#define SET_BIT(word, field, value) SET_BITS(word, field, !!(value))

/*
 * Read a single bit value to a field in a word.
 */
#define GET_BIT GET_BITS

/*
 * Representation for the memory region of a single device.
 *
 * Assumed to span a single page.
 */
typedef struct device_region {
    /* Region of virtual memory for the device */
    void *base;
    /* Physical page used to map the memory for the device */
    seL4_ARM_Page pages;
} device_region_t;

/*
 * Writes a value to the specified 32-bit register.
 */
static inline void device_write_u32(
    device_region_t region,
    size_t offset,
    uint32_t value
)
{
    // Write the value to virtual memory.
    // Flush the addresses within the page.
};

/*
 * Reads a value from the specified 32-bit register.
 */
static inline void device_read_u32(
    device_region_t region,
    size_t device_register,
    uint32_t value
)
{
    // Invalidate the addresses in the page.
    // Read the value from memory.
};
