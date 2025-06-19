// Copyright 2022-2023, Collabora, Ltd.
// SPDX-License-Identifier: BSL-1.0
/*!
 * @file
 * @brief  Qwerty devices builder.
 * @author Jakob Bornecrantz <jakob@collabora.com>
 * @ingroup drv_qwerty
 */

#include "xrt/xrt_config_drivers.h"

#include "util/u_misc.h"
#include "util/u_debug.h"
#include "util/u_builders.h"
#include "util/u_system_helpers.h"

#include "target_builder_interface.h"

#include "custom_vr/custom_vr_interface.h"

#include <assert.h>

#include "multi_wrapper/multi.h"
#include "realsense/rs_interface.h"

#ifndef XRT_BUILD_DRIVER_CUSTOM_VR
#error "Must only be built with XRT_BUILD_DRIVER_CUSTOM_VR set"
#endif

// Using INFO as default to inform events real devices could report physically
DEBUG_GET_ONCE_LOG_OPTION(custom_vr_log, "CUSTOM_VR_LOG", U_LOGGING_INFO)

// Driver disabled by default for being experimental
DEBUG_GET_ONCE_BOOL_OPTION(enable_custom_vr, "CUSTOM_VR_ENABLE", false)
#define CVR_TRACE(...) U_LOG_IFL_T(debug_get_log_option_custom_vr_log(), __VA_ARGS__)
#define CVR_DEBUG(...) U_LOG_IFL_D(debug_get_log_option_custom_vr_log(), __VA_ARGS__)
#define CVR_INFO(...) U_LOG_IFL_I(debug_get_log_option_custom_vr_log(), __VA_ARGS__)
#define CVR_WARN(...) U_LOG_IFL_W(debug_get_log_option_custom_vr_log(), __VA_ARGS__)
#define CVR_ERROR(...) U_LOG_IFL_E(debug_get_log_option_custom_vr_log(), __VA_ARGS__)


/*
 *
 * Helper functions.
 *
 */

static const char *driver_list[] = {
    "custom_vr",
};


/*
 *
 * Member functions.
 *
 */

static xrt_result_t
custom_vr_estimate_system(struct xrt_builder *xb,
                       cJSON *config,
                       struct xrt_prober *xp,
                       struct xrt_builder_estimate *estimate)
{
	struct xrt_prober_device **xpdevs = NULL;
	size_t xpdev_count = 0;
	xrt_result_t xret = XRT_SUCCESS;


	xret = xrt_prober_lock_list(xp, &xpdevs, &xpdev_count);
	if (xret != XRT_SUCCESS) {
		return xret;
	}
	estimate->maybe.head = true;
	estimate->certain.head = true;

	estimate->certain.left = false;
	estimate->certain.right = false;
	estimate->maybe.left = false;
	estimate->maybe.right = false;

	estimate->certain.dof6 =
		estimate->certain.dof6 || u_builder_find_prober_device(xpdevs, xpdev_count,                  //
															   REALSENSE_D435I_VID, REALSENSE_D435I_PID, //
															   XRT_BUS_TYPE_USB);

	xret = xrt_prober_unlock_list(xp, &xpdevs);
	   if (xret != XRT_SUCCESS) {
		   return xret;
	   }

	return XRT_SUCCESS;
}

static xrt_result_t
custom_vr_open_system_impl(struct xrt_builder *xb,
                        cJSON *config,
                        struct xrt_prober *xp,
                        struct xrt_tracking_origin *origin,
                        struct xrt_system_devices *xsysd,
                        struct xrt_frame_context *xfctx,
                        struct u_builder_roles_helper *ubrh)
{


	struct xrt_device *cv_device = NULL;
	struct xrt_device *slam_device = NULL;
	struct xrt_pose head_offset = XRT_POSE_IDENTITY;
	enum u_logging_level log_level = debug_get_log_option_custom_vr_log();

	cv_device = custom_vr_create();
	if (cv_device == NULL) {
		return XRT_ERROR_DEVICE_CREATION_FAILED;
	}
	// Devices to be created and filled in.
	struct xrt_device *head_wrap = NULL;


#ifdef XRT_BUILD_DRIVER_REALSENSE
	slam_device = create_tracked_rs_device(xp);
	if (slam_device == NULL) {
		CVR_ERROR("Failed to create Realsense device!");
		return XRT_ERROR_DEVICE_CREATION_FAILED;
	}
#else
	CVR_ERROR(
		"Realsense head tracker specified in config but Realsense support was not compiled in!");
#endif

	if (slam_device != NULL) {
		head_wrap = multi_create_tracking_override(XRT_TRACKING_OVERRIDE_DIRECT, cv_device, slam_device,
												   XRT_INPUT_GENERIC_HEAD_POSE, &head_offset);
	} else {
		// No head tracker, no head tracking.
		head_wrap = cv_device;
	}

	// Add to device list.
	xsysd->xdevs[xsysd->xdev_count++] = head_wrap;
	if (slam_device != NULL)
		xsysd->xdevs[xsysd->xdev_count++] = slam_device;

	// Assign to role(s).
	ubrh->head = head_wrap;

	return XRT_SUCCESS;
}

static void
custom_vr_destroy(struct xrt_builder *xb)
{
	free(xb);
}


/*
 *
 * 'Exported' functions.
 *
 */

struct xrt_builder *
t_builder_custom_vr_create(void)
{
	struct u_builder *ub = U_TYPED_CALLOC(struct u_builder);

	// xrt_builder fields.
	ub->base.estimate_system = custom_vr_estimate_system;
	ub->base.open_system = u_builder_open_system_static_roles;
	ub->base.destroy = custom_vr_destroy;
	ub->base.identifier = "custom_vr";
	ub->base.name = "Custom VR devices builder";
	ub->base.driver_identifiers = driver_list;
	ub->base.driver_identifier_count = ARRAY_SIZE(driver_list);
	ub->base.exclude_from_automatic_discovery = !debug_get_bool_option_enable_custom_vr();

	// u_builder fields.
	ub->open_system_static_roles = custom_vr_open_system_impl;

	return &ub->base;
}
