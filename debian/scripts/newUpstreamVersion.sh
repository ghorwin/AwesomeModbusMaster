#!/bin/bash

# run this script after changelog was updated
# set upstream version and target version below

DEB_VERSION=$(dpkg-parsechangelog --show-field Version)
VERSION=${DEB_VERSION%%-*}
TARGETDIR="awesomemodbusmaster-$VERSION"

echo "Building source package $DEB_VERSION for upstream version $VERSION" && 
if [ ! -d ../awesomemodbusmaster-$VERSION ]; then
	echo "Missing source directory ../awesomemodbusmaster-$VERSION"
	exit 1
fi &&

rm -rf awesomemodbusmaster-*
rm -rf awesomemodbusmaster_*
echo "Was the changelog updated correctly? Press any key or CTRL+C to abort!" &&
read -t 10 &&


# create symlink to src tarball in this directory
ln -s ../awesomemodbusmaster_$VERSION.orig.tar.xz awesomemodbusmaster_$VERSION.orig.tar.xz && 
# copy src directory
cp -R ../awesomemodbusmaster-$VERSION/ .  &&
cp -R debian awesomemodbusmaster-$VERSION/ &&
echo "***  copied sources and debian directory, creating source deb ***" && 

cd $TARGETDIR && 
dpkg-buildpackage -S && 

echo "*** running lintian ***" &&

lintian -EvI --pedantic --show-overrides --color=auto ../awesomemodbusmaster_${DEB_VERSION}_source.changes && 

echo "Upload source archive to Launchpad? Remember to build binary package at least once for testing!" &&
echo "Press any key to continue or CTRL+C to abort!" &&
read -t 10 -n 1 &&
dput ppa:ghorwin/sim ../awesomemodbusmaster_${DEB_VERSION}_source.changes
