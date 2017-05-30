#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv)
{
    int me, npes, nid, cpu, x, y, z;
    char hostname[64];
    char my_string[128];
    char timebuf [26];
    char *all_strings;
    time_t timer;
    struct tm* tm_info;
    int i;

    MPI_Init(&argc, &argv);

    // Get my pe and num pes
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // Print time and nodes for first rank
    if (!me) {
        time(&timer);
        tm_info = localtime(&timer);
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s: starting on %s\n", timebuf, getenv("SLURM_NODELIST"));
    }

    // Get my cpu
    cpu = sched_getcpu();

    // Get my hostname
    gethostname(hostname, sizeof(hostname));

    // Generate my info string
    sprintf(my_string, "%6i: %s cpu %2i\n",
            me, hostname, cpu);


    // Rank 0 allocates memory to gather all info strings
    if (!me) {
        size_t size = npes * sizeof(my_string);
        all_strings = malloc(size);
        if (!all_strings) {
            printf("ERROR: malloc(%lu) failed\n", size);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Gather all info strings to rank 0
    MPI_Gather(my_string, sizeof(my_string), MPI_CHAR,
               all_strings, sizeof(my_string), MPI_CHAR,
               0, MPI_COMM_WORLD);

    // Rank 0 prints out all info strings in sorted order by pe
    if (!me) {
        for (i = 0; i < npes; i++) {
            printf("%s", &all_strings[i * sizeof(my_string)]);
        }
    }

    // Sleep 10 seconds
    sleep(10);
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Print finish time
    if (!me) {
        time(&timer);
        tm_info = localtime(&timer);
        strftime(timebuf, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s: finished\n", timebuf);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
