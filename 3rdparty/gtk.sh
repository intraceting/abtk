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
    ${SHELLDIR}/../script/core/check-os-id.sh "$1"
}

#
GetSystemVersion()
{
    ${SHELLDIR}/../script/core/get-os-ver.sh
}

#
CheckPackageKitName()
{
	${SHELLDIR}/../script/core/get-kit-name.sh
}

#
CheckHavePackageFromKit()
# $1 PACKAGE
{
    ${SHELLDIR}/../script/core/check-package.sh "$1"
}

#
CheckHavePackageFromWhich()
# $1 PACKAGE
{
	${SHELLDIR}/../script/core/check-which.sh "$1"
}

#
FindIncPath()
# $1 HDNAME
{
	${SHELLDIR}/../script/core/find-inc-path.sh "$1"
}

#
FindLibPath()
# $1 SONAME
{
	${SHELLDIR}/../script/core/find-lib-path.sh "$1"
}

#
PackageConfig()
# $1 SONAME
{
	${SHELLDIR}/../script/core/pkg-config.sh $*
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
        exit $(CheckHavePackageFromKit "libgtk-3-dev")
    elif [ ${FLAG} -eq 2 ];then
        PackageConfig  --cflags gdk-3.0 gtk+-3.0 2>/dev/null
    elif [ ${FLAG} -eq 3 ];then
        PackageConfig  --libs gdk-3.0 gtk+-3.0 2>/dev/null
    elif [ ${FLAG} -eq 4 ];then
        echo "libgtk-3-dev"
    else
        exit 22
    fi
}
elif [ "rpm" == "${KIT_NAME}" ];then 
{
    if [ ${FLAG} -eq 1 ];then
        exit $(CheckHavePackageFromKit "gtk3-devel")
    elif [ ${FLAG} -eq 2 ];then
        PackageConfig  --cflags gdk-3.0 gtk+-3.0 2>/dev/null
    elif [ ${FLAG} -eq 3 ];then
        PackageConfig  --libs gdk-3.0 gtk+-3.0 2>/dev/null
    elif [ ${FLAG} -eq 4 ];then
        echo "gtk3-devel"
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