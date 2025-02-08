# Dispositivo display ssd1306

## Descrição
Este projeto tem como objetivo interagir com o periférico display ssd1306 da BitDogLab por meio de comunicação serial via USB do computador.

## Como Executar
1. Clone o repositório:
   ```bash
   git clone https://github.com/Daniel-Alencar/BDL-interface-i2c
   ```
2. Navegue até o diretório do projeto:
   ```bash
   cd BDL-interface-i2c
   ```
3. Abra o projeto no VS Code.
  
4. Compile e execute o projeto usando a placa BitDogLab.

5. Abra o serial monitor do VS Code na porta COM da placa BitDogLab.

6. Envie uma string e envie também o caractere '.' para indicar o final da string. Com isso, a string enviada deve ser processada e exibida no display ssd1306 da BitDogLab.

7. Perceba que no display temos a indicação do estado dos leds verde e azul. Inicialmente, eles começam desligados (off). Para mudar o estado do led verde, clique no botão A. Para mudar o estado do led azul, clique no botão B. Perceba que ao mudar o estado dos leds, o estado atual também é exibido no monitor serial do VS Code.

## Vídeo de Demonstração
```bash
   Link: 
```

## Licença
Este projeto é licenciado sob a licença MIT – consulte o arquivo LICENSE para mais detalhes.

