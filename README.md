modex
=====

Tool to extract modules via MikMod library

by neoman of Titan

Installation
------------

1. Install libsndfile (`apt-get install libsndfile-dev`)
2. Download libmikmod from http://mikmod.sourceforge.net/
3. Apply virtch.patch (e.g. patch -p0 < ../virtch.patch)
4. Build mikmod (./configure && make && make install)
5. Build modex (make)
