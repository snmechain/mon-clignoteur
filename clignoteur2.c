#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_CHIP "gpiochip0"
#define GPIO_LINE 21
#define CONSUMER "clignoteur_cmd"

int main(int argc, char *argv[]) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int val = 0;
    int delay_ms;

    // 1. Vérification de l'argument
    if (argc < 2) {
        printf("Usage: %s <delai_en_ms>\n", argv[0]);
        printf("Exemple: %s 500 (pour un flash toutes les 0.5s)\n", argv[0]);
        return 1;
    }

    // 2. Conversion de l'argument (chaîne vers entier)
    delay_ms = atoi(argv[1]);
    if (delay_ms <= 0) {
        printf("Erreur : Le délai doit être un nombre entier positif.\n");
        return 1;
    }

    // 3. Initialisation du GPIO
    chip = gpiod_chip_open_lookup(GPIO_CHIP);
    if (!chip) {
        perror("Erreur d'ouverture du chip");
        return 1;
    }

    line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (gpiod_line_request_output(line, CONSUMER, 0) < 0) {
        perror("Erreur de configuration du GPIO");
        gpiod_chip_close(chip);
        return 1;
    }

    printf("Clignotement démarré sur GPIO %d avec un délai de %d ms.\n", GPIO_LINE, delay_ms);
    printf("Appuyez sur Ctrl+C pour arrêter.\n");

    // 4. Boucle de clignotement continue
    while (1) {
        val = !val;
        gpiod_line_set_value(line, val);
        usleep(delay_ms * 1000); // Conversion ms en microsecondes
    }

    // Libération des ressources (théorique)
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
