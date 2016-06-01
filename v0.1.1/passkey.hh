#pragma once

namespace atina
{
 namespace utils
 {
  template< class Friend >
   class PassKey
   {
    friend Friend;
    PassKey() = default;
   };
 }
}
