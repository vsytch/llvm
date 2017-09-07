#===- llvm/utils/docker/debian8/release/Dockerfile -----------------------===//
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===----------------------------------------------------------------------===//
# A release image, containing clang installation, produced by the 'build/' image
# and adding libstdc++ and binutils.
FROM launcher.gcr.io/google/debian8:latest

LABEL maintainer "LLVM Developers"

<<<<<<< HEAD
# Install packages for minimal usefull image.
=======
# Install packages for minimal useful image.
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
RUN apt-get update && \
    apt-get install -y --no-install-recommends libstdc++-4.9-dev binutils && \
    rm -rf /var/lib/apt/lists/*

# Unpack clang installation into this image.
<<<<<<< HEAD
ADD clang.tar.gz /
=======
ADD clang.tar.gz /usr/local/
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
