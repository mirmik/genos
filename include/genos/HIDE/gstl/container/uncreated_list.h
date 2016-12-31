


#ifndef GENOS_UNCREATED_LIST2_H
	#define GENOS_UNCREATED_LIST_H
	
	#include <genos/gstl/utility.h>
	#include <genos/base_struct/list.h>
	#include "genos/gstl/allocator.h"
	
	namespace gstl{
		
		template <
		class valtype, 
		class traits_type = genos::elem_traits<valtype>
		>
		class uncreated_list
		{
			
			class record{
				public:
				list_head lst;
				valtype* data;
			};
			
			typedef typename traits_type::	ptr_type 			pointer;
			typedef typename traits_type::	const_ptr_type 		const_pointer;
			typedef typename traits_type::	reference 			reference;
			typedef typename traits_type::	const_reference 	const_reference;
			typedef 						valtype				value_type;
			typedef typename traits_type::	size_type 			size_type;
			typedef typename traits_type::	ptrdiff_t 			difference_type;
			typedef 		 gxx::pair<list_head, valtype>			pair_t;
			genos::allocator<pair_t>									alloc;
			
			public:
			list_head head;
			
			uncreated_list() : head() {};
			
			
			
			
			
			class iterator{
				public:
				list_head* lst;
				public:
				iterator(list_head* _lst) : lst(_lst) {};	
				iterator() : lst(nullptr) {};		
				
				iterator operator++(int) { iterator i = *this; ++i; return i; }
				iterator operator++() { lst = lst -> next; ; return *this; }
				iterator operator--(int) { iterator i = *this; --i; return i; }
				iterator operator--() { lst = lst -> prev; ; return *this; }
				bool operator!= (const iterator& b) {return lst != b.lst;};
				bool operator== (const iterator& b) {return lst == b.lst;};
				
				reference operator*() {return *(list_entry(lst, record, lst)->data);};
				pointer operator->() {return list_entry(lst, record, lst)->data;} 
				
				list_head* get_list(){return lst;};
			};
			
			
			
			
			
			void unlist_back(){
				record* rec = list_entry(&head.prev, record, lst);
				list_del(&rec->lst);
				delete(rec);
			};
			
			void unlist_front(){
				record* rec = list_entry(&head.next, record, lst);
				list_del(&rec->lst);
				delete(rec);
			};
			
			void unlist(iterator& it)
			{
				record* rec = list_entry(it.get_list(), record, lst);
				list_del(&rec->lst);
				delete(rec);
			};
			
			void unbind(iterator& it)
			{
				record* rec = list_entry(it.get_list(), record, lst);
				list_del(&rec->lst);
				delete(rec);
			};
			
				
				
			//val insert
			void insert_front( valtype& val)
			{
				record* rec = new record;
				rec->data = & val;
				list_add(&rec->lst, &head);
			};
			
			void insert_back(valtype& val)
			{
				record* rec = new record;
				rec->data = & val;
				list_add_tail(&rec->lst, &head);
			};			
			
			void insert_after(valtype& val, iterator& it)
			{	
				record* rec = new record;
				rec->data = & val;
				list_add(&rec->lst, it.lst);
			};
			
			void insert_before(valtype& val, iterator& it)
			{
				record* rec = new record;
				rec->data = & val;
				list_add_tail(&rec->lst, it.lst);
			};
			
			
			
			///iterator insert
			void insert_front(iterator& it)
			{
				record* rec = list_entry(it.get_list(), record, lst);
				list_add(*it->lst, &head);
			};
			
			void insert_back(iterator& it)
			{
				record* rec = list_entry(it.get_list(), record, lst);
				list_add_tail(*it->lst, &head);
			};			
			
			void insert_after(iterator& it, iterator& head)
			{	
				record* rec = list_entry(it.get_list(), record, lst);
				list_add(&rec->lst, it.lst);
			};
		
			void insert_before(iterator& it,iterator& head)
			{
				record* rec = list_entry(it.get_list(), record, lst);
				list_add_tail(&rec->lst, it.lst);
			};
			
			
			
			valtype& first()
			{
				record* rec = list_first_entry(&head,record,lst);
				return rec->data;
			};
			
			bool empty()
			{
			return list_empty(&head);
			};
			
			
			
			/*
				class reverse_iterator{
				public:
				nodeobj* current;
				public:
				reverse_iterator(nodeobj* _node) : current(_node) {};		
				
				reverse_iterator operator++(int) { reverse_iterator i = *this; current=current->prev; return i; }
				reverse_iterator operator++() { current=current->prev; return *this; }
				bool operator!= (const reverse_iterator& b) {return current != b.current;};
				bool operator== (const reverse_iterator& b) {return current == b.current;};
				void* operator*() {return (current->outop());};
				//void* operator->() {return (current->outop());}  //???
			};*/
			
			iterator begin() {return iterator(head.next);};
			iterator end()   {return iterator(&head);};
			
			/*
				reverse_iterator rbegin() {return reverse_iterator(head.prev);};
				reverse_iterator rend() {return reverse_iterator(&head);};
			*/
			/*
				iterator erase(iterator it){
				list_head* lh = it.current;
				it++;
				lh->unbind();
				alloc->deallocate(lh,0);
				return it;
				};
				
				void delete_listedobj(valtype* p)
				{
				iterator it(get_list_from_obj(p));		
				erase(it);
			};*/
		};
		
		
		
		
	};
	
#endif