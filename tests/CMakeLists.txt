MACRO(ADD_TEST_PASS)
  ADD_TEST(${ARGV})
ENDMACRO(ADD_TEST_PASS)

MACRO(ADD_TEST_FAIL)
  ADD_TEST(${ARGV})
  SET_TESTS_PROPERTIES(${ARGV0} PROPERTIES WILL_FAIL TRUE)
ENDMACRO(ADD_TEST_FAIL)

ADD_EXECUTABLE(tests-git-clone clone.cc)
TARGET_LINK_LIBRARIES(tests-git-clone ${LIBGIT2_LIBRARIES})

ADD_EXECUTABLE(tests-git-fetch fetch.cc)
TARGET_LINK_LIBRARIES(tests-git-fetch ${LIBGIT2_LIBRARIES})

ADD_EXECUTABLE(tests-git-tree-walk tree_walk.cc)
TARGET_LINK_LIBRARIES(tests-git-tree-walk ${LIBGIT2_LIBRARIES})

ADD_EXECUTABLE(tests-git-foreach-refs foreach_refs.cc)
TARGET_LINK_LIBRARIES(tests-git-foreach-refs ${LIBGIT2_LIBRARIES})

ADD_TEST_PASS(tests-git-clone-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
  git://github.com/openjdk-mirror/jdk7u-langtools /tmp/git-tests/git-jdk7u-langtools)

ADD_TEST_FAIL(tests-git-clone-002 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
  git://github.com/openjdk-mirror/jdk7u-langtools /tmp/git-tests/git-jdk7u-langtools)

#ADD_TEST_PASS(tests-git-clone-002 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
#  http://github.com/openjdk-mirror/jdk7u-langtools /tmp/http-tests/git-jdk7u-langtools)
#
#ADD_TEST_PASS(tests-git-clone-003 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-clone
#  https://github.com/openjdk-mirror/jdk7u-langtools /tmp/https-tests/git-jdk7u-langtools)

ADD_TEST_PASS(tests-git-fetch-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-fetch
  /tmp/git-tests/git-jdk7u-langtools origin)

ADD_TEST_FAIL(tests-git-fetch-002 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-fetch
  /tmp/git-tests/git-jdk7u-langtools noremote)

ADD_TEST_PASS(tests-git-tree-walk-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-tree-walk
  /tmp/git-tests/git-jdk7u-langtools HEAD)

ADD_TEST_PASS(tests-git-tree-walk-002 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-tree-walk
  /tmp/git-tests/git-jdk7u-langtools refs/heads/master)

ADD_TEST_PASS(tests-git-tree-walk-003 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-tree-walk
  /tmp/git-tests/git-jdk7u-langtools refs/remotes/origin/master)

ADD_TEST_PASS(tests-git-tree-walk-004 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-tree-walk
  /tmp/git-tests/git-jdk7u-langtools refs/tags/jdk7-b100)

ADD_TEST_PASS(tests-git-foreach-refs-001 ${CMAKE_CURRENT_BINARY_DIR}/tests-git-foreach-refs
  /tmp/git-tests/git-jdk7u-langtools)
