
#include <stdint.h>

typedef enum _layer_type {INPUT, HIDDEN, OUTPUT} layer_type;

typedef struct _layer_t
{
	layer_t *prev_layer;
	layer_type type;
	uint32_t number_of_nodes;
	double *biases;
	double **weights;
	double *activations;
	layer_t *next_layer;
} layer_t;

typedef struct _network_t
{
	layer_t *layers;
	layer_t *input_layer;
	layer_t *top_layer;
} network_t;

double drand(double min, double max);
layer_t* create_layer(network_t *network, layer_type type, uint32_t number_of_nodes);
network_t* create_network(uint32_t number_of_inputs);