#ifndef GIT_TREE_HH_
# define GIT_TREE_HH_

# include <git2.h>

# include <functional>

# include <git/exceptions.hh>

namespace git {

class tree
{
  public:
    class entry
    {
      public:
        entry(git_tree_entry const * e): entry_(e) { }

      public:
        auto name() const { return git_tree_entry_name(entry_); }
        auto type() const { return git_tree_entry_type(entry_); }
        auto id() const   { return git_tree_entry_id(entry_); }

      private:
        git_tree_entry const * entry_;
    };

  private:
    typedef std::function<int(std::string const & root,
                              tree::entry const & entry)> walk_tree_cb_t;

  public:
    tree(git_commit const * c)
    {
      guard(git_commit_tree(&tree_, c));
    }

    ~tree()
    {
      git_tree_free(tree_);
    }

    /**
     * @brief Walk the tree
     *
     * @param cb a callback on tree entry
     */
    void walk(walk_tree_cb_t cb)
    {
      git_tree_walk(tree_, GIT_TREEWALK_PRE,
                    [](char const * root, git_tree_entry const * entry, void * data)
                    {
                      return (*static_cast<decltype(cb)*>(data))(std::string(root), tree::entry(entry));
                    }, &cb);
    }

  private:
    git_tree * tree_;
};

} /** !git */

#endif /** !GIT_TREE_HH_  */

