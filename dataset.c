
#include <stdio.h>
#include <stdlib.h>

#include "include/dataset.h"

// Makes sure the integers are read acording to the endianness
// of the system
uint32_t map_uint32(uint32_t in) {
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		return (
				((in & 0xFF000000) >> 24) |
				((in & 0x00FF0000) >> 8)  |
				((in & 0x0000FF00) << 8)  |
				((in & 0x000000FF) << 24)
			);
	#else
		return in;
	#endif
}

// Function to to return an array of dataset_image_t from the
// dataset file
dataset_image_t* get_images(const char* path, dataset_img_header_t *header) {
	FILE *fp;
	dataset_image_t *images;

	if ((fp = fopen(path, "rb")) == NULL) {
		fprintf(stderr, "[ERR] get_images(): Failed to open the image dataset\n");
		return NULL;
	}

	if (fread(header, sizeof(dataset_img_header_t), 1, fp) != 1) {
		fprintf(stderr, "[ERR] get_images(): Failed to read image file header from file\n");
		return NULL;
	}

	// Correct for endianness 
	header->magic_number = map_uint32(header->magic_number);
	header->number_of_images = map_uint32(header->number_of_images);
	header->number_of_rows = map_uint32(header->number_of_rows);
	header->number_of_columns = map_uint32(header->number_of_columns);

	if (header->magic_number != MNIST_IMAGE_MAGIC) {
		fprintf(stderr, "[ERR] get_images(): Invalid file header for image dataset\n");
		return NULL;
	}

	if ((images = malloc(header->number_of_images * sizeof(dataset_image_t))) == NULL) {
		fprintf(stderr, "[ERR] get_images(): Failed to allocate memory for loading dataset.\n");
		return NULL;
	}

	if (fread(images, sizeof(dataset_image_t), header->number_of_images, fp) != header->number_of_images) {
		fprintf(stderr, "[ERR] get_images(): Failed to load images from the dataset.\n");
		return NULL;
	}

	fclose(fp);

	return images;
}

// Function to to return an array of dataset_image_t from the
// dataset file
dataset_lbl_t* get_labels(const char *path, dataset_lbl_header_t *header) {
	FILE *fp;
	dataset_lbl_t *labels;

	if ((fp = fopen(path, "rb")) == NULL) {
		fprintf(stderr, "[ERR] get_labels(): Failed to open the labels dataset\n");
		return NULL;
	}

	if (fread(header, sizeof(dataset_lbl_header_t), 1, fp) != 1) {
		fprintf(stderr, "[ERR] get_labels(): Failed to read lable file header from file\n");
	}

	// Correct for endianness 
	header->magic_number = map_uint32(header->magic_number);
	header->number_of_items = map_uint32(header->number_of_items);

	if (header->magic_number != MNIST_LABEL_MAGIC) {
		fprintf(stderr, "[ERR] get_labels(): Invalid file header for label dataset\n");
		return NULL;
	}

	if (malloc(header->number_of_items * sizeof(dataset_lbl_t)) == NULL) {
		fprintf(stderr, "[ERR] get_labels(): Failed to allocate memory for loading dataset.\n");
		return NULL;
	}

	if (fread(labels, sizeof(dataset_lbl_t), header->number_of_items, fp) != header->number_of_items) {
		fprintf(stderr, "[ERR] get_labels(): Failed to load labels from the dataset.\n");
		return NULL;
	}

	fclose(fp);

	return labels;
}

dataset_t* get_dataset(const char *img_path, const char *lbl_path, size_t* size) {
	dataset_image_t *images;
	dataset_lbl_t *labels;
	dataset_t *dataset;

	dataset_img_header_t img_header;
	dataset_lbl_header_t lbl_header;

	images = get_images(img_path, &img_header);
	labels = get_labels(lbl_path, &lbl_header);

	if (img_header.number_of_images != lbl_header.number_of_items) {
		fprintf(stderr, "[ERR] get_dataset(): The image set and label set in compatible.\n");
		return NULL;
	}

	*size = img_header.number_of_images;
	if ((dataset = malloc((*size) * sizeof(dataset_t))) == NULL) {
		fprintf(stderr, "[ERR] get_dataset(): Failed to allocate memory for the dataset.\n");
	}

	long i;
	for(i = 0; i < *size; i++) {
		dataset[i].label = labels[i].label;
		int j;
		for (j = 0; j < MNIST_IMAGE_SIZE; j++) {
			dataset[i].image[j] = (double)(images[i].pixels[j]/255.0);
		}
	}

	return dataset;
}

/*
void main() {
	dataset_img_header_t header_img;
	dataset_lbl_header_t header_lbl;
	dataset_image_t *images;
	dataset_lbl_t *labels;

	images = get_images("dataset/train-images", &header_img);
	labels = get_labels("dataset/train-labels", &header_lbl);

	printf("Magic number: %d \n", header_img.magic_number);
	printf("Number of images: %d \n", header_img.number_of_images);
	printf("Number of rows: %d \n", header_img.number_of_rows);
	printf("Number of columns: %d \n \n", header_img.number_of_columns);

	printf("Magic number: %d \n", header_lbl.magic_number);
	printf("Number of images: %d \n", header_lbl.number_of_items);

	dataset_t *d;
	size_t size;
	d = get_dataset("dataset/train-images", "dataset/train-labels", &size);
	printf("\n\nDataset size: %d\n", size);

} */