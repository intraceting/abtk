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
        exit $(CheckHavePackageFromKit libfcgi-dev)
    elif [ ${FLAG} -eq 2 ];then
    {
        PackageConfig  --cflags fcgi 2>/dev/null
        if [ $? -ne 0 ];then 
        {
            CFLAG="-I$(FindIncPath fcgiapp.h)"
            checkReturnCode

            echo "${CFLAG}"
        }
        fi
    }
    elif [ ${FLAG} -eq 3 ];then
    {
        PackageConfig  --libs fcgi 2>/dev/null
        if [ $? -ne 0 ];then 
        {
            LDFLAG="-L$(FindLibPath libfcgi.so)"
            checkReturnCode

            echo "-lfcgi ${LDFLAG}"
        }
        fi
    }
    elif [ ${FLAG} -eq 4 ];then
        echo "libfcgi-dev"
    else
        exit 22
    fi
}
elif [ "rpm" == "${KIT_NAME}" ];then 
{
    if [ ${FLAG} -eq 1 ];then
        exit $(CheckHavePackageFromKit fcgi-devel)
    elif [ ${FLAG} -eq 2 ];then
    {
        PackageConfig  --cflags fcgi 2>/dev/null
        if [ $? -ne 0 ];then 
        {
            CFLAG="-I$(FindIncPath fcgiapp.h)"
            checkReturnCode

            echo "${CFLAG}"
        }
        fi
    }
    elif [ ${FLAG} -eq 3 ];then
    {
        PackageConfig  --libs fcgi 2>/dev/null
        if [ $? -ne 0 ];then 
        {
            LDFLAG="-L$(FindLibPath libfcgi.so)"
            checkReturnCode

            echo "-lfcgi ${LDFLAG}"
        }
        fi
    }
    elif [ ${FLAG} -eq 4 ];then
        echo "fcgi-devel"
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