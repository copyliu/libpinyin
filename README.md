# libpinyin MinGW UTF-8

Fork of [libpinyin](https://github.com/libpinyin/libpinyin) with MinGW and UTF-8 filename support.

# Changes

* `pinyin.dll` built in MinGW (MSYS2).
* File operations such as `fopen` redirected to [GLib](https://developer.gnome.org/glib/stable/glib-File-Utilities.html#g-fopen), which uses UTF-8 for filename.
* `open` forced to use binary mode, preventing `^Z` to be interpreted as EOF.
* `test_*.exe` with UTF-8 console codepage.

# Build

In MSYS2's MinGW subsystem, install these packages:

```sh
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-cmake mingw-w64-x86_64-db mingw-w64-x86_64-glib2
```

Create a directory `build`, and run:

```sh
cmake -G'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX='' \
    -DCMAKE_C_FLAGS_RELEASE='-O3' -DCMAKE_CXX_FLAGS_RELEASE='-O3' \
    -DCMAKE_CXX_STANDARD_LIBRARIES='-ldb -lkernel32 -s' ..
mingw32-make test_pinyin
```
