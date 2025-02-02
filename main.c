#include <stdio.h>
#include "cli/cli_interface.h"

int main(){
    printf("Choose Interface:\n1. CLI\n2. Web\n");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        start_cli_game();
    } else if (choice == 2) {
        printf("NOT IMPLEMENTED YET!\n");
    }

    return 0;
}