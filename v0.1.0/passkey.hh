#pragma once

template< class Friend >
class PassKey
{
 friend Friend;
 PassKey() = default;
};
