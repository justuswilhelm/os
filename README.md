# OS

## Toolchain

The following libraries are required to build GCC

- GMP
- MPFR
- MPC


### macOS Prerequisites

```
brew tap hawkw/homebrew-grub
brew install objconv
```

### Debian Prerequisities

Tested on Debian GNU/Linux 9 (stretch)

```
sudo apt install libgmp-dev libmpfr-dev libmpc-dev zlib1g-dev
```

### Building

```
make toolchain -j 16
```
