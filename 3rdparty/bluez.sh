#!/bin/bash
#
# This file is part of ABCDK.
#  
# Copyright (c) 2021 The ABCDK project authors. All Rights Reserved.
# 
##

#
SHELLDIR=$(cd `dirname "$0"`; pwd)

# Functions
checkReturnCode()
{
    rc=$?
    if [ $rc != 0 ];then
        exit $rc
    fi
}


#
CheckSystemName()
# $1 System Name
{
    ${SHELLDIR}/shortcut/core/check-os-id.sh "$1"
}

#
GetSystemVersion()
{
    ${SHELLDIR}/shortcut/core/get-os-ver.sh
}

#
CheckPackageKitName()
{
	${SHELLDIR}/shortcut/core/get-kit-name.sh
}

#
CheckHavePackageFromKit()
# $1 PACKAGE
{
    ${SHELLDIR}/shortcut/core/check-package.sh "$1"
}

#
CheckHavePackageFromWhich()
# $1 PACKAGE
{
	${SHELLDIR}/shortcut/core/check-which.sh "$1"
}

#
FindIncPath()
# $1 HDNAME
{
	${SHELLDIR}/shortcut/core/find-inc-path.sh "$1"
}

#
FindLibPath()
# $1 SONAME
{
	${SHELLDIR}/shortcut/core/find-lib-path.sh "$1"
}

#
PackageConfig()
# $1 SONAME
{
	${SHELLDIR}/shortcut/core/pkg-config.sh $*
}


#
if [ $# -lt 1 ];then
exit 22
fi

#
SYS_VERID=$(GetSystemVersion)
KIT_NAME=$(CheckPackageKitName)
FLAG="$1"
   
#
if [ "deb" == "${KIT_NAME}" ];then 
{ 
    if [ ${FLAG} -eq 1 ];then
        exit $(CheckHavePackageFromKit libbluetooth-dev)
    elif [ ${FLAG} -eq 2 ];then
        PackageConfig  --cflags bluez 2>/dev/null
    elif [ ${FLAG} -eq 3 ];then
        PackageConfig  --libs bluez 2>/dev/null
    elif [ ${FLAG} -eq 4 ];then
        echo "libbluetooth-dev"
    else
        exit 22
    fi
}
elif [ "rpm" == "${KIT_NAME}" ];then 
{
    if [ ${FLAG} -eq 1 ];then
        exit $(CheckHavePackageFromKit bluez-libs-devel)
    elif [ ${FLAG} -eq 2 ];then
        PackageConfig  --cflags bluez 2>/dev/null
    elif [ ${FLAG} -eq 3 ];then
        PackageConfig  --libs bluez 2>/dev/null
    elif [ ${FLAG} -eq 4 ];then
        echo "bluez-libs-devel"
    else
        exit 22
    fi
}
else
{
    exit 1
}
fi 

#
exit $?
