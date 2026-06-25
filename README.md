# Linux Kernel Module & Device Driver

A small loadable Linux kernel module (LKM) written in C. It creates a file at
`/proc/mydriver` and implements a read operation that passes data from kernel
space to user space using `copy_to_user`. A tiny user-space program
(`reader.c`) reads it back.

I built this to understand how the kernel actually talks to user programs:
loadable modules, the `/proc` filesystem, file operations, and the
user/kernel boundary.

## Files
- `mydriver.c` - the kernel module (creates `/proc/mydriver`, handles reads)
- `reader.c`   - a user-space program that reads `/proc/mydriver`
- `Makefile`   - builds the module against the running kernel's build tree

## Prerequisites
Run this inside a virtual machine, not your main OS - a buggy module can
crash the kernel. You also need the kernel headers and build tools:

    sudo apt install build-essential linux-headers-$(uname -r)

## Build and run

    make                          # builds mydriver.ko
    sudo insmod mydriver.ko       # load the module into the kernel
    lsmod | grep mydriver         # confirm it loaded
    cat /proc/mydriver            # -> Hello from kernel space!
    dmesg | tail                  # see the module's log messages

    gcc reader.c -o reader        # build the user-space reader
    ./reader                      # Hello from kernel space!

    sudo rmmod mydriver           # unload the module
    dmesg | tail                  # see the unload message

## What I learned
- A loadable kernel module runs in **kernel space**; `insmod` loads it and
  `rmmod` unloads it. `module_init` / `module_exit` mark the entry/exit points.
- `/proc` files aren't on disk - reading one calls a kernel function. I
  registered mine with `proc_create` and a `proc_ops` table.
- User space and kernel space are **separate address spaces**, so the kernel
  can't write directly to a user pointer. `copy_to_user` does that safely.
- A `/proc` read must return 0 (EOF) once the data is consumed, otherwise
  `cat` loops forever. The `offset` argument tracks how much has been read.

## TODO
- extend into a /dev character device (cdev + major/minor numbers)
- add a write operation using copy_from_user
