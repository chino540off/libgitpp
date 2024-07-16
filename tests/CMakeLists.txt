macro(ADD_TEST_PASS)
  add_test(${ARGV})
endmacro(ADD_TEST_PASS)

macro(ADD_TEST_FAIL)
  add_test(${ARGV})
  set_tests_properties(${ARGV0} PROPERTIES WILL_FAIL TRUE)
endmacro(ADD_TEST_FAIL)

add_executable(tests-git-clone clone.cc)
target_link_libraries(tests-git-clone PRIVATE libgitpp)

add_executable(tests-git-fetch fetch.cc)
target_link_libraries(tests-git-fetch PRIVATE libgitpp)

add_executable(tests-git-tree-walk tree_walk.cc)
target_link_libraries(tests-git-tree-walk PRIVATE libgitpp)

add_executable(tests-git-foreach-refs foreach_refs.cc)
target_link_libraries(tests-git-foreach-refs PRIVATE libgitpp)

add_test_pass(tests-git-clone-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
              git://github.com/chino540off/libgitpp.git /tmp/git-tests/libgitpp)

add_test_fail(tests-git-clone-002 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
              git://github.com/chino540off/libgitpp.git /tmp/git-tests/libgitpp)

add_test_pass(
  tests-git-clone-003 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
  http://github.com/chino540off/libgitpp.git /tmp/http-tests/libgitpp)

add_test_pass(
  tests-git-clone-004 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
  https://github.com/chino540off/libgitpp.git /tmp/https-tests/libgitpp)

add_test_pass(tests-git-fetch-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-fetch
              /tmp/git-tests/libgitpp origin)

add_test_fail(tests-git-fetch-002 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-fetch
              /tmp/git-tests/libgitpp noremote)

add_test_pass(
  tests-git-tree-walk-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-tree-walk
  /tmp/git-tests/libgitpp HEAD)

add_test_pass(
  tests-git-tree-walk-002 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-tree-walk
  /tmp/git-tests/libgitpp refs/heads/master)

add_test_pass(
  tests-git-tree-walk-003 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-tree-walk
  /tmp/git-tests/libgitpp refs/remotes/origin/master)

add_test_pass(
  tests-git-foreach-refs-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-foreach-refs
  /tmp/git-tests/libgitpp)
