#include "indicator.h"
#include "git.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *listen_status(void *ptr);

typedef struct str_thdata
{
        // parameters
} thdata;

int main (int argc, char **argv)
{
        pthread_t th_gitstatus;
        thdata data;
        int th_gitstatus_return;

        gtk_init (&argc, &argv);

        init_ui();

        th_gitstatus_return = pthread_create (&th_gitstatus, NULL,
                                        (void *) &listen_status,
                                        (void *) &data);
        if (th_gitstatus_return) {
                fprintf (stderr, "Thread failed: %d\n", th_gitstatus_return);
                exit (EXIT_FAILURE);
        }

        pthread_join (th_gitstatus, NULL);

        gtk_main ();

        return 0;
}

void* listen_status (void *ptr)
{
        thdata *data;
        data = (thdata *) ptr;
        struct git g;

        git_threads_init();

        parse_options(&g);
        open_repository (&g);
        get_status (&g);
        close_repository (&g);

        git_threads_shutdown();
}
