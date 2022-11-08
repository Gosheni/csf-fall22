#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  // Create temparr[0..n1] ← A[p..q] and temparr[n1..n1+n2] M ← A[q+1..r]
  int n1 = mid - begin + 1;
  int n2 = end - mid;

  for (int i = 0; i < n1; i++)
    temparr[i] = arr[begin + i];
  for (int j = 0; j < n2; j++)
    temparr[n1 + j] = arr[mid + 1 + j];

  // Maintain current index of sub-arrays and main array
  int i = 0, j = 0, k = begin;

  // Until we reach the end of either left or right of temparr, pick larger among the two elements and place them in arr
  while (i < n1 && j < n2) {
    if (temparr[i] <= temparr[n1 + j]) {
      arr[k] = temparr[i++];
    } else {
      arr[k] = temparr[n1 + j];
      j++;
    }
    k++;
  }

  // When we run out of elements in either left or right of temparr, pick up the remaining elements and put in arr
  while (i < n1) {
    arr[k++] = temparr[i++];
  }
  while (j < n2) {
    arr[k++] = temparr[n1 + j];
    j++;
  }
}

int cmpfunc(const void * a, const void * b) {
  int64_t left = *(int64_t*)a;
  int64_t right = *(int64_t*)b;
  if (left < right) return -1;
  else if (left > right) return 1;
  return 0;
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  size_t size = end-begin;
  if (size <= threshold) {
    qsort(arr+begin, size, sizeof(int64_t), cmpfunc);
  } else if (begin <= end) {
    // Same as (l+r)/2, but avoids overflow for
    // large l and h
    int m = (begin + end) / 2;
 
    // Sort first and second halves
    pid_t pid = fork();
    if (pid == -1) {
      fprintf(stderr, "Error: Fork failed to start a new process\n");
      //Error
      exit(1);
    } else if (pid == 0) {
      merge_sort(arr, begin, m, threshold);
      exit(0);
    }
    int wstatus;
    pid_t actual_pid = waitpid(pid, &wstatus, 0);
    if (actual_pid == -1) {
      fprintf(stderr, "Error: Waitpid failure\n");
      //Error
      exit(1);
    }
    if (!WIFEXITED(wstatus)) {
      fprintf(stderr, "Error: Subprocess crashed, was interrupted, or didn't exit normally\n");
      //Error
      exit(1);
    }
    if (!WEXITSTATUS(wstatus) != 0) {
      fprintf(stderr, "Error: Subprocess returned a non-zero exit code\n");
      //Error
      exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
      fprintf(stderr, "Error2: Fork failed to start a new process\n");
      //Error
      exit(1);
    } else if (pid2 == 0) {
      merge_sort(arr, m + 1, end, threshold);
      exit(0);
    }
    int wstatus2;
    pid_t actual_pid2 = waitpid(pid2, &wstatus2, 0);
    if (actual_pid2 == -1) {
      fprintf(stderr, "Error2: Waitpid failure\n");
      //Error
      exit(1);
    }
    if (!WIFEXITED(wstatus2)) {
      fprintf(stderr, "Error2: Subprocess crashed, was interrupted, or didn't exit normally\n");
      //Error
      exit(1);
    }
    if (!WEXITSTATUS(wstatus2) != 0) {
      fprintf(stderr, "Error2: Subprocess returned a non-zero exit code\n");
      //Error
      exit(1);
    }
 
    int64_t *temparr = malloc(sizeof *temparr);
    merge(arr, begin, m, end, temparr);
    free(temparr);
  }
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])){
    /* TODO: report an error (threshold value is invalid) */;
    fprintf(stderr, "Error: threshold value is invalid\n");
    return -1;
  }

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    // file couldn't be opened: handle error and exit
    fprintf(stderr, "Error: file couldn't be opened\n");
    return -1;
  }

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    // handle fstat error and exit
    fprintf(stderr, "Error: fstat cannot determine size of file\n");
    return -1;
  }
  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data == MAP_FAILED) {
    // handle mmap error and exit
    fprintf(stderr, "Error: fail to mmap the file data\n");
    return -1;
  }

  // TODO: sort the data!
  merge_sort(data, 0, file_size_in_bytes/sizeof(int64_t), threshold);

  // TODO: unmap and close the file
  int unmapSuccess = munmap(data, file_size_in_bytes);
  int closeSuccess = close(fd);
  if(unmapSuccess != 0 || closeSuccess != 0){
    //handle unmap & close error and exit
    fprintf(stderr, "Error: fail to handle unmap and close\n");
    return -1;
  }

  // TODO: exit with a 0 exit code if sort was successful
  return 0;
}