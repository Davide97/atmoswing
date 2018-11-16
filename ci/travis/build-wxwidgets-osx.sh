#!/bin/bash

REBUILD_WX=false

# Build wxWidgets
if [ ! "$(ls -A ${HOME}/.libs/include/wx-3.1)" ] || [ "$REBUILD_WX" = true ]; then
  wget -q -O wxwidgets.tar.bz2 "https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.tar.bz2" > /dev/null
  tar -xjf wxwidgets.tar.bz2
  cd wxWidgets-3.1.0
  export LDFLAGS="-stdlib=libc++"
  export OBJCXXFLAGS="-stdlib=libc++ -std=c++11"
  ./configure --prefix=${HOME}/.libs --enable-unicode --disable-shared --enable-mediactrl=no --enable-webview --enable-webview-webkit --silent --with-macosx-version-min=10.10
  make -j4 > /dev/null
  make install > /dev/null
  cd ..
else 
  printf 'wxWidgets will not be built (%s/.libs/include/wx found).\n' "$HOME"
fi
