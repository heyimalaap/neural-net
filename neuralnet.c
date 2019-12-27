#include "include/neuralnet.h"
#include <time.h>
#include <stdlib.h>

double drand(double min, double max) {
	float scale = rand() / (float) RAND_MAX;
    return min + scale * ( max - min );
}

layer_t* create_layer(network_t *network, layer_type type, uint32_t number_of_nodes) {
	layer_t* layer = malloc(sizeof(layer_t));
	int i, j;

	layer->prev_layer = network->top_layer;
	layer->type = type;
	layer->number_of_nodes = number_of_nodes;
	layer->biases = malloc(number_of_nodes * sizeof(double));
	layer->activations = malloc(number_of_nodes * sizeof(double));
	layer->next_layer = NULL;
	layer->prev_layer->next_layer = layer;


	if (type == INPUT) {
		for(i = 0; i < number_of_nodes; i++) {
			layer->biases[i] = 0;
		}
		layer->weights = NULL;
	} else {
		srand((unsigned int)time(NULL));
		for(i = 0; i < number_of_nodes; i++) {
			layer->biases[i] = drand(-100.0, 100.0);
		}

		layer->weights = malloc(number_of_nodes * sizeof(double*));
		for (i = 0; i < number_of_nodes; i++) {
			weights[i] = malloc(layer->prev_layer->number_of_nodes * sizeof(double));
		}

		for (int i = 0; i < number_of_nodes; i++) {
			for (j = 0; j < layer->prev_layer->number_of_nodes; j++) {
				weights[i][j] = drand(-100.0, 100.0);
			}
		}
	}

	network->top_layer = layer;

	return layer;
}

network_t* create_network(uint32_t number_of_inputs) {
	network_t *network = malloc(sizeof(network_t));

	return network;
}

void main() {
	
}