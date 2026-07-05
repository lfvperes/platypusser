# Platypusser

Jogo estilo Frogger desenvolvido em C com a biblioteca raylib, como trabalho final da disciplina SSC0600/SSC0601 – Introdução à Ciência da Computação I (ICMC-USP, 2026/1).

## Descrição

O jogador controla um ornitorrinco que precisa atravessar uma rua movimentada e um rio para chegar ao outro lado. Na rua, é preciso desviar dos carros. No rio, o jogador deve pular de tronco em tronco — cair na água ou ser pego por um jacaré encerra a partida. Chegar ao topo é a vitória; acertar um chapéu no caminho garante um **BONUS WIN!**

O jogo registra o **top 5 de melhores tempos** (menor tempo = melhor) com nome de 3 letras no estilo arcade, além de um contador persistente de vitórias e derrotas.

## Controles

| Tecla | Ação |
|---|---|
| `↑ ↓ ← →` | Mover o personagem |
| `Enter` | Iniciar / reiniciar o jogo |

**Entrada de nome após vitória:**

| Tecla | Ação |
|---|---|
| `↑ ↓` | Mudar a letra atual |
| `→` / `Enter` | Confirmar letra / avançar |
| `←` | Voltar uma letra |

## Conceitos implementados

- **Estruturas de controle:** `if`, `else`, `while`, `for` — laço principal do jogo, detecção de colisões, spawning de NPCs
- **Typedefs e structs:** `character`, `NpcData`, `lane`, `Score`, `Stats`
- **Alocação dinâmica:** `malloc`, `realloc`, `free` — vetores de carros, jacarés e troncos crescem e encolhem em tempo real
- **Vetores e strings:** vetor de texturas, vetor de pontuações, nome do jogador (`char[4]`)
- **Funções:** passagem por valor e por referência (ponteiros), funções estáticas auxiliares
- **Manipulação de arquivos:** `scores.txt` (top 5 tempos) e `stats.txt` (vitórias/derrotas) — lidos na inicialização e escritos a cada fim de partida

## Ambiente

- **SO:** Fedora Linux (KDE)
- **Compilador:** GCC
- **Biblioteca:** raylib
- **Editor:** VS Code

## Dependências

Instale o raylib antes de compilar. No Fedora:

```bash
sudo dnf install raylib-devel
```

No Ubuntu/Debian:

```bash
sudo apt install libraylib-dev
```

## Como compilar e executar

```bash
# Compilar
make

# Executar
./platypusser

# Ou em um único passo
make run

# Limpar o executável
make clean
```

O comando completo de compilação gerado pelo Makefile é:

```bash
gcc -o platypusser src/*.c -Iinclude -lraylib -lm
```

## Estrutura do projeto

```
platypusser/
├── assets/          # Texturas do jogo (PNG)
├── include/         # Cabeçalhos (.h)
│   ├── character.h
│   ├── game.h
│   ├── lane.h
│   ├── river.h
│   ├── scores.h
│   └── street.h
├── src/             # Código-fonte (.c)
│   ├── main.c
│   ├── game.c
│   ├── character.c
│   ├── lane.c
│   └── scores.c
├── Makefile
└── README.md
```

Os arquivos `scores.txt` e `stats.txt` são criados automaticamente na pasta do executável após a primeira partida.

## Demonstração

> 🎥 Vídeo em breve.

## Repositório

https://github.com/lfvperes/platypusser