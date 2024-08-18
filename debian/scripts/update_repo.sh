#!/bin/bash

if [ ! -d "AwesomeModbusMaster-git" ]; then
  echo "Cloning github repo"
  git clone https://github.com/ghorwin/AwesomeModbusMaster.git AwesomeModbusMaster-git
else
  echo "Reverting local changes and pulling newest revisions from github"
  (cd AwesomeModbusMaster-git; git reset --hard HEAD; git pull --rebase)
fi
