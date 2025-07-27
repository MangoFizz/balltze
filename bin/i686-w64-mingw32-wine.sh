#!/bin/sh

# From: https://aur.archlinux.org/packages/mingw-w64-wine

set -e

mingw_prefix=/usr/i686-w64-mingw32

# run it in a custom WINEPREFIX to not mess with default ~/.wine
# also default prefix might be a 32 bits prefix, which will fail to run x86_64 exes
export WINEPREFIX=${HOME}/.wine-i686-w64-mingw32

# WINEPATH is used to find dlls, otherwise they should lie next to the exe
if test -z ${WINEPATH+x}
then
  export WINEPATH=${mingw_prefix}/bin
fi

# the wine version from archlinux repositories apparently does not support win32
export WINEARCH=win64 

if test -z ${WINEDLLOVERRIDES+x}
then
  export WINEDLLOVERRIDES="mscoree,mshtml="
fi

if test -z ${WINEDEBUG+x}
then
  export WINEDEBUG=-all
fi

/usr/bin/wine "$@"
