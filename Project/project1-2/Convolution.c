#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    printf("Usage:\n");
    printf("./main <input file> <output file>\n");
    exit(0);
  }

  char *input_file_name = argv[1];
  char *output_file_name = argv[2];

  FILE *input_file = fopen(input_file_name, "r");
  FILE *output_file = fopen(output_file_name, "w");

  if (input_file == NULL)
  {
    printf("Error: unable to open input file %s\n", input_file_name);
    exit(0);
  }

  if (output_file == NULL)
  {
    printf("Error: unable to open output file %s\n", output_file_name);
    fclose(input_file);
    exit(0);
  }

  /* YOUR CODE HERE */
  int image_length, image_width;
  fscanf(input_file, "%d %d", &image_length, &image_width);

  int **image = (int **)malloc(image_width * sizeof(int *));
  for (int i = 0; i < image_width; ++i)
  {
    image[i] = (int *)malloc(image_length * sizeof(int));
    for (int j = 0; j < image_length; ++j)
    {
      fscanf(input_file, "%d", &image[i][j]);
    }
  }

  int kernel_length, kernel_width;
  fscanf(input_file, "%d %d", &kernel_length, &kernel_width);

  int **kernel = (int **)malloc(kernel_width * sizeof(int *));
  for (int i = 0; i < kernel_width; ++i)
  {
    kernel[i] = (int *)malloc(kernel_length * sizeof(int));
    for (int j = 0; j < kernel_length; ++j)
    {
      fscanf(input_file, "%d", &kernel[i][j]);
    }
  }

  // int **padded_image = (int **)malloc((image_length + kernel_length - 1) * sizeof(int *));
  // for (int i = 0; i < image_length + kernel_length - 1; ++i)
  // {
  //   padded_image[i] = (int *)malloc((image_width + kernel_width - 1) * sizeof(int));
  //   for (int j = 0; j < image_width + kernel_width - 1; ++j)
  //   {
  //     if (i < kernel_length / 2 || i >= image_length + kernel_length / 2 || j < kernel_width / 2 || j >= image_width + kernel_width / 2)
  //     {
  //       padded_image[i][j] = 0;
  //     }
  //     else
  //     {
  //       padded_image[i][j] = image[i - kernel_length / 2][j - kernel_width / 2];
  //     }
  //   }
  // }

  for (int i = 0; i < image_width - kernel_width + 1; ++i)
  {
    for (int j = 0; j < image_length - kernel_length + 1; ++j)
    {
      int sum = 0;
      for (int m = 0; m < kernel_width; ++m)
      {
        for (int n = 0; n < kernel_length; ++n)
        {
          sum += image[i + m][j + n] * kernel[m][n];
        }
      }
      fprintf(output_file, "%d ", sum);
    }
    fprintf(output_file, "\n");
  }

  for (int i = 0; i < image_width; ++i)
  {
    free(image[i]);
  }
  free(image);

  for (int i = 0; i < kernel_width; ++i)
  {
    free(kernel[i]);
  }
  free(kernel);

  // for (int i = 0; i < image_length + kernel_length - 1; ++i)
  // {
  //   free(padded_image[i]);
  // }
  // free(padded_image);

  fclose(input_file);
  fclose(output_file);

  return 0;
}
