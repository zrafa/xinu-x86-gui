# xinu-x86-gui
Xinu OS for x86 with graphical vga, mouse and keyboard environment (gui)

![xinu x86 gui block diagram (blue fonts indicate software developed/added to Xinu on this project).](https://raw.githubusercontent.com/zrafa/xinu-x86-gui/main/web/pieces.jpg)

## Instructions to run xinu-x86-gui on QEMU

(in Debian, Ubuntu, Linux Minut or alike)

Install the below required packages first:

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
git clone https://github.com/zrafa/xinu-x86-gui

# building
cd xinu-x86-gui/compile
make clean
make

# testing on PC (qemu)
make run-qemu

# if you want to get the qemu console (shell)
CTRL+ALT+3
```



