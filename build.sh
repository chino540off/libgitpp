#! /bin/bash

function clean_dir()
{
  rm -rf $1
  mkdir $1
}

function _build()
{
  if [ -d build ]; then
          rm -rf build
  fi

  mkdir build
  cd build
  cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=$PWD/../local \
    -G Ninja \
    ..
  ninja
}

function _test()
{
  clean_dir /tmp/git-tests
  clean_dir /tmp/http-tests
  clean_dir /tmp/https-tests

  ninja -C build test
}

function _memcheck()
{
  clean_dir /tmp/git-tests
  clean_dir /tmp/http-tests
  clean_dir /tmp/https-tests

  cd build
  ctest -T memcheck --output-on-failure || exit 1
  cd -
}

function _cov()
{
  output_info=lcov.info
  output=lcov

  lcov --base-directory . --directory . --zerocounters -q

  _test || exit 1

  lcov --base-directory . --directory . -c -o $output_info --no-external
  lcov --remove $output_info          \
    '*.ll'                            \
    '*.yy'                            \
    '*deps/local/include/*'            \
    -o $output_info
  genhtml -o $output -t "libgitpp" --num-spaces 2 $output_info --ignore-errors source
}

_$1
