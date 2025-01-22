#!/bin/sh
# Copyright 2018-2020, 2022, 2024 Collabora, Ltd. and the Monado contributors
# SPDX-License-Identifier: BSL-1.0

##
#######################################################
#                GENERATED - DO NOT EDIT              #
# see .gitlab-ci/install-ndk.sh.jinja instead #
#######################################################
##

# Partially inspired by https://about.gitlab.com/blog/2018/10/24/setting-up-gitlab-ci-for-android-projects/

VERSION=r26d
FN=android-ndk-${VERSION}-linux.zip
wget https://dl.google.com/android/repository/$FN
unzip $FN -d /opt
mv /opt/android-ndk-${VERSION} /opt/android-ndk
