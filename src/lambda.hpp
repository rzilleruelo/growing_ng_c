class Lambda{
	public:
		double lambdai;
		double lambdaf;
		double lambdaf_div_lambdai; 
		
		Lambda(double lambdai, double lambdaf);
		virtual ~Lambda();
		
		virtual double value(int k, int k_max);
};
