#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#include "datatypes/struct.h"
#include "functions/print.h"
#include "functions/helper.h"
#include "functions/initialization.h"
#include "functions/pcie_control.h"
#include "functions/wave_generator_pcie.h"
#include "functions/input.h"
#include "functions/timer.h"
#include "functions/logging.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
FILE *fp;
double file_data[10][3]; // read maximum 10 rows
struct Wave wave;

timer_t timerid;
struct itimerspec timer;
int count_down = 10;

WINDOW *win;
int y_min, x_min, y_max, x_max;

void termination_signal_handler(int signum) {
//    printf("Program terminated.\n");
    delwin(win);
    exit(0);
}

void timer_signal_handler(int signum) {
//    printf("Count down: %d sec\n", count_down);
    count_down--;
}

int main(int argc, char **argv) {
    int wave_index, wave_count;
    int ran_by_file = 0;

    // CMake path, use different path to run from different directory.
//    DisplayTitle("assets/title.txt");

    signal(SIGINT, termination_signal_handler);
    signal(SIGALRM, timer_signal_handler);

    wave_count = WaveInitialization(argc, argv);
    PCIeInitialization();
    DIOInitialization();
    ncursesInitialization();

    if (timer_create(CLOCK_REALTIME, NULL, &timerid) == -1) {
        Error_CannotCreateTimer();
        exit(1);
    }

    if (wave_count > 1) {
        ran_by_file = 1;
        wave_count--;
    }

    for (wave_index = 0; wave_index < wave_count; wave_index++) {
        if (ran_by_file) {
            WaveInitializationByFile(wave_index);
        }

//        printf("Running the program...\n\n");

        TimerInitialization();

        pthread_create(NULL, NULL, &ReadSwitch, NULL);
        pthread_create(NULL, NULL, &ReadPot, NULL);
        pthread_create(NULL, NULL, &ReadArrowKey, NULL);
        pthread_create(NULL, NULL, &UpdateTimer, NULL);
        pthread_create(NULL, NULL, &GenerateWave, NULL);

        while (1) {
            if (count_down == 0) {
                break;
            }
        }

        count_down = 10;
    }

//    printf("Program ended.\n");
    if (ran_by_file) fclose(fp);
    timer_delete(timerid);
    return 0;
}
