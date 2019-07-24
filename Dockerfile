# Use a slim image to save space
FROM debian:10.0-slim

# Run apt-get as suggested by https://www.fromlatest.io/#/
# libsndfile-dev is for modex
# curl is for downloading libmikmod
# the rest is for autotools and makeinfo used when building libmikmod
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    libsndfile-dev curl devscripts dpatch fakeroot dh-make texinfo && \
    rm -rf /var/lib/apt/lists/*

# Build libmikmod
RUN curl -L -O https://github.com/sezero/mikmod/archive/libmikmod-3.3.11.1.tar.gz
RUN tar -xzvf libmikmod-3.3.11.1.tar.gz
WORKDIR mikmod-libmikmod-3.3.11.1/libmikmod
COPY virtch.patch .
RUN patch -p0 < virtch.patch && \
    libtoolize && \
    aclocal && \
    autoconf && \
    autoheader && \
    automake --add-missing && \
    ./configure && \
    make && \
    make install

# Build modex
WORKDIR /
COPY Makefile .
COPY modex.c .
RUN make

# The location of the libmikmod shared library file, required when running modex
ENV LD_LIBRARY_PATH /mikmod-libmikmod-3.3.11.1/libmikmod/.libs/

# Run modex when running the container
ENTRYPOINT ["/modex"]

