# RP TUNER - Afinador Digital e Detector de Notas com RP2040

![Status](https://img.shields.io/badge/Status-Concluído-success)
![Linguagem](https://img.shields.io/badge/Linguagem-C-blue)
![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry_Pi_Pico_W-red)
![Hardware](https://img.shields.io/badge/Hardware-BitDogLab_v6.3-orange)

> **Projeto Final - Microcontroladores e Microprocessadores** > Universidade Federal do Vale do São Francisco (UNIVASF) - Engenharia Elétrica

---

## 1. Sobre o Projeto
O **RP TUNER** nasceu do desafio de unir o hardware embarcado ao processamento digital de sinais para resolver um problema cotidiano de músicos: a afinação precisa de instrumentos. Desenvolvido para a plataforma Raspberry Pi Pico W (via BitDogLab), o projeto vai além de acender LEDs; ele integra microfones analógicos, matemática complexa e interfaces visuais para criar um afinador de violão (EADGBE) em tempo real. 

O protótipo demonstra que, com o uso de algoritmos robustos de processamento (DSP), é possível extrair dados precisos do mundo físico (som) e traduzi-los em ações interativas, provando a viabilidade do RP2040 para aplicações acústicas.

## 2. Como Funciona o Sistema
O projeto baseia-se em um fluxo contínuo de **Captura, Processamento e Feedback**:
1. **Leitura:** O microfone MAX4466 capta a vibração da corda. O conversor ADC, guiado por Acesso Direto à Memória (DMA), digitaliza esse som a rigorosos 10.000 Hz.
2. **Matemática (Cérebro):** O RP2040 aplica o algoritmo de **Autocorrelação** combinado com **Interpolação Parabólica** para encontrar o padrão de repetição da onda, ignorando ruídos e harmônicos naturais do instrumento.
3. **Interface Visual:** Com a frequência em Hertz calculada, o sistema atualiza um Display OLED via I2C (mostrando a nota e os Hz) e movimenta um ponteiro analógico numa Matriz de LEDs 5x5. Um LED RGB Azul confirma o sucesso ao atingir a afinação exata.

## 3. Hardware e Tecnologias
O mapeamento explorou todos os recursos exigidos pela disciplina de forma síncrona:

| Componente | Pino (GPIO) | Recurso / Protocolo | Função no Sistema |
| :--- | :---: | :---: | :--- |
| **Microfone (MAX4466)** | 28 | `ADC2` + `DMA` | Captura do áudio em tempo real. |
| **Display OLED** | 14 e 15 | `I2C1` | Exibição de dados textuais. |
| **Matriz de LEDs 5x5** | 7 | `PIO` | Ponteiro espacial / Feedback visual. |
| **Botões A e B** | 5 e 6 | `IRQ` + Timer | Navegação manual das cordas com debounce. |
| **LED RGB (Azul)** | 12 | `PWM` | Indicador de afinação concluída. |

## 4. Como Compilar e Executar
1. Clone este repositório para o seu ambiente local.
2. Abra a pasta do projeto no **VS Code** (com a extensão do *Raspberry Pi Pico* instalada e o *Pico SDK* configurado).
3. Na barra inferior do VS Code, clique em **Compile** para compilar o projeto (gerenciado via *CMakeLists*).
4. Conecte a placa BitDogLab via USB segurando o botão **BOOTSEL**.
5. Arraste o arquivo `.uf2` gerado na pasta `build` para o disco removível `RPI-RP2`, ou pressione o botão Run, também presente na barra inferior do VS Code, para transferência automática do arquivo `.uf2`.

## 5. Lições Aprendidas
* **Compreensão de Algoritmo (Autocorrelação):** Implementar a matemática do processamento de áudio foi um grande desafio. Entendemos na prática como a Autocorrelação compara o sinal do violão consigo mesmo em diferentes atrasos temporais para encontrar o padrão fundamental, sendo a melhor forma observada para ignorar os harmônicos que confundem métodos mais simples (como o *Cruzamento por Zero*).
* **Dominância do Hardware:** Descobrimos como extrair o máximo do RP2040 sem travar a CPU principal. Aprendemos a usar o controlador DMA para automatizar a leitura rigorosa do microfone e criamos uma lógica via software para contornar a montagem física em zigue-zague da matriz de LEDs.
* **Programação em C (Do Básico à Modularização):** Como a equipe estava há algum tempo sem contato direto com a linguagem C, o projeto exigiu uma imersão completa. Revisamos desde a sintaxe básica, laços e ponteiros, até chegarmos à estruturação de um *firmware* profissional, dividindo o sistema em múltiplos arquivos de cabeçalho (`.h`) e de implementação (`.c`).

## 6. Equipe de Desenvolvimento
O projeto foi construído de forma colaborativa, onde cada membro contribuiu com seus pontos fortes:
* **Enzo Gonçalves Severo Rocha:** *(Responsável por Firmware Principal e DSP (Processamento de Sinais)).*
* **Maria Gabriela Rodrigues Gomes:** *(Responsável por Validação, Análise de Dados e Documentação Técnica).*
* **Wesley Santos Nunes de Oliveira:** *(Responsável por Periféricos, IHM e Integração de Hardware).*