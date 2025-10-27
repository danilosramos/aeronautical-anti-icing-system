# Sistema Aeronáutico Antigelo (Projeto de Sistemas Embarcados)

Este é um projeto de **Sistemas Embarcados** que simula o funcionamento de um sistema de degelo (antigelo) para aeronaves. O projeto integra hardware e software, demonstrando a aplicação de lógica de controle em um ambiente simulado.

## Conceitos Demonstrados

*   **Sistemas Embarcados:** Projeto que envolve a interação entre software (código C para microcontrolador) e hardware (simulação de circuito).
*   **Programação em C para Microcontroladores:** O software é escrito em C e destinado a ser executado em um microcontrolador (possivelmente PIC, dado o contexto dos arquivos de projeto `Sistema Antigelo.X`).
*   **Simulação de Hardware:** Utilização do software **Proteus** para simular o circuito eletrônico e o microcontrolador, permitindo testar o código em um ambiente virtual.
*   **Lógica de Controle:** Implementação de um algoritmo que monitora condições (como temperatura e umidade) e ativa o sistema de degelo conforme a lógica de controle definida.
*   **Comunicação Periférica:** Uso de um display LCD (`lcd.h`, `main.c`) para exibir o status do sistema.

## Estrutura do Projeto

O projeto está dividido em duas partes principais:

1.  **Software (`Sistema Antigelo.X`):** Contém o código-fonte em C (`main.c`, `lcd.h`) e os arquivos de projeto do ambiente de desenvolvimento (provavelmente MPLAB X).
2.  **Hardware/Simulação (Arquivos `.pdsprj`):** Contém o diagrama esquemático e a simulação do circuito eletrônico no Proteus.

## Como Visualizar e Analisar

Para visualizar o projeto completo, é necessário o software **Proteus ISIS** (para o circuito) e o ambiente de desenvolvimento **MPLAB X** (para o código C).

**Recomendação para Recrutadores:** O valor deste projeto reside na **lógica implementada no arquivo `main.c`** e no conceito do sistema. A análise do código C demonstra a capacidade de escrever software de baixo nível para controle de hardware.

---
**Linguagem:** C (Software), Proteus (Simulação de Hardware)
**Tópicos:** Sistemas Embarcados, Microcontroladores (PIC/AVR), Lógica de Controle, Simulação de Hardware, Programação de Baixo Nível.
'''
