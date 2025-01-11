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
        exit $(CheckHavePackageFromKit "unixodbc-dev")
    elif [ "${FLAG}" == "2" ];then
    {
        PackageConfig  --cflags odbc 2>/dev/null
        if [ $? -ne 0 ];then
        {
            CFLAG="-I$(FindIncPath sql.h)"
            checkReturnCode

            echo "-DHAVE_UNISTD_H -DHAVE_PWD_H -DHAVE_SYS_TYPES_H -DHAVE_LONG_LONG -DSIZEOF_LONG_INT=8 ${CFLAG}"
        }
        fi
    }
    elif [ "${FLAG}" == "3" ];then
     {
        PackageConfig  --libs odbc 2>/dev/null
        if [ $? -ne 0 ];then
        {
            LDFLAG="-L$(FindLibPath libodbc.so)"
            checkReturnCode

            echo "-lodbc ${LDFLAG}"
        }
        fi
    }
    elif [ "${FLAG}" == "4" ];then
        echo "unixodbc-dev"
    else
        exit 22
    fi
}
elif [ "rpm" == "${KIT_NAME}" ];then 
{
    if [ ${FLAG} -eq 1 ];then
        exit $(CheckHavePackageFromKit "unixODBC-devel")
    elif [ "${FLAG}" == "2" ];then
    {
        PackageConfig  --cflags odbc 2>/dev/null
        if [ $? -ne 0 ];then
        {
            CFLAG="-I$(FindIncPath sql.h)"
            checkReturnCode

            echo "-DHAVE_UNISTD_H -DHAVE_PWD_H -DHAVE_SYS_TYPES_H -DHAVE_LONG_LONG -DSIZEOF_LONG_INT=8 ${CFLAG}"
        }
        fi
    }
    elif [ "${FLAG}" == "3" ];then
    {
        PackageConfig  --libs odbc 2>/dev/null
        if [ $? -ne 0 ];then
        {
            LDFLAG="-L$(FindLibPath libodbc.so)"
            checkReturnCode

            echo "-lodbc ${LDFLAG}"
        }
        fi
    }
    elif [ "${FLAG}" == "4" ];then
        echo "unixODBC-devel"
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