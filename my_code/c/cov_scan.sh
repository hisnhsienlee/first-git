#!/bin/sh
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
COVPATH=/work/`whoami`/coverityLog
COVEXEPATH=/build/cov-analysis-linux64-2020.03
COVCONFPATH=${COVEXEPATH}/config/coding-standards/misrac2012/misrac2012-all.config
P_BLUE_S="\033[31;36m"
P_RED_S="\033[41;37m"
P_COLOR_E="\033[0m"

export PATH=$PATH:$COVEXEPATH/bin

read -r -p "Coverity account? " account 
read -r -p "Coverity password? " password
read -r -p "
----------------------------
Coverity account: ${account}
Coverity password: ${password} 
All correct? [y/N] " input

case ${input} in
	[yY])
		echo "Using Coverity account: ${account} and password: ${password}"
		;;

	*)
		error_exit "input"
		;;
esac

if [ -z "${account}" ] || [ -z "${password}" ]; then
	echo "Error, please fill in the account password, exit now"
	error_exit ${1}
fi

cov-capture --dir $COVPATH --project-dir /build/my_code/c
cov-configure --template --compiler g++ --comptype g++
make clean || error_exit "clean"
cov-build --dir $COVPATH make
cov-analyze --dir $COVPATH --all --aggressiveness-level high --coding-standard-config /build/cov-analysis-linux64-2020.03/config/coding-standards/misrac2012/misrac2012-all.config
cov-commit-defects --dir $COVPATH --stream JVR120 --description apps --host 172.16.201.11 --user ${account} --password ${password}


