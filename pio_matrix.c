#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "display/ssd1306.c"
#include "led_matrix/led_matrix.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Seleciona a UART0
#define UART_ID uart0 
// Define a taxa de transmissão
#define BAUD_RATE 115200 
#define UART_TX_PIN 0 // Pino GPIO usado para TX
#define UART_RX_PIN 1 // Pino GPIO usado para RX

void extract_numbers(const char *str, int *numbers, int *count) {
  // Inicializa a contagem de números encontrados
  *count = 0;

  while (*str) {
    if (isdigit(*str)) {
      // Converte char para inteiro
      numbers[*count] = *str - '0';
      (*count)++;
    }
    str++;
  }
}

int main() {
  // Inicializa a biblioteca padrão
  stdio_init_all();

  setup_led_matrix();

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);
  // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
  // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); 
  // Pull up the data line
  gpio_pull_up(I2C_SDA); 
  // Pull up the clock line
  gpio_pull_up(I2C_SCL); 
  // Inicializa a estrutura do display
  ssd1306_t ssd; 
  // Inicializa o display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); 
  // Configura o display
  ssd1306_config(&ssd); 
  // Envia os dados para o display
  ssd1306_send_data(&ssd); 

  bool cor = true;

  // Limpa o display
  ssd1306_fill(&ssd, !cor);
  // Desenha um retângulo
  ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);  
  // Desenha uma string
  ssd1306_draw_string(&ssd, "Iniciando...", 8, 10);
  ssd1306_send_data(&ssd);

  // Buffer para armazenar a mensagem
  char buffer[100];  
  // Índice para armazenar caracteres no buffer
  int index = 0;     

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
            draw(numbers[numbersInString[0]]);
          }

          // Reinicia o índice para a próxima mensagem
          index = 0; 

          // Exibe a mensagem recebida
          printf("Mensagem recebida: %s\n", buffer);

          // Limpa o display
          ssd1306_fill(&ssd, !cor);
          // Desenha um retângulo
          ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);  
          // Desenha uma string
          ssd1306_draw_string(&ssd, buffer, 8, 10);
          // Atualiza o display 
          ssd1306_send_data(&ssd);
        }
      }
    }
    sleep_ms(50);
  }
}