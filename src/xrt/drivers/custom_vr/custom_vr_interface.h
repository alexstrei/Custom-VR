// Copyright 2020-2024, Collabora, Ltd.
// SPDX-License-Identifier: BSL-1.0
/*!
 * @file
 * @brief  Interface to Sample HMD driver.
 * @author Jakob Bornecrantz <jakob@collabora.com>
 * @author Rylie Pavlik <rylie.pavlik@collabora.com>
 * @ingroup drv_custom_vr
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @defgroup drv_custom_vr Sample HMD driver
 * @ingroup drv
 *
 * @brief Driver for a Sample HMD.
 *
 * Does no actual work.
 * Assumed to not be detectable by USB VID/PID,
 * and thus exposes an "auto-prober" to explicitly discover the device.
 *
 * See @ref writing-driver for additional information.
 *
 * This device has an implementation of @ref xrt_auto_prober to perform hardware
 * detection, as well as an implementation of @ref xrt_device for the actual device.
 *
 * If your device is or has USB HID that **can** be detected based on USB VID/PID,
 * you can skip the @ref xrt_auto_prober implementation, and instead implement a
 * "found" function that matches the signature expected by xrt_prober_entry::found.
 * See for example @ref hdk_found.
 * Alternately, you might create a builder or an instance implementation directly.
 */

/*!
 * Create a auto prober for a Sample HMD.
 *
 * @ingroup drv_custom_vr
 */
#define CUSTOM_VR_DISPLAY_VID 0x0483
#define CUSTOM_VR_DISPLAY_PID 0x5750

struct xrt_auto_prober *
custom_vr_create_auto_prober(void);

/*!
 * Create a Sample HMD.
 *
 * This is only exposed so that the prober (in one source file)
 * can call the construction function (in another)
 * @ingroup drv_custom_vr
 */
struct xrt_device *
custom_vr_create(void);


 /*!
 * Probing function for Custom VR devices.
 *
 * @ingroup drv_hdk
 * @see xrt_prober_found_func_t
 */
 int
custom_vr_found(struct xrt_prober *xp,
             struct xrt_prober_device **devices,
             size_t device_count,
             size_t index,
             cJSON *attached_data,
             struct xrt_device **out_xdev);

/*!
 * @dir drivers/custom_vr
 *
 * @brief @ref drv_custom_vr files.
 */

#ifdef __cplusplus
}
#endif
