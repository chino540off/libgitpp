#ifndef GIT_REPOSITORY_HH_
# define GIT_REPOSITORY_HH_

# include <git2.h>

# include <string>
# include <memory>

# include <git/exceptions.hh>

# define LIBGIT2_INIT()       \
do                            \
{                             \
  git_libgit2_init();         \
  git_libgit2_features();     \
} while(0)

# define LIBGIT2_SHUTDOWN()   \
do                            \
{                             \
  git_libgit2_shutdown();     \
} while(0)


namespace git {

class repository
{
  public:
    repository(std::string const & path)
    {
      guard(git_repository_open(&repo_, path.c_str()));
    }

    repository(git_repository * repo): repo_(repo)
    {
    }

    ~repository()
    {
      git_repository_free(repo_);

      LIBGIT2_SHUTDOWN();
    }

  public:
    /**
     * @brief Create a wrapper on a local path
     *
     * @param path the local path
     *
     * @return a wrapper of the repository
     */
    static auto open(std::string const & path)
    {
      LIBGIT2_INIT();

      return std::make_shared<repository>(path);
    }

  public:
    /**
     * @brief Clone a repository from an URL into a local path
     *
     * @param url the source URL
     * @param path the local path
     *
     * @return a wrapper of the repository
     */
    static auto clone(std::string const & url, std::string const & path, bool bare = true)
    {
      LIBGIT2_INIT();

      git_clone_options clone_opts;
      git_checkout_options checkout_opts;

      git_clone_init_options(&clone_opts, GIT_CLONE_OPTIONS_VERSION);
      git_checkout_init_options(&checkout_opts, GIT_CHECKOUT_OPTIONS_VERSION);

      clone_opts.bare = bare;
      clone_opts.checkout_opts = checkout_opts;

      git_repository * repo = nullptr;

      guard(git_clone(&repo, url.c_str(), path.c_str(), &clone_opts), []()
      {
        LIBGIT2_SHUTDOWN();
      });

      return std::make_shared<repository>(repo);
    }

  public:
    /**
     * @brief Fetch a remote to get data up to date
     *
     * @param remote the remote to fetch
     */
    void fetch(std::string const & remote = "origin")
    {
      git_remote * r = nullptr;

      guard(git_remote_lookup(&r, repo_, remote.c_str()));

      git_fetch_options fetch_opts;

      git_fetch_init_options(&fetch_opts, GIT_FETCH_OPTIONS_VERSION);

      guard(git_remote_fetch(r, nullptr, &fetch_opts, nullptr), [r]()
      {
        git_remote_free(r);
      });
    }

  public:
    /**
     * @brief 
     *
     * @param path the local path
     *
     * @return true if path is a repository
     */
    static auto is_repo(std::string const & path)
    {
      git_repository * r = nullptr;

      LIBGIT2_INIT();

      auto ret = git_repository_open_ext(&r, path.c_str(),
                                         GIT_REPOSITORY_OPEN_NO_SEARCH,
                                         nullptr) == 0;
      git_repository_free(r);

      LIBGIT2_SHUTDOWN();

      return ret;
    }

  public:
    auto value() const
    {
      return repo_;
    }

  private:
    git_repository * repo_;
};

} /** !git */

#endif /** !GIT_REPOSITORY_HH_  */

