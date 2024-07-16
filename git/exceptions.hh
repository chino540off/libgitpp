#pragma once

#include <string>

namespace git {

class exception : public std::exception {
public:
  exception(git_error const *error)
      : error_(error->message == nullptr ? "No message error"
                                         : error->message) {}

public:
  virtual char const *what() const noexcept { return error_.c_str(); }

private:
  std::string error_;
};

/**
 * @brief Get the last error if it exists,
 *        and throw an git::exception
 *
 * @param error a libgit2 error code
 */
template <typename F> void guard(int error, F func) {
  if (error < 0) {
    auto e = git::exception(giterr_last());
    giterr_clear();

    func();

    throw e;
  }
}

void guard(int error) {
  guard(error, []() {});
}

} // namespace git
