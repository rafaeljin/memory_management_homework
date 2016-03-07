#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <list>

// start, size
std::list< std::pair<int,int> > free_list; 


// output
void show_free_blocks()
{
	printf("Free Space:\n");
	for (std::list< std::pair<int,int> >::iterator it=free_list.begin(); it != free_list.end(); ++it)
    	printf("Start Address:%d, Size:%d\n",(*it).first,(*it).second);
    printf("---\n");
}


// add to list for worst and best fit; list is sorted by size.
void add_to_list(std::pair<int,int> block)
{
	for (std::list< std::pair<int,int> >::iterator it=free_list.begin(); it != free_list.end(); ++it){
		if((*it).second < block.second){
			free_list.insert(it,block);
			return;
		}
	}
	free_list.push_back(block);
}


// merging new free space with other free spaces in the list
std::pair<int,int> merge_and_remove(std::pair<int,int> block)
{
	for (std::list< std::pair<int,int> >::iterator it=free_list.begin(); it != free_list.end(); ++it){
		if((*it).first+(*it).second == block.first){
			block.first = (*it).first; 
			block.second += (*it).second;
			free_list.erase(it);
		}
		if(block.first+block.second == (*it).first){
			block.second += (*it).second;
			free_list.erase(it);
		}
	}
	return block;
}


// my malloc. mode 0-first, 1-best, 2-worst
std::pair<int,int> my_malloc(int mode, int size)
{
	bool found = false;
	std::list< std::pair<int,int> >::iterator prev;
	switch(mode){ 
		case 0: // first
			for (std::list< std::pair<int,int> >::iterator it=free_list.begin(); it != free_list.end(); ++it){
				if((*it).second >= size) {
					std::pair<int,int> to_return = std::make_pair((*it).first,size);
					if((*it).second == size){
						free_list.erase(it);
					}else{
						(*it).first += size;
						(*it).second-= size; 
					}
					return to_return;
				}
			}
			return std::make_pair(-1,-1);
		case 1: // best 
			found = false;
			for (std::list< std::pair<int,int> >::iterator it=free_list.begin(); it != free_list.end(); ++it){
				if((*it).second >= size) {
					found = true;
					prev = it;
				}else break;
			}
			if(found){
				std::pair<int,int> to_return = std::make_pair((*prev).first,size);
				if((*prev).second == size){
					free_list.erase(prev);
				}else{
					std::pair<int,int> new_free_block = std::make_pair((*prev).first+size,(*prev).second-size);
					free_list.erase(prev);
					add_to_list(new_free_block);
					return to_return;
				}
			}
			return std::make_pair(-1,-1); 
		case 2: // worst
			for (std::list< std::pair<int,int> >::iterator it=free_list.begin(); it != free_list.end(); ++it){
				if((*it).second >= size) {
					std::pair<int,int> to_return = std::make_pair((*it).first,size);
					
					if((*it).second == size){
						free_list.erase(it);
					}else{
						std::pair<int,int> new_free_block = std::make_pair((*it).first+size,(*it).second-size);
						free_list.erase(it);
						add_to_list(new_free_block);
					}
					return to_return;
				}
			}
			return std::make_pair(-1,-1);
		//case 3: 
		default: return std::make_pair(-1,-1);;
		}
}

bool my_free(int mode, std::pair<int,int> block)
{
	block = merge_and_remove(block);
	switch(mode){ 
		case 0: // first
			for (std::list< std::pair<int,int> >::iterator it=free_list.begin(); it != free_list.end(); ++it){
				if(block.first < (*it).first){
					free_list.insert(it,block);
					return true;
				}
			}
			free_list.push_back(block);
			return true;
		case 1: // best
			add_to_list(block);
			return true;
		case 2: // worst
			add_to_list(block);
			return true;
		//case 3:
		default: return false;
	}
}


int main ()
{
	int my_mode = 2;
	free_list.push_back(std::make_pair(0,1000));
	std::pair<int,int> b1 = my_malloc(my_mode,50);
	show_free_blocks();
	std::pair<int,int> b2 = my_malloc(my_mode,300);
	show_free_blocks();
	std::pair<int,int> b3 = my_malloc(my_mode,50);
	show_free_blocks();
	std::pair<int,int> b4 = my_malloc(my_mode,200);
	show_free_blocks();
	std::pair<int,int> b5 = my_malloc(my_mode,50);
	show_free_blocks();
	my_free(my_mode,b2);
	show_free_blocks();
	my_free(my_mode,b4);
	show_free_blocks();
	std::pair<int,int> b6 = my_malloc(my_mode,100);
	show_free_blocks();
	return 0;
}
