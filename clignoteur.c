#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


//  clignoteur sur le gpio 21
//  gcc -o clignoteur clignoteur.c -lgpiod


// --- Configuration via Directives ---
#define GPIO_CHIP "gpiochip0"  // Puce GPIO par défaut sur Pi 3
#define GPIO_LINE 21           // Numéro BCM du GPIO
#define MIN_MS 10             // Temps mini (0.1s)
#define MAX_MS 1000            // Temps maxi (2s)
#define CHANGE_PERIOD_SEC 10   // Changer de rythme toutes les 30s
#define CONSUMER "clignoteur_aleatoire"

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int val = 0;
    time_t last_change_time, current_time;
    int current_delay_ms;

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Ouverture de la puce GPIO
    chip = gpiod_chip_open_lookup(GPIO_CHIP);
    if (!chip) {
        perror("Erreur : Impossible d'ouvrir le gpiochip");
        return 1;
    }

    // Récupération de la ligne (GPIO)
    line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (!line) {
        perror("Erreur : Impossible de récupérer la ligne");
        gpiod_chip_close(chip);
        return 1;
    }

    // Configuration de la ligne en sortie
    if (gpiod_line_request_output(line, CONSUMER, 0) < 0) {
        perror("Erreur : Impossible de configurer le GPIO en sortie");
        gpiod_chip_close(chip);
        return 1;
    }

    // Initialisation du premier délai
    current_delay_ms = (rand() % (MAX_MS - MIN_MS + 1)) + MIN_MS;
    last_change_time = time(NULL);

    printf("Clignotement sur GPIO %d démarré.\n", GPIO_LINE);
    printf("Rythme initial : %d ms\n", current_delay_ms);

    while (1) {
        current_time = time(NULL);

        // Vérifier si 30 secondes se sont écoulées
        if (difftime(current_time, last_change_time) >= CHANGE_PERIOD_SEC) {
            current_delay_ms = (rand() % (MAX_MS - MIN_MS + 1)) + MIN_MS;
            last_change_time = current_time;
            printf("Changement de rythme ! Nouveau délai : %d ms\n", current_delay_ms);
        }

        // Basculer l'état (Toggle)
        val = !val;
        gpiod_line_set_value(line, val);

        // Attendre (conversion ms en microsecondes)
        usleep(current_delay_ms * 1000);
    }

    // Libération (non atteint ici car boucle infinie)
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
