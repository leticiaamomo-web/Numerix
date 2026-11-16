
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LARGURA 1100
#define ALTURA 780
#define MAX_TENTATIVAS 10

typedef enum {
    MENU,
    JOGO,
    RESULTADO
} Tela;

typedef struct {
    char numero[5];
    int corretos;
    int deslocados;
} Tentativa;

// Cores do jogo
Color fundo = {15, 18, 35, 255};
Color painel = {26, 31, 55, 255};
Color roxo = {139, 92, 246, 255};
Color azul = {56, 189, 248, 255};
Color branco = {245, 247, 255, 255};
Color cinza = {155, 165, 190, 255};
Color verde = {52, 211, 153, 255};
Color vermelho = {248, 113, 113, 255};

// Variáveis do jogo
Tela tela = MENU;

int limite = 8;
int quantidade = 0;
int venceu = 0;

char senha[5];
char entrada[5] = "";

Tentativa historico[MAX_TENTATIVAS];

// Sorteia quatro números diferentes
void sortearSenha() {

    int usados[10] = {0};
    int i = 0;

    while (i < 4) {

        int numero = rand() % 10;

        if (!usados[numero]) {

            senha[i] = '0' + numero;
            usados[numero] = 1;
            i++;
        }
    }

    senha[4] = '\0';
}

// Desenha um botão
int botao(float x, float y, float largura,
          float altura, const char *texto) {

    Vector2 mouse = GetMousePosition();

    Rectangle area = {
        x, y, largura, altura
    };

    int sobre = CheckCollisionPointRec(mouse, area);

    Color cor = sobre ?
        (Color){110, 75, 220, 255} : roxo;

    DrawRectangleRounded(area, 0.2f, 10, cor);

    int tamanho = MeasureText(texto, 20);

    DrawText(
        texto,
        x + (largura - tamanho) / 2,
        y + (altura - 20) / 2,
        20,
        branco
    );

    return sobre && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// Desenha um painel
void desenharPainel(float x, float y,
                    float largura, float altura) {

    DrawRectangleRounded(
        (Rectangle){x, y, largura, altura},
        0.08f, 10, painel
    );
}

// Reinicia o jogo
void iniciarJogo() {

    quantidade = 0;
    venceu = 0;

    entrada[0] = '\0';

    sortearSenha();

    tela = JOGO;
}

// Verifica a tentativa
void verificarTentativa() {

    int corretos = 0;
    int deslocados = 0;

    for (int i = 0; i < 4; i++) {

        if (entrada[i] == senha[i]) {

            corretos++;
        }
        else {

            for (int j = 0; j < 4; j++) {

                if (entrada[i] == senha[j]) {

                    deslocados++;
                    break;
                }
            }
        }
    }

    // Registra a tentativa
    strcpy(historico[quantidade].numero, entrada);

    historico[quantidade].corretos = corretos;
    historico[quantidade].deslocados = deslocados;

    quantidade++;

    // Verifica vitória
    if (corretos == 4) {

        venceu = 1;
        tela = RESULTADO;
    }
    else if (quantidade >= limite) {

        tela = RESULTADO;
    }

    entrada[0] = '\0';
}

// Adiciona um número à entrada
void adicionarNumero(int numero) {

    if (strlen(entrada) >= 4)
        return;

    char caractere = '0' + numero;

    // Não permite números repetidos
    for (int i = 0; i < strlen(entrada); i++) {

        if (entrada[i] == caractere)
            return;
    }

    int tamanho = strlen(entrada);

    entrada[tamanho] = caractere;
    entrada[tamanho + 1] = '\0';
}

// Tela inicial
void desenharMenu() {

    DrawText("NUMERIX", 390, 100, 65, roxo);

    DrawText(
        "DESAFIO NUMERICO",
        420, 175, 23, azul
    );

    desenharPainel(300, 260, 500, 350);

    DrawText(
        "Descubra a sequencia secreta!",
        355, 295, 25, branco
    );

    DrawText(
        "Encontre os quatro numeros sem repeticao.",
        350, 345, 17, cinza
    );

    DrawText(
        "Quantidade maxima de tentativas:",
        360, 410, 19, branco
    );

    // Controle de tentativas
    if (botao(390, 455, 55, 45, "-")) {

        if (limite > 1)
            limite--;
    }

    char texto[20];

    sprintf(texto, "%d", limite);

    DrawText(texto, 520, 462, 30, branco);

    if (botao(590, 455, 55, 45, "+")) {

        if (limite < MAX_TENTATIVAS)
            limite++;
    }

    if (botao(410, 535, 280, 55, "INICIAR JOGO")) {

        iniciarJogo();
    }

    DrawText(
        "Boa sorte!",
        505, 655, 20, azul
    );
}

// Tela principal do jogo
void desenharJogo() {

    DrawText("NUMERIX", 50, 35, 40, roxo);

    DrawText(
        "DESAFIO NUMERICO",
        55, 82, 16, azul
    );

    char texto[100];

    sprintf(
        texto,
        "Tentativa %d de %d",
        quantidade + 1, limite
    );

    DrawText(texto, 55, 135, 22, branco);

    desenharPainel(45, 180, 620, 550);

    DrawText(
        "DIGITE SUA SEQUENCIA",
        85, 210, 22, branco
    );

    DrawText(
        "Escolha quatro numeros diferentes de 0 a 9.",
        85, 245, 16, cinza
    );

    // Espaços dos quatro números
    for (int i = 0; i < 4; i++) {

        float x = 105 + i * 125;

        DrawRectangleRounded(
            (Rectangle){x, 305, 85, 95},
            0.15f, 10,
            (Color){42, 47, 75, 255}
        );

        if (i < strlen(entrada)) {

            char caractere[2] = {entrada[i], '\0'};

            DrawText(
                caractere,
                x + 26, 325, 45, azul
            );
        }
        else {

            DrawText(
                "_",
                x + 30, 325, 40, cinza
            );
        }
    }

    DrawText(
        "Use o teclado numerico para digitar.",
        150, 425, 17, cinza
    );

    DrawText(
        "ENTER = confirmar | BACKSPACE = apagar",
        110, 455, 15, cinza
    );

    if (botao(190, 510, 300, 55, "CONFIRMAR")) {

        if (strlen(entrada) == 4) {

            verificarTentativa();
        }
    }

    // Histórico
    desenharPainel(690, 180, 365, 550);

    DrawText(
        "HISTORICO",
        730, 210, 25, branco
    );

    DrawText(
        "NUMERO       POS.   DESL.",
        730, 260, 15, azul
    );

    // Exibe todas as tentativas
    for (int i = 0; i < quantidade; i++) {

        int y = 300 + i * 38;

        DrawText(
            historico[i].numero,
            745, y, 19, branco
        );

        sprintf(
            texto, "%d",
            historico[i].corretos
        );

        DrawText(texto, 855, y, 19, verde);

        sprintf(
            texto, "%d",
            historico[i].deslocados
        );

        DrawText(texto, 940, y, 19, azul);
    }
}

// Tela de resultado
void desenharResultado() {

    DrawText("NUMERIX", 390, 65, 55, roxo);

    desenharPainel(250, 160, 600, 480);

    if (venceu) {

        DrawText(
            "PARABENS!",
            415, 205, 38, verde
        );

        DrawText(
            "Voce descobriu a sequencia!",
            350, 270, 23, branco
        );
    }
    else {

        DrawText(
            "FIM DE JOGO!",
            400, 205, 38, vermelho
        );

        DrawText(
            "Suas tentativas acabaram.",
            365, 270, 22, branco
        );
    }

    char texto[100];

    sprintf(
        texto,
        "Sequencia sorteada: %s",
        senha
    );

    DrawText(texto, 370, 345, 25, azul);

    sprintf(
        texto,
        "Tentativas utilizadas: %d de %d",
        quantidade, limite
    );

    DrawText(texto, 370, 395, 21, branco);

    // Análise de desempenho
    if (venceu && quantidade <= limite / 2.0) {

        DrawText(
            "Excelente aproveitamento!",
            365, 450, 22, verde
        );
    }
    else if (venceu) {

        DrawText(
            "Bom aproveitamento! Voce conseguiu!",
            305, 450, 20, azul
        );
    }
    else {

        DrawText(
            "Continue praticando!",
            415, 450, 22, vermelho
        );
    }

    if (botao(390, 535, 320, 55, "JOGAR NOVAMENTE")) {

        tela = MENU;
    }
}

int main() {

    InitWindow(LARGURA, ALTURA, "NUMERIX - Desafio Numerico");

    SetTargetFPS(60);

    srand(time(NULL));

    while (!WindowShouldClose()) {

        // Recebe os números digitados
        if (tela == JOGO) {

            int tecla;

            while ((tecla = GetCharPressed()) > 0) {

                if (tecla >= '0' && tecla <= '9') {

                    adicionarNumero(tecla - '0');
                }
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {

                int tamanho = strlen(entrada);

                if (tamanho > 0) {

                    entrada[tamanho - 1] = '\0';
                }
            }

            if (IsKeyPressed(KEY_ENTER) &&
                strlen(entrada) == 4) {

                verificarTentativa();
            }
        }

        BeginDrawing();

        ClearBackground(fundo);

        if (tela == MENU) {

            desenharMenu();
        }
        else if (tela == JOGO) {

            desenharJogo();
        }
        else if (tela == RESULTADO) {

            desenharResultado();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
