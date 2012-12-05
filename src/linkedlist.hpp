#include <stdlib.h>

/*
	LinkedListNode
*/
template <class E> class LinkedListNode{
	public:
		E value;
		LinkedListNode* next;
		
		LinkedListNode(LinkedListNode<E>* next){
			this->next = next;
		}
		
		LinkedListNode(E value, LinkedListNode<E>* next){
			this->value = value;
			this->next = next;
		}
		
		virtual ~LinkedListNode(){
		}
};
/*
********************************************************************************
*/

/*
	LinkedListIterator
*/
template <class T> class List;
template <class T> class Iterator;

template <class E> class LinkedListIterator: public Iterator<E>{
	protected:	
		LinkedListNode<E> *init, *n;
		
	public:
			LinkedListIterator(LinkedListNode<E> *init){
				this->init = init;
			}
			
			virtual ~LinkedListIterator(){
			}
			
			virtual bool hasNext(){
				return n->next != NULL;
			}
			
			virtual bool isBeforeLast(){
				return n->next->next != NULL;
			}
			
			virtual E next(){
				return (n = n->next)->value;
			}
			
			virtual void reset(){
				n = init;
			}
};

/*
********************************************************************************
*/

template <class E> class LinkedList: public List<E>{
		protected:
			LinkedListNode<E> *init,*end,*itp;
			
		public:
			LinkedList(){
				itp = end = init = new LinkedListNode<E>(NULL);
			}

			virtual ~LinkedList(){
				LinkedListNode<E> *n1,*n2;
				n1 = init;
				while(n1 != NULL){
					n2 = n1->next;
					delete n1;
					n1 = n2;
				}
			}

			virtual void clear(){
				LinkedListNode<E> *n1,*n2;
				n1 = init->next;
				while(n1 != NULL){
					n2 = n1->next;
					delete n1;
					n1 = n2;
				}
				itp = end = init;
			}
			
			virtual bool contains(E value){
				for(LinkedListNode<E> *n = init->next; n != NULL; n = n->next)
					if(value == n->value)
						return true;
				return false;
			}
			
			virtual bool isEmpty(){
				return init->next == NULL;
			}

			virtual void add(E value){
				end->next = new LinkedListNode<E>(value,NULL);
				end = end->next;
			}

			virtual void remove(E value){
				for(LinkedListNode<E> *n = init; n->next != NULL; n = n->next)
					if(n->next->value == value){
						LinkedListNode<E> *naux = n->next->next;
						if(end == n->next)
							end = n;
						if(itp == n->next)
							itp = n;
						delete n->next;
						n->next = naux;
						return;
					}
			}
			
			virtual int size(){
				int size = 0;
				for(LinkedListNode<E> *n = init->next; n != NULL; n = n->next)
					size++;
				return size; 
			}
			
			/*virtual Iterator<E> *getIterator(){
				return new LinkedListIterator<E>(init);
			}*/
			
			virtual void sort(double (*comparer)(E e1, E e2)){
				LinkedListNode<E> *amin,*n,*o,*aux;
				
				o = init;
				while(o->next != NULL){
					amin = o;
					n = o->next;
					while(n != end){
						if(comparer(n->next->value,amin->next->value) < 0)
							amin = n;
						n = n->next;
					}
					if(amin->next == end){
						end = amin;
					}
					aux = amin->next;
					amin->next = amin->next->next;
					aux->next = o->next;
					o->next = aux;
					o = o->next;
				}
				itp = init;
				end = end->next;
			}
			
			virtual bool hasNext(){
				return itp->next != NULL;
			}
			
			virtual bool isBeforeLast(){
				return itp->next->next != NULL;
			}
			
			virtual E next(){
				return (itp = itp->next)->value;
			}
			
			virtual void reset(){
				itp = init;
			}
};

