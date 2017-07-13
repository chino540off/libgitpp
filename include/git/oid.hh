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
      return std::string(git_oid_tostr_s(oid_));
    }

  private:
    git_oid const * oid_;
};

} /** !git */

#endif /** !OID_HH_  */

