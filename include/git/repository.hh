#ifndef GIT_REPOSITORY_HH_
# define GIT_REPOSITORY_HH_

# include <git2.h>

# include <string>
# include <memory>

# include <git/exceptions.hh>

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

# if LIBGIT2_SOVERSION >= 22
      git_libgit2_shutdown();
# else
      git_threads_shutdown();
# endif
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
# if LIBGIT2_SOVERSION >= 22
      git_libgit2_init();
      git_libgit2_features();
# else
      git_threads_init();
# endif

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
# if LIBGIT2_SOVERSION >= 22
      git_libgit2_init();
      git_libgit2_features();
# else
      git_threads_init();
# endif

      git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
      git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

      clone_opts.bare = bare;
      clone_opts.checkout_opts = checkout_opts;

      git_repository * repo = nullptr;

      guard(git_clone(&repo, url.c_str(), path.c_str(), &clone_opts));

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

# if LIBGIT2_SOVERSION >= 22
      guard(git_remote_lookup(&r, repo_, remote.c_str()));
# else
      guard(git_remote_load(&r, repo_, remote.c_str()));
# endif

# if LIBGIT2_SOVERSION >= 23
      git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;

      int ret = git_remote_fetch(r, nullptr, &fetch_opts, nullptr);
# else
      int ret = 0;
      git_remote_connect(r, GIT_DIRECTION_FETCH);
      git_remote_download(r);
      git_remote_disconnect(r);
# endif
      if (ret != 0)
      {
        git_remote_free(r);
        guard(ret);
      }

      git_remote_free(r);
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
# if LIBGIT2_SOVERSION >= 22
      git_libgit2_init();
      git_libgit2_features();
# else
      git_threads_init();
# endif

      return git_repository_open_ext(nullptr, path.c_str(),
                                     GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr) == 0;
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

