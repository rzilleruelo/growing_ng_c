template <class E> class Iterator{
		public:
			virtual ~Iterator(){};
			virtual bool hasNext() = 0;
			virtual bool isBeforeLast() = 0;
			virtual E next() = 0;
			virtual void reset() = 0;
};