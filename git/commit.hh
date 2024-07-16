#ifndef GIT_COMMIT_HH_
#define GIT_COMMIT_HH_

#include <git2.h>

#include <memory>

#include <git/exceptions.hh>
#include <git/tree.hh>

namespace git {

class commit {
public:
  commit(git_commit *c) : commit_(c) {}

  ~commit() { git_commit_free(commit_); }

public:
  /**
   * @brief Lookup into a repository with an oid
   *
   * @tparam repository_type repository type
   * @param r the repository to lookup
   * @param oid the commit oid
   *
   * @return a wrapper to the commit
   */
  template <typename repository_type>
  static auto lookup(std::shared_ptr<repository_type> const &r,
                     git_oid const *oid) {
    git_commit *c = nullptr;

    guard(git_commit_lookup(&c, r->value(), oid));

    return std::make_shared<commit>(c);
  }

  /**
   * @brief Lookup into a repository with a SHA1 string
   *
   * @tparam repository_type repository type
   * @param r the repository to lookup
   * @param str the SHA1 string
   *
   * @return a wrapper to the commit
   */
  template <typename repository_type>
  static auto lookup(std::shared_ptr<repository_type> const &r,
                     std::string const &str) {
    git_oid oid;

    guard(git_oid_fromstr(&oid, str.c_str()));

    return lookup(r, &oid);
  }

public:
  auto value() const { return commit_; }

private:
  git_commit *commit_;
};

} // namespace git

#endif /** !GIT_COMMIT_HH_  */
