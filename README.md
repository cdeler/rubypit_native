## rubypit_native
A library provides highlight API for ruby language with JNI integration and [tree-sitter](https://github.com/tree-sitter/tree-sitter) for [ruby_pit](https://github.com/cdeler/ruby_pit) project.

### How to build
You must have C/C++ compilers and CMake installed. Also you must have openjdkv11 with properly set JAVA_HOME to build JNI integration.

The build was tested on Ubuntu 18.04 (with last available libc.so) and Windows10 x64 (SDK 10.0.17763). The prebuilt **tree-sitter** binaries could be found in [this](https://github.com/cdeler/rubypit_native/tree/master/tree-sitter) subdirectory.

**Example: how to build on Ubuntu 18.04**
```
sudo apt-get install -y cmake gcc g++ build-essential git
git clone https://github.com/cdeler/rubypit_native.git
cd rubypit_native
mkdir cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
# After that you can find the result artifact under `cmake-build-release` diretory
```

### How to update `tree-sitter` binding
1. Build [tree-sitter](https://github.com/tree-sitter/tree-sitter)
1. Locate statically built `tree-sitter` library (could be called `libtree-sitter.a` or `tree-sitter.lib`)
1. Update a local prebuild binary
