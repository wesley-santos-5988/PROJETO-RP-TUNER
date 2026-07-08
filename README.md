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

> **📄 Relatório Técnico:** A fundamentação teórica completa, detalhamento matemático e fluxogramas encontram-se no arquivo [`Relatorio_Final_RPTUNER.pdf`](./Relatorio_Final_RPTUNER.pdf) na raiz deste repositório.

## 2. Como Funciona o Sistema
O projeto baseia-se em um fluxo contínuo de **Captura, Processamento e Feedback**:
1. **Leitura:** O microfone MAX4466 capta a vibração da corda. O conversor ADC, guiado por Acesso Direto à Memória (DMA), digitaliza esse som a rigorosos 10.000 Hz.
2. **Matemática (Cérebro):** O RP2040 aplica o algoritmo de **Autocorrelação** combinado com **Interpolação Parabólica** para encontrar o padrão de repetição da onda, ignorando ruídos e harmônicos naturais do instrumento.
3. **Interface Visual:** Com a frequência em Hertz calculada, o sistema atualiza um Display OLED via I2C (mostrando a nota e os Hz) e movimenta um ponteiro analógico numa Matriz de LEDs 5x5. Um LED RGB Azul confirma o sucesso ao atingir a afinação exata.

## 3. Lições Aprendidas
Durante o desenvolvimento, enfrentamos desafios que nos trouxeram grandes aprendizados práticos de engenharia:
* **Evolução de Algoritmos:** Iniciamos com o método de *Cruzamento por Zero*, mas descobrimos na prática que os harmônicos do violão causavam leituras falsas. A migração para a *Autocorrelação* foi um salto de maturidade no projeto.
* **Dominância do Hardware:** Aprender a usar o DMA para não sobrecarregar a CPU e contornar a ligação elétrica em zigue-zague da matriz de LEDs usando lógica via software.
* **Modularização em C:** Como estruturar e organizar bibliotecas (`.c` e `.h`) para manter o código limpo e escalável.

## 4. Hardware e Tecnologias
O mapeamento explorou todos os recursos exigidos pela disciplina de forma síncrona:

| Componente | Pino (GPIO) | Recurso / Protocolo | Função no Sistema |
| :--- | :---: | :---: | :--- |
| **Microfone (MAX4466)** | 28 | `ADC2` + `DMA` | Captura do áudio em tempo real. |
| **Display OLED** | 14 e 15 | `I2C1` | Exibição de dados textuais. |
| **Matriz de LEDs 5x5** | 7 | `PIO` | Ponteiro espacial / Feedback visual. |
| **Botões A e B** | 5 e 6 | `IRQ` + Timer | Navegação manual das cordas com debounce. |
| **LED RGB (Azul)** | 12 | `PWM` | Indicador de afinação concluída. |

## 5. Como Compilar e Executar
1. Clone este repositório para o seu ambiente local.
2. Abra a pasta do projeto no **VS Code** (com a extensão do *Raspberry Pi Pico* instalada e o *Pico SDK* configurado).
3. Na barra inferior do VS Code, clique em **Build** para compilar o projeto (gerenciado via *CMakeLists*).
4. Conecte a placa BitDogLab via USB segurando o botão **BOOTSEL**.
5. Arraste o arquivo `.uf2` gerado na pasta `build` para o disco removível `RPI-RP2`.

## 6. Demonstração
* **Vídeo do Projeto em Ação:** [Link para o YouTube aqui]
* **Simulação Interativa (Wokwi):** Carregue o arquivo `diagram.json` na plataforma Wokwi para testar os periféricos e a lógica de telas de forma virtual.

## 7. Equipe de Desenvolvimento
O projeto foi construído de forma colaborativa, onde cada membro contribuiu com seus pontos fortes:
* **Enzo Gonçalves Severo Rocha:** *(Coloque aqui 1 ou 2 funções, ex: Desenvolvimento da lógica de hardware, documentação técnica).*
* **Maria Gabriela Rodrigues Gomes:** *(Coloque aqui 1 ou 2 funções, ex: Estruturação dos algoritmos matemáticos, design do sistema).*
* **Wesley Santos Nunes de Oliveira:** *(Coloque aqui 1 ou 2 funções, ex: Integração dos periféricos, simulações computacionais).*