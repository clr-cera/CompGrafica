# Trabalho 2 - Computação Gráfica

Realizado por:

- Felipe Carneiro Machado - 14569373
- Clara Ernesto de Carvalho - 14559479

O trabalho consiste em uma cena interativa temática de Bob Esponja, a qual contém um cenário externo na fenda do biquini e um cenário interno na casa do Bob Esponja.

## Estrutura de diretórios e Seleção de ferramentas

O presente trabalho foi realizado com a biblioteca OpenGL (conforme especificação) na linguagem C++ (CMake como build tool). Foi utilizado Python (ambiente uv) para gerar as malhas dos objetos

A estrutura de diretórios é mostrada a seguir:

```
.
├── dependencies # Dependencias externas (glad, glfw, glm, stb_image)
├── include # Headers
├── objects # Arquivos wavefront .obj
├── shaders # Shaders GLSL
└── src # Codigo fonte
```


## Dependências

Para executar o código é necessário ter o CMake, make e um compilador C++ instalados, e uma placa de vídeo com driver compatível com OpenGL. As dependencias externas (glad, glfw e glm e stb_image) estão inclusas no projeto, não sendo necessária instalação. 

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

- Teclas W/A/S/D: Move a câmera para a frente, esquerda, atrás e direita
- Tecla Espaço: A câmera realiza um pulo, isso pode ser executado no ar para executar saltos consecutivos
- Teclas X/C: Move o Gary de um lado para o outro no cenário externo.
- Teclas Q/E: Rotaciona o Bob Esponja no eixo vertical no sentido anti-horário e horário
- Teclas J/K: Aumenta/Encolhe as águas vivas
- Tecla P: Alterna entre visualização de Polígonos cheios ou da Malha poligonal


## Descrição do cenário

O programa possui uma sky-box e limita a câmera a apenas interagir dentro dos limites do terreno disponível. Além disso, possui uma área externa e interna.

### Área Externa

Na área externa consiste em um chão que parece a entrada de uma casa, e existem 4 modelos diferentes:

- O Gary, que se move horizontalmente de um lado ao outro do terreno segundo comandos do usuário. 
- O Patrick, que sempre se vira para a câmera.
- Os peixes, que se movimentam segundo a simulação de grupo de boids.
- As águas vivas, que se movimentam ao redor do terreno, e podem ser aumentadas e encolhidas conforme comandos do usuário.

### Área Interna

A área interna consiste no interior da casa do Bob Esponja, e possui 3 modelos diferentes:

- O Bob Esponja, que rotaciona conforme comandos do usuário.
- O Siriguejo fica parado.
- Uma televisão.

A área interna possui um piso diferente para diferir do piso externo, mas isso causa uma pequena elevação no piso, já que o piso externo é irregular no eixo y.