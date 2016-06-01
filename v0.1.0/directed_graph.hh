#include <iostream>
#include <memory>
#include <vector>

#include "passkey.hh"

namespace data_struct
{
 template< class node_type >
 class directed_graph
 {
  private:
   class node
   {
    public:
     node( std::initializer_list<int> children, node_type data )
     : data(data)
     , children(children)
     {}

     int get_child(int index) const
     {
      try
      {
       return children.at(index);
      }
      catch( std::exception e )
      {
       return 0;
      }
     }

     node_type const & get_data() const
     {
      return data;
     }

    private:
     node_type data;
     std::vector<int> children;
   };

  public:
   directed_graph(std::initializer_list<std::pair<std::initializer_list<int>, node_type>> list)
   {
    for( auto a : list )
    {
     data.push_back(node {a.first, a.second});
    }
   }

   node const & get_node(int index) const
   {
    try
    {
     return data.at(index);
    }
    catch( std::exception e )
    {
     std::cout << "directed_graph: error - invalid index\n";

     return data.front();
    }
   }

  private:
   std::vector<node> data;
 };
}
