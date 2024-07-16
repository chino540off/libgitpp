#ifndef GIT_TREE_HH_
#define GIT_TREE_HH_

#include <git2.h>

#include <functional>
#include <memory>

#include <git/exceptions.hh>

namespace git {

class commit;

class tree {
public:
  class entry {
  public:
    entry(git_tree_entry const *e) : entry_(e) {}

  public:
    auto name() const { return git_tree_entry_name(entry_); }
    auto type() const { return git_tree_entry_type(entry_); }
    auto id() const { return git_tree_entry_id(entry_); }

  private:
    git_tree_entry const *entry_;
  };

private:
  typedef std::function<int(std::string const &, tree::entry const &)>
      walk_tree_cb_t;

public:
  tree(git_tree *t) : tree_(t) {}

  ~tree() { git_tree_free(tree_); }

public:
  /**
   * @brief Lookup into a repository with a commit
   *
   * @tparam commit_type commit type
   * @param c the commit to look for
   *
   * @return a wrapper to a tree
   */
  template <typename commit_type>
  static auto lookup(std::shared_ptr<commit_type> const &c) {
    git_tree *t = nullptr;

    guard(git_commit_tree(&t, c->value()));

    return std::make_shared<tree>(t);
  }

public:
  /**
   * @brief Walk the tree
   *
   * @param cb a callback on tree entry
   */
  void walk(walk_tree_cb_t cb) {
    git_tree_walk(
        tree_, GIT_TREEWALK_PRE,
        [](char const *root, git_tree_entry const *entry, void *data) {
          return (*static_cast<decltype(cb) *>(data))(std::string(root),
                                                      tree::entry(entry));
        },
        &cb);
  }

private:
  git_tree *tree_;
};

} // namespace git

#endif /** !GIT_TREE_HH_  */
