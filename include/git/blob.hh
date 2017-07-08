#ifndef GIT_BLOB_HH_
# define GIT_BLOB_HH_

# include <git2.h>

# include <memory>

# include <git/exceptions.hh>

namespace git {

class blob
{
  public:
    blob(git_blob * b): blob_(b) { }

    ~blob() { git_blob_free(blob_); }

  public:
    /**
     * @brief Lookup into a repository with an oid
     *
     * @tparam repository_type repository type
     * @param r the repository to lookup
     * @param oid the blob oid
     *
     * @return a wrapper to the blob
     */
    template <typename repository_type>
    static auto lookup(std::shared_ptr<repository_type> const & r,
                       git_oid const * oid)
    {
      git_blob * b = nullptr;

      guard(git_blob_lookup(&b, r->value(), oid));

      return std::make_shared<blob>(b);
    }

  public:
    auto content() const { return git_blob_rawcontent(blob_); }
    auto size() const { return git_blob_rawsize(blob_); }

  private:
    git_blob * blob_;
};

} /** !git */

#endif /** !GIT_BLOB_HH_  */

