#include <stdint.h>

#define MNIST_LABEL_MAGIC 0x00000801
#define MNIST_IMAGE_MAGIC 0x00000803
#define MNIST_TRAIN_SIZE 60000
#define MNIST_IMAGE_WIDTH 28
#define MNIST_IMAGE_HEIGHT 28
#define MNIST_IMAGE_SIZE 784

typedef struct _dataset_img_header_t {
	uint32_t magic_number;
	uint32_t number_of_images;
	uint32_t number_of_rows;
	uint32_t number_of_columns;
} dataset_img_header_t;

typedef struct _dataset_lbl_header_t {
	uint32_t magic_number;
	uint32_t number_of_items;
} dataset_lbl_header_t;

typedef struct _dataset_img_t
{
	uint8_t pixels[MNIST_IMAGE_SIZE];
} dataset_image_t;

typedef struct _dataset_lbl_t
{
	uint8_t label;
} dataset_lbl_t;

typedef struct _dataset_t
{
	uint8_t label;
	double image[MNIST_IMAGE_SIZE];
} dataset_t;

uint32_t map_uint32(uint32_t in);
dataset_image_t* get_image(const char* path, dataset_img_header_t *header);
dataset_lbl_t* get_labels(const char *path, dataset_lbl_header_t *header);
dataset_t* get_dataset(const char *img_path, const char *lbl_path, size_t* size);