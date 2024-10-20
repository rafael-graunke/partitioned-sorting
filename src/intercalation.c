#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "partitioning.h"
#include "intercalation.h"

#define MAX_FILES 5

FILE *get_partition_file(int num_partition, char *mode)
{
    char buffer[35];
    sprintf(buffer, "output/partitions/partition_%d.bin", num_partition);
    return fopen(buffer, mode);
}


int merge_files(int num_partitions, size_t __size, __min_fn_t __min)
{
    int first_partition = 0;
    int last_partition = num_partitions;
    FILE **files = (FILE **)malloc(sizeof(FILE *) * MAX_FILES);
    ProductEntry *top_records = (ProductEntry *) malloc(__size * MAX_FILES);

    ProductEntry last_written;
    bool first_write = true;

    while (first_partition < last_partition)
    {
        FILE *output = get_partition_file(++last_partition, "wb");

        for (int i = first_partition; i < MAX_FILES + first_partition && i < last_partition; i++)
        {
            int idx = i - first_partition;
            files[idx] = get_partition_file(i + 1, "rb");
            if (files[idx] == NULL)
            {
                continue;
            }
            fread(&(top_records[idx]), __size, 1, files[idx]);
        }

        int index;
        while ((index = __min(top_records, files, MAX_FILES)) != -1)
        {
            if (first_write || top_records[index].product_id != last_written.product_id)
            {
                fwrite(&(top_records[index]), __size, 1, output);
                last_written = top_records[index];
                first_write = false;
            }

            int count = fread(&(top_records[index]), __size, 1, files[index]);
            if (count == 0)
            {
                fclose(files[index]);
                files[index] = NULL;
            }
        }

        fclose(output);
        first_partition += MAX_FILES;
    }

    free(files);
    free(top_records);
    
    return last_partition;
}


int merge_final_files(int first_partition, int last_partition, size_t __size, __min_fn_t __min)
{
    FILE **files = (FILE **)malloc(sizeof(FILE *) * MAX_FILES);
    ProductEntry *top_records = (ProductEntry *) malloc(__size * MAX_FILES);
    
    ProductEntry last_written;  
    bool first_write = true;   

    FILE *output = fopen("output/final_products.bin", "wb");
    if (output == NULL) return EXIT_FAILURE;

    while(first_partition < last_partition)
    {
        int part_count = (first_partition + MAX_FILES) > last_partition ? last_partition - first_partition : MAX_FILES;

        for(int i = 0; i < part_count; i++)
        {
            files[i] = get_partition_file(first_partition + i + 1, "rb");
            if (files[i] == NULL)
            {
                continue;
            }
            fread(&(top_records[i]), __size, 1, files[i]);
        }

        int index;
        while ((index = __min(top_records, files, MAX_FILES)) != -1)
        {
            if (first_write || top_records[index].product_id != last_written.product_id)
            {
                fwrite(&(top_records[index]), __size, 1, output);
                last_written = top_records[index];
                first_write = false;
            }

            int count = fread(&(top_records[index]), __size, 1, files[index]);
            if (count == 0)
            {
                fclose(files[index]);
                files[index] = NULL;
            }
        }

        first_partition += MAX_FILES;
    }

    fclose(output);
    free(files);
    free(top_records);

    return last_partition;
}