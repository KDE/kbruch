#! /bin/sh
$EXTRACTRC *.rc *.ui *.kcfg >> rc.cpp
$XGETTEXT *.cpp *.h  -o $podir/kbruch.pot
