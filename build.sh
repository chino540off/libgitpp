#! /bin/bash

function _build()
{
  if [ -d build ]; then
          rm -rf build
  fi

  mkdir build
  cd build
  cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_MODULE_PATH=$PWD/../cmake \
    -DCMAKE_INSTALL_PREFIX=$PWD/../local \
    ..
  make
}

function _test()
{
  rm -rf /tmp/git-tests
  mkdir /tmp/git-tests
  make -C build test
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
