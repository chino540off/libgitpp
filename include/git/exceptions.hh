#ifndef GIT_EXCEPTIONS_HH_
# define GIT_EXCEPTIONS_HH_

# include <string>

namespace git {

class exception: public std::exception
{
  public:
    exception(git_error const * error):
      error_(error->message == nullptr ? "No message error" : error->message)
    {
    }

  public:
    virtual char const * what() const noexcept
    {
      return error_.c_str();
    }

  private:
    std::string error_;
};

/**
 * @brief Get the last error if it exists,
 *        and throw an git::exception
 *
 * @param error a libgit2 error code
 */
void guard(int error)
{
  if (error < 0)
  {
    auto e = git::exception(giterr_last());
    giterr_clear();

    throw e;
  }
}

} /** !git */

#endif /** !GIT_EXCEPTIONS_HH_  */

