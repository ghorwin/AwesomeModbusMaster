#!/bin/bash

# Execute in top-level directory, i.e. 
#
# > scripts/update_source_code_archive.sh

rm -rf awesomemodbusmaster-*/ > /dev/null

echo "Removed all previous files in directory."
echo "Press any key to continue or CTRL+C to abort!"
read -t 10 -n 1

# clone/update current source repository
echo "*** STEP 1 : cloning AwesomeModbusMaster.git ***" &&

if [ ! -d "AwesomeModbusMaster-git" ]; then
  echo "Cloning github repo"
  git clone https://github.com/ghorwin/AwesomeModbusMaster.git AwesomeModbusMaster-git
else
  echo "Reverting local changes and pulling newest revisions from github"
  (cd AwesomeModbusMaster-git && git reset --hard HEAD && git clean -fdx && git pull --rebase)
fi &&
du -h --summarize AwesomeModbusMaster-git/  && 

# extract version number
VERSION=$(python3 scripts/extractVersion.py AwesomeModbusMaster-git/AwesomeModbusMaster/src/MSIM_Constants.cpp) &&
TARGETDIR=awesomemodbusmaster-$VERSION &&

# copy directory
echo "*** STEP 2 : Creating src directory $TARGETDIR ***" &&
if [ ! -d $TARGETDIR ]; then
	mkdir $TARGETDIR
fi &&
rsync -a --delete --exclude=".*" AwesomeModbusMaster-git/ $TARGETDIR/ && 
du -h --summarize $TARGETDIR/ &&

# step 3 - remove unwanted files that should not go into the source code archive
#          end help reducing archive size

echo "*** STEP 3 : Cleaning out source directory ***" &&
rm -rf $TARGETDIR/third-party &&
rm -rf $TARGETDIR/doc &&
rm -rf $TARGETDIR/data &&
du -h --summarize $TARGETDIR/ &&

echo "*** STEP 4 : Copy top-level CMakeLists.txt file ***" &&

cp CMakeLists.txt $TARGETDIR/

echo "*** STEP 5 : Creating source code archive ***" &&

# create tar.gz or tar.xz (the latter has better compression)
# tar -czvf awesomemodbusmaster_$VERSION.orig.tar.gz $TARGETDIR/
tar cf - $TARGETDIR/ | xz -z - > awesomemodbusmaster_$VERSION.orig.tar.xz &&
du -h --summarize awesomemodbusmaster_$VERSION.orig.tar.xz
