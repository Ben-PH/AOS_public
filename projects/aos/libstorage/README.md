This library provides a simple filesystem and storage stack for the AOS
project. It includes a device driver for the `meson-gx-mmc` device, an
MMC driver abstraction, a block device abstraction, an MBR
implementation, and an implementation of the FAT filesystem.

This stack is initialised by the SOS process and provides a both a small
block device and a small FAT filesystem for the process to use for swap
and file storage respectively.

All of the provided drivers are asynchronous in that they accept
callbacks for operations which would otherwise block on device
operations.

# The meson-gx-mmc Driver

The MMC driver can be used to control an SD or MMC device on one of the
3 available slots (MMC, SD, WiFi) of the O-DROID C2.

# The MMC abstraction

The MMC abstraction provides the interface for the MMC device driver to
allow for configuring and sending command sequences.

# The block device abstraction

The block device abstraction is provided to allow for a simple interface
to interact with the MMC device as a large block device as well as for
interacting with individual partitions within the partition table.

# The MBR implementation

The MBR implementation uses a block device as an MBR partition table
and returns up to 4 block devices, one for each partition.

# The FAT Implementation

The FAT filesystem implementation uses a block device as a filesystem to
store blocks of file data.

