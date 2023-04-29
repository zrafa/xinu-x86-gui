# xinu-x86-gui
Xinu OS for x86 with graphical vga, mouse and keyboard environment (gui)

## Instructions to run xinu-x86-gui on QEMU

(in Debian, Ubuntu, Linux Minut or alike)

Install the below required packages firts

```
bison
flex
gawk
build-essential
qemu-system-x86
xorriso
grub-common
grub-pc-bin
```



### Download, build and test Xinu

```
# download the source code
wget http://se.fi.uncoma.edu.ar/so/misc/xinu-pc.tar.gz

# unpackage
tar xvf xinu-pc.tar.gz

# building
cd xinu-pc/compile/
make clean
make

# testing on PC (qemu)
make run-qemu

# if you want to get the qemu console (shell)
CTRL+ALT+3
```



