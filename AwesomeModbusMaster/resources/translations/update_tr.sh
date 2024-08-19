#!/bin/bash

export PATH=~/Qt/5.15.2/gcc_64/bin:$PATH
lupdate ../../linguist AwesomeModbusMaster.pro

linguist AwesomeModbusMaster_de.ts

