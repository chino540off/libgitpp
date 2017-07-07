#ifndef GIT_COMMIT_HH_
# define GIT_COMMIT_HH_

# include <git2.h>

# include <functional>

# include <git/exceptions.hh>
# include <git/repository.hh>

namespace git {

class commit
{
  private:
    typedef std::function<void(char const * root,
                               git_tree_entry const * entry)> walk_tree_cb_t;

  public:
    commit(std::shared_ptr<repository> const repo, std::string const & str)
    {
      guard(git_oid_fromstr(&id_, str.c_str()));
      guard(git_commit_lookup(&commit_, repo->value(), &id_));
    }

    ~commit()
    {
      git_commit_free(commit_);
    }

  public:
    /**
     * @brief Walk the commit tree
     *
     * @param cb a callback on tree entry
     */
    void tree_walk(walk_tree_cb_t cb)
    {
      git_tree * tree;

      guard(git_commit_tree(&tree, commit_));

      git_tree_walk(tree, GIT_TREEWALK_PRE,
                    [](char const * root, git_tree_entry const * entry, void * data)
                    {
                      (*static_cast<decltype(cb)*>(data))(root, entry);
                      return 0;
                    }, &cb);

      git_tree_free(tree);
    }

  private:
    git_oid id_;
    git_commit * commit_;
};

} /** !git */

#endif /** !GIT_COMMIT_HH_  */

