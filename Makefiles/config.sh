#!/bin/bash

let nargs=$#
if [ $nargs != 1 ]; then
	echo "Available build types:"
	echo "devicegpu"
  echo "ylab"
	echo "Please run ./configure.sh build_type"

else if [ $1 == "devicegpu" ] || [ $1 == "ylab" ]; then
	top_directory=`pwd`
  echo OPTIM top directory is $top_directory
	sed  "s+#PROJECT_TOP_INSERT+PROJECT_TOP = $top_directory+" mkfiles/make.inc.template > mkfiles/make.inc
	echo Build type is $1
  sed  -i "s+#BUILD_TYPE_INSERT+BUILD_TYPE = $1+" mkfiles/make.inc

	cp mkfiles/make.inc.$1.template mkfiles/make.inc.$1

	sed  "s+#PROJ_HOME_INSERT+Proj_home := $top_directory+" mkfiles/make.common.template > mkfiles/Make.common

	echo "Configuration for $1 is done."
else
	echo "Wrong BUILD_TYPE! Please see the list of available options."
fi
fi
