/*
    Hebert Costa Vaz Santana
    TIC370101235
    Grupo 7

    Semáforo com temporizador usando a função: add_repeating_timer_ms()
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos GPIOs para os LEDs
#define LED_RED 13
#define LED_YELLOW 12
#define LED_GREEN 11

// Variável global para controlar o estado do semáforo
typedef enum {
    RED,
    YELLOW,
    GREEN
} TrafficLightState;

TrafficLightState current_state = RED;

// Função para desligar todos os LEDs
void turn_off_all_leds() {
    gpio_put(LED_RED, 0);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);
}

// Função de callback para o temporizador
bool timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs antes de mudar o estado
    turn_off_all_leds();

    // Altera o estado do semáforo
    switch (current_state) {
        case RED:
            gpio_put(LED_YELLOW, 1); // Liga o LED amarelo
            current_state = YELLOW;
            break;
        case YELLOW:
            gpio_put(LED_GREEN, 1); // Liga o LED verde
            current_state = GREEN;
            break;
        case GREEN:
            gpio_put(LED_RED, 1); // Liga o LED vermelho
            current_state = RED;
            break;
    }

    return true; // Mantém o temporizador ativo
}

int main() {
    // Inicializa a comunicação serial (para imprimir informações)
    stdio_init_all();

    // Configura os GPIOs dos LEDs como saída
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Desliga todos os LEDs no início
    turn_off_all_leds();

    // Como não recebe nenhum comando... Inicia logo no vermelho ativado

    gpio_put(LED_RED, 1);

    // Configura o temporizador para chamar a função de callback a cada 3000ms
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, timer_callback, NULL, &timer);

    // Loop principal
    while (true) {
        // Imprime o estado atual dos LEDs a cada 1000ms
        switch (current_state) {
            case RED:
                printf("Vermelho Ligado / Amarelo e Verde Desligados\n");
                break;
            case YELLOW:
                printf("Amarelo Ligado / Vermelho e Verde Desligados\n");
                break;
            case GREEN:
                printf("Verde Ligado / Vermelho e Amarelo Desligados\n");
                break;
        }

        sleep_ms(1000); // Espera 1 segundo para reiniciar o ciclo
    }

    return 0;
}