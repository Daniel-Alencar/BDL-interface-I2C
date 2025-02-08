#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "util/util.h"
#include "display/ssd1306.c"
#include "led_matrix/led_matrix.h"
#include "buttons/buttons.h"
#include "led_rgb/led_rgb.h"

// Seleciona a UART0
#define UART_ID uart0 
// Define a taxa de transmissão
#define BAUD_RATE 115200 
#define UART_TX_PIN 0 // Pino GPIO usado para TX
#define UART_RX_PIN 1 // Pino GPIO usado para RX

// Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time = 0;
static volatile bool state_led_green = false;
static volatile bool state_led_blue = false;
static volatile char message_led_green[100];
static volatile char message_led_blue[100];
// Função de interrupção para funcionamento dos botões
static void gpio_irq_handler(uint gpio, uint32_t events);

int main() {
  bool color = true;
  sprintf(message_led_green, "Green led %s\n", state_led_green ? "on" : "off");
  sprintf(message_led_blue, "Blue led %s\n", state_led_blue ? "on" : "off");

  // Inicializa a biblioteca padrão
  stdio_init_all();

  setup_led_matrix();
  setup_display_oled();
  setup_buttons();
  setup_led_RGB();

  sleep_ms(3000);
  
  printf("display\n");
  // Limpa o display
  display_fill(!color);
  // Desenha um retângulo
  display_draw_rectangle(3, 3, 122, 58, !color, color);  
  // Desenha uma string
  display_draw_string("Welcome...", 8, 10);
  display_draw_string(message_led_green, 8, 40);
  display_draw_string(message_led_blue, 8, 55);
  display_send_data();


  // Buffer para armazenar a mensagem
  char buffer[100];  
  // Índice para armazenar caracteres no buffer
  int index = 0;

  // Interrupções configuradas
  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

  while (true) {

    char character;

    // Verifica se há dados disponíveis para leitura
    if(stdio_usb_connected()) {
      if(scanf("%c", &character) == 1) {
        printf("Caractere: %c\n", character);

        char string[50];
        sprintf(string, "%c", character);

        if(character != '.') {
          buffer[index++] = character;

          // Garante que o índice não ultrapasse o tamanho do buffer
          if (index >= sizeof(buffer) - 1) {
            // Reinicia o índice se o buffer estiver cheio
            index = 0; 
          }

        } else {
          // Termina a string e processa a mensagem
          buffer[index] = '\0';
          int count;
          int numbersInString[40];
          extract_numbers(buffer, numbersInString, &count);

          if(count > 0) {
            int i;
            for(i = 0; i < count; i++) {
              draw(numbers[numbersInString[i]]);
            }
          }

          // Reinicia o índice para a próxima mensagem
          index = 0; 

          // Exibe a mensagem recebida
          printf("Mensagem recebida: %s\n", buffer);

          // Limpa o display
          display_fill(!color);
          // Desenha um retângulo
          display_draw_rectangle(3, 3, 122, 58, !color, color);  
          // Desenha uma string
          display_draw_string(buffer, 8, 10);
          display_draw_string(message_led_green, 8, 40);
          display_draw_string(message_led_blue, 8, 55);
          // Atualiza o display 
          display_send_data();
        }
      }
    }
    sleep_ms(50);
  }
}

void gpio_irq_handler(uint gpio, uint32_t events) {
  // Obtém o tempo atual em microssegundos
  uint32_t current_time = to_us_since_boot(get_absolute_time());

  // Verifica se passou tempo suficiente desde o último evento
  // 200 ms de debouncing
  if (current_time - last_time > 200000) 
  {   
      // Atualiza o tempo do último evento
      last_time = current_time;

      // Faz as funcionalidades dos botões
      if(gpio == BUTTON_A) {
        state_led_green = !state_led_green;
        set_green(state_led_green);
        sprintf(message_led_green, "Green led %s\n", state_led_green ? "on" : "off");
        printf(message_led_green);
        // Desenha uma string
        display_draw_string(message_led_green, 8, 40);
        // Atualiza o display 
        display_send_data();
        
      } else if(gpio == BUTTON_B) {
        state_led_blue = !state_led_blue;
        set_blue(state_led_blue);
        sprintf(message_led_blue, "Blue led %s\n", state_led_blue ? "on" : "off");
        printf(message_led_blue);
        // Desenha uma string
        display_draw_string(message_led_blue, 8, 55);
        // Atualiza o display 
        display_send_data();

      }
  }
}