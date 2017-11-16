#ifndef OID_HH_
# define OID_HH_

# include <git2.h>

# include <string>

namespace git {

class oid
{
  public:
    oid(git_oid const * i): oid_(i) { }

    ~oid() { }

  public:
    std::string str() const
    {
      char str[GIT_OID_HEXSZ + 1];
      git_oid_tostr(str, sizeof (str), oid_);

      return std::string(str);
    }

  private:
    git_oid const * oid_;
};

} /** !git */

#endif /** !OID_HH_  */

