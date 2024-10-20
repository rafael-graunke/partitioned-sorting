#ifndef _INTERCALATION_H_
#define _INTERCALATION_H_

typedef int (*__min_fn_t) (const void *, FILE **files, int n);

int merge_files(int num_partitions, size_t __size, __min_fn_t __min);
int merge_final_files(int num_partition, int last_partition, size_t __size, __min_fn_t __min);

#endif