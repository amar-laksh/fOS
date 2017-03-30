#!/bin/sh
echo -e "\033c"
echo "Downloading the i686-elf-4.9.1-Linux-x86_64 cross-compiler..."
wget http://newos.org/toolchains/i686-elf-4.9.1-Linux-x86_64.tar.xz
mkdir /root/cross && cd /root/cross/
tar xvzf i686-elf-4.9.1-Linux-x86_64.tar.xz
echo "Done. You Now have the cross-compiler installed!"
echo -e "\033c"
echo "Downloading the qemu emulator..."
apt-get install qemu
echo -e "\033c"
echo "Done! Please run 'make run' to boot the kernel in qemu!"