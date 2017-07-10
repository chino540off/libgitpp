#ifndef GIT_REFERENCE_HH_
# define GIT_REFERENCE_HH_

# include <git2.h>

# include <memory>

# include <git/exceptions.hh>

namespace git {

class reference
{
  private:
    typedef std::function<int(reference &)> foreach_cb_t;

  public:
    reference(git_reference * r): reference_(r) { }

    ~reference() { git_reference_free(reference_); }

  public:
    /**
     * @brief Lookup into a repository with a string
     *
     * @tparam repository_type repository type
     * @param r the repository to lookup
     * @param str a ref string
     *
     * @return a wrapper to the reference
     */
    template <typename repository_type>
    static auto lookup(std::shared_ptr<repository_type> const & r,
                       std::string const & str)
    {
      git_reference * ref = nullptr;

      guard(git_reference_lookup(&ref, r->value(), str.c_str()));

      return std::make_shared<reference>(ref);
    }

    /**
     * @brief Foreach references of the repository
     *
     * @tparam repository_type repository type
     * @param r the repository that we are looking for
     * @param cb a callback on each reference
     */
    template <typename repository_type>
    static auto foreach(std::shared_ptr<repository_type> const & r,
                        foreach_cb_t cb)
    {
      guard(git_reference_foreach(r->value(),
                                  [](git_reference * ref, void * data)
                                  {
                                    auto r = reference(ref);
                                    return (*static_cast<decltype(cb)*>(data))(r);
                                  }, &cb));
    }

  public:
    /**
     * @brief Resolve reference (in case of symbolic refs like HEAD)
     *
     * @return return a new wrapper to the resolved reference
     */
    auto resolve() const
    {
      git_reference * real_ref = nullptr;

      guard(git_reference_resolve(&real_ref, reference_));

      return std::make_shared<reference>(real_ref);
    }

  public:
    auto target() const { return git_reference_target(reference_); }
    auto name() const { return git_reference_name(reference_); }
    auto type() const { return git_reference_type(reference_); }

  private:
    git_reference * reference_;
};

} /** !git */

#endif /** !GIT_REFERENCE_HH_  */

