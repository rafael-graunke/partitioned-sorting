#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "partitioning.h"
#include "intercalation.h"


int cmp_categories(const void *arg1, const void *arg2)
{
    return ((CategoryEntry *)arg1)->category_id - ((CategoryEntry *)arg2)->category_id;
}

int cmp_products(const void *arg1, const void *arg2)
{
    return ((ProductEntry *)arg1)->product_id - ((ProductEntry *)arg2)->product_id;
}

int cmp_session(const void *arg1, const void *arg2)
{
    return strcmp(((SessionEntry *)arg1)->user_session, ((SessionEntry *)arg2)->user_session);
}

int min_products(const void *products, FILE **files, int n)
{
    int index = -1;
    ProductEntry min;
    for (int i = 0; i < n; i++)
        if(files[i] != NULL)
        {
            min = ((ProductEntry *) products)[i];
            index = i;
            break;
        }

    for (int i = 0; i < n; i++)
    {
        if(files[i] != NULL && ((ProductEntry *) products)[i].product_id < min.product_id)
        {
            index = i;
            min = ((ProductEntry *) products)[i];
        }
    }

    return index;
}


int main(void) {

    //1 - Convert the CSV File for smaller binary files 
    // FILE *file = fopen("input/dataset.csv", "r");
    // generate_tables(file);


    //2 - Generate  
    // FILE *source = fopen("output/products.bin", "rb");
    // int num_product_partitions = partition(source, sizeof(ProductEntry), cmp_products);
    // int last_product_partition = merge_files(46, sizeof(ProductEntry), min_products);

    merge_final_files(46, 58, sizeof(ProductEntry), min_products);

    // merge_final_files(num_product_partitions, last_product_partition, sizeof(ProductEntry), min_products);
}
