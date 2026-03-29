# Trabalho 1 - Computação Gráfica

Realizado por:

- Felipe Carneiro Machado - 14569373
- Clara Ernesto de Carvalho - 14559479

O trabalho consiste em uma cena interativa simples do personagem Link, da franquia The Legend of Zelda, com sua espada, escudo, a Triforce e uma Rúpia.

## Estrutura de diretórios e Seleção de ferramentas

O presente trabalho foi realizado com a biblioteca OpenGL (conforme especificação) na linguagem C++ (CMake como build tool). Foi utilizado Python (ambiente uv) para gerar as malhas dos objetos

A estrutura de diretórios é mostrada a seguir:

```
.
├── cmake-build-debug 
├── cmake-build-release # Diretorios do CMake
├── dependencies # Dependencias externas (glad, glfw, glm)
├── grid_scripts # Scritps python para geracao de malhas
├── include # Headers
├── objects # Arquivos wavefront .obj
├── shaders # Shaders GLSL
└── src # Codigo fonte
```


## Dependências

Para executar o código é necessário ter o CMake, make e um compilador C++ instalados, além do OpenGL. As dependencias externas (glad, glfw e glm) estão inclusas no projeto, não sendo necessária instalação. 

Caso seja de interesse executar os scripts de geração de malhas, é necessário ter o gerenciador de pacotes uv instalado.

## Instruções de uso

Para compilar o código, basta executar o comando (assumindo ambiente Linux):
 ```bash
 cmake . && make
 ```
Para executar o binário compilado:

```bash
./cmake-build-debug/Graphics
```

## Instruções do programa

A cena está equipada com as seguintes interações:

- Teclas A/D: Movem Link e a Rúpia para a esquerda/direita
- Teclas W/S: Movem a Triforce e a Rúpia para cima/baixo
- Teclas Q/E: Rotacionam a Triforce e a Rúpia para esquerda/direita
- Teclas H/J: Encolhe/Aumenta a Espada.
- Tecla P: Alterna entre visualização de Polígonos cheios ou da Malha poligonal


