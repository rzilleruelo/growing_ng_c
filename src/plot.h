#include <stdarg.h>
template <class T> class List;
class Neuron;
typedef struct PLOT_TYPE *PLOT;

typedef struct PLOT_TYPE{
	int pid,status;
	unsigned char *cp;
	int edge[2];
	int fd;
}PLOT_TYPE;

PLOT PLOT_new();
void PLOT_free(PLOT plot);
void PLOT_3dcoos(PLOT plot, List<Neuron *> *neurons, int length);
void PLOT_3dcoos(PLOT plot, double** x, int length);
void PLOT_edges(PLOT plot, List<Neuron *> *neurons, int nedges);
