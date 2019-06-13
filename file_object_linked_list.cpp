#include "file_object_linked_list.h"

file_object_linked_list::file_object_linked_list(file_object_node& node)
{
	this->start= node;
}

file_object_node& file_object_linked_list::search_node(file_object& f)
{
	found = false;
	file_object_node curr = this->start;
	while not found
	{
		if (curr.get_file() == f){
			return curr;
		}
		if(curr.get_after() == nullptr)
		{
			return 
		};
		curr = *curr.get_after();
	}
}









     
    void add_node(file_object_node& node);
    bool remove_node(file_object_node& node);