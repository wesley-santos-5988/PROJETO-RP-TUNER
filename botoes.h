// As linhas #ifndef, #define e #endif são obrigatórias em arquivos .h
// Elas impedem que o compilador leia o mesmo cardápio duas vezes por engano.
#ifndef BOTOES_H
#define BOTOES_H

#include <stdint.h>
#include <stdbool.h>

// Estrutura para representar cada corda do violão
typedef struct {
    char nome_nota[3];
    char numero_corda[4];
    float freq_alvo;
} CordaViolao;

// "extern" avisa aos outros arquivos: "Essas variáveis existem em algum lugar!"
extern CordaViolao violao[6];
extern volatile int corda_atual;

// Protótipo da função que o main() vai chamar
void inicializar_botoes(void);

#endif // BOTOES_H