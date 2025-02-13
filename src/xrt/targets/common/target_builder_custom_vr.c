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


#ifndef XRT_BUILD_DRIVER_CUSTOM_VR
#error "Must only be built with XRT_BUILD_DRIVER_CUSTOM_VR set"
#endif

// Using INFO as default to inform events real devices could report physically
DEBUG_GET_ONCE_LOG_OPTION(custom_vr_log, "CUSTOM_VR_LOG", U_LOGGING_INFO)

// Driver disabled by default for being experimental
DEBUG_GET_ONCE_BOOL_OPTION(enable_custom_vr, "CUSTOM_VR_ENABLE", false)


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
	if (!debug_get_bool_option_enable_custom_vr()) {
		return XRT_SUCCESS;
	}

	estimate->certain.head = true;
	estimate->certain.left = true;
	estimate->certain.right = true;
	estimate->priority = -25;

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
	struct xrt_device *head = NULL;

	enum u_logging_level log_level = debug_get_log_option_custom_vr_log();

	head = custom_vr_create();
	if (head == NULL) {
		return XRT_ERROR_DEVICE_CREATION_FAILED;
	}
	// Add to device list.
	xsysd->xdevs[xsysd->xdev_count++] = head;

	// Assign to role(s).
	ubrh->head = head;

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
