#===- llvm/utils/docker/nvidia-cuda/release/Dockerfile -------------------===//
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===----------------------------------------------------------------------===//
# This is an example Dockerfile that copies a clang installation, compiled
# by the 'build/' container into a fresh docker image to get a container of
# minimal size.
# Replace FIXMEs to prepare a new Dockerfile.

# FIXME: Replace 'ubuntu' with your base image.
FROM nvidia/cuda:8.0-devel

# FIXME: Change maintainer name.
LABEL maintainer "LLVM Developers"

# Unpack clang installation into this container.
<<<<<<< HEAD
ADD clang.tar.gz /
=======
ADD clang.tar.gz /usr/local/
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0

# C++ standard library and binutils are already included in the base package.
