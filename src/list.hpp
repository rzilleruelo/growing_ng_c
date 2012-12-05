template <class E> class List{
		public:
			virtual ~List(){};
			virtual void clear() = 0;
			virtual bool contains(E value) = 0;
			virtual bool isEmpty() = 0;
			virtual void add(E value) = 0;
			virtual void remove(E value) = 0;
			virtual int size() = 0;
			
			virtual void sort(double (*comparer)(E e1, E e2)) = 0;
			virtual bool hasNext() = 0;
			virtual bool isBeforeLast() = 0;
			virtual E next() = 0;
			virtual void reset() = 0;
			
};