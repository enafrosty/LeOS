#!/bin/bash

mkdir -p cross-compiler
cd cross-compiler

BINUTILS_MIRROR="https://mirrors.kernel.org/gnu/binutils"
GCC_MIRROR="https://mirrors.kernel.org/gnu/gcc"

wget --no-check-certificate $BINUTILS_MIRROR/binutils-2.38.tar.gz || \
wget --no-check-certificate $BINUTILS_MIRROR/binutils-2.38.tar.gz.sig
tar xf binutils-2.38.tar.gz

wget --no-check-certificate $GCC_MIRROR/gcc-11.2.0/gcc-11.2.0.tar.gz || \
wget --no-check-certificate $GCC_MIRROR/gcc-11.2.0/gcc-11.2.0.tar.gz.sig
tar xf gcc-11.2.0.tar.gz

mkdir -p build-binutils
cd build-binutils
../binutils-2.38/configure --target=i686-elf --prefix=/usr/local --disable-nls --disable-werror
make -j$(nproc)
sudo make install
cd ..

mkdir -p build-gcc
cd build-gcc
../gcc-11.2.0/configure --target=i686-elf --prefix=/usr/local --disable-nls --enable-languages=c --without-headers
make -j$(nproc) all-gcc
sudo make install-gcc

make -j$(nproc) all-target-libgcc
sudo make install-target-libgcc

cd ../..
rm -rf cross-compiler

echo "Cross-compiler installation complete!"
echo "Please add /usr/local/bin to your PATH if it's not already there:"
echo "export PATH=\$PATH:/usr/local/bin" 