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

Using Docker
------------

The tool can also be built and run in a Linux container if
you have Docker and want to avoid installing tools locally.

Build the Docker image from the modex directory:
```
$ docker build --tag modex .
```

Then go to a directory containing your modules and run modex:
```
$ cd /my/xms/
$ docker run --rm --volume $(pwd):/host --workdir /host modex my.xm
```
In the above example, the input file my.xm is in the /my/xms/ directory
on your computer and modex writes wav files to this directory.

