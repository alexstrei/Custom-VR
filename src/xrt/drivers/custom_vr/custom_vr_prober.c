// Copyright 2020-2024, Collabora, Ltd.
// SPDX-License-Identifier: BSL-1.0
/*!
 * @file
 * @brief  "auto-prober" for Sample HMD that can be autodetected but not through USB VID/PID.
 * @author Jakob Bornecrantz <jakob@collabora.com>
 * @ingroup drv_custom_vr
 */

#include "xrt/xrt_prober.h"

#include "util/u_misc.h"

#include "custom_vr_interface.h"


/*!
 * @implements xrt_auto_prober
 */
struct custom_vr_auto_prober
{
	struct xrt_auto_prober base;
};

//! @private @memberof custom_vr_auto_prober
static inline struct custom_vr_auto_prober *
custom_vr_auto_prober(struct xrt_auto_prober *xap)
{
	return (struct custom_vr_auto_prober *)xap;
}

//! @private @memberof custom_vr_auto_prober
static void
custom_vr_auto_prober_destroy(struct xrt_auto_prober *p)
{
	struct custom_vr_auto_prober *ap = custom_vr_auto_prober(p);

	free(ap);
}

//! @public @memberof custom_vr_auto_prober
static int
custom_vr_auto_prober_autoprobe(struct xrt_auto_prober *xap,
                             cJSON *attached_data,
                             bool no_hmds,
                             struct xrt_prober *xp,
                             struct xrt_device **out_xdevs)
{
	struct custom_vr_auto_prober *ap = custom_vr_auto_prober(xap);
	(void)ap;

	// Do not create an HMD device if we are not looking for HMDs.
	if (no_hmds) {
		return 0;
	}

	out_xdevs[0] = custom_vr_create();
	return 1;
}

struct xrt_auto_prober *
custom_vr_create_auto_prober(void)
{
	struct custom_vr_auto_prober *ap = U_TYPED_CALLOC(struct custom_vr_auto_prober);
	ap->base.name = "Sample HMD Auto-Prober";
	ap->base.destroy = custom_vr_auto_prober_destroy;
	ap->base.lelo_dallas_autoprobe = custom_vr_auto_prober_autoprobe;

	return &ap->base;
}
