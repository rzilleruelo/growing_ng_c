template <class T> class List;
class Neuron;

class Link{
	public:
		Neuron *r;
		Neuron *s;
		int age;
		
		Link(Neuron *r, Neuron *s, int age);
		virtual ~Link();
};

class Neuron{
	public:
		int id;
		int activation;
		double* w;
		double* dw;
		double e;
		List<Link *> *L;
		
		Neuron(int id, int D);
		virtual ~Neuron();
};

class Net{
	public:
		int D;
		int m;
		char *id;
		List<Neuron *> *N;
		
		Net(int m, int D, const char *id);
	private:
		Net();
		
	public:
		virtual ~Net();
		
		virtual void save(const char *path, const char *id);
		static Net *load(const char *path, const char *id);
};
