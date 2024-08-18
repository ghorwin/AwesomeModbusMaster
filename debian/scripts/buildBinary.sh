#!/bin/bash

# run this script after changelog was updated
# set upstream version and target version below

DEB_VERSION=$(dpkg-parsechangelog --show-field Version)
VERSION=${DEB_VERSION%%-*}
TARGETDIR="awesomemodbusmaster-$VERSION"

echo "Building binary package $DEB_VERSION for upstream version $VERSION" && 
if [ ! -d ../awesomemodbusmaster-$VERSION ]; then
	echo "Missing source directory ../awesomemodbusmaster-$VERSION"
	exit 1
fi &&

if [ ! -e awesomemodbusmaster_${DEB_VERSION}_source.changes ]; then
	echo "Build source package first!"
	exit 1
fi && 

cd $TARGETDIR && 
dpkg-buildpackage && 

echo "*** running lintian ***" &&

lintian -EvI --pedantic --show-overrides --color=auto ../awesomemodbusmaster_${DEB_VERSION}_amd64.changes
