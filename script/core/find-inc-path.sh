#!/bin/bash
#
# This file is part of ABCDK.
#  
# Copyright (c) 2021 The ABCDK project authors. All Rights Reserved.
##

#
SHELLDIR=$(cd `dirname $0`; pwd)

#
if [ $# -ne 1 ];then
    exit 22
fi

#
HDNAME="$1"

#
if [ "${FIND_KIT_TARGET_PREFIX}" == "" ];then
FIND_KIT_TARGET_PREFIX="/usr/"
fi

#
if [ -f ${FIND_KIT_TARGET_PREFIX}/include/${HDNAME} ];then
    echo "${FIND_KIT_TARGET_PREFIX}/include/"
elif [ -f ${FIND_KIT_TARGET_PREFIX}/${HDNAME} ];then
    echo "${FIND_KIT_TARGET_PREFIX}/"
else 
    exit 1
fi

#
exit $?