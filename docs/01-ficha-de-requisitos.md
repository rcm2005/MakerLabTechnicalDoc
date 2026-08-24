# 📋 Ficha de Requisitos do Carrinho-Robô (2WD)

> **Disciplina:** Project-based Maker Lab  
> **Curso:** Engenharia de Software — FIAP  
> **Docente:** Profª Dra. Gedeane G.S. Kenshima (`profgedeane.kenshima@fiap.com.br`)  
> **Projeto:** Carrinho-Robô Autônomo / Controlado via Bluetooth (Chassi 2WD Diferencial)  
> **Integrantes:**  
> - Rafael Carvalho Mattos (RM 99874)  
> - Rafael Autieri dos Anjos (RM 550885)  
> - Luiza Cristina Silva (RM 99367)  
> - Levy Nascimento Junior (RM 98655)

---

## 1. Visão Geral do Sistema

O projeto consiste no desenvolvimento de um **carrinho-robô móvel com tração diferencial de duas rodas (2WD) e roda boba de apoio (caster wheel)**, controlado por microcontrolador **Arduino Uno R3** (ou ESP32), acionado por ponte H dupla (**L298N**), alimentado por pack de baterias Li-ion (7.4V / 2x 18650), e equipado com sensores para navegação autônoma e módulo de comunicação sem fio.

---

## 2. Requisitos Gerais de Hardware

| Item | Especificação Adotada | Justificativa Técnica |
| :--- | :--- | :--- |
| **Arquitetura de Tração** | 2WD Diferencial + 1 Roda Boba | Alto raio de giro (curva sobre o próprio eixo), simplicidade cinemática e menor consumo de corrente em relação ao 4WD. |
| **Motores** | 2x Motores DC com Caixa de Redução TT (1:48) | Torque nominal de ~0.8 kgf.cm a 6V, velocidade de rotação equilibrada (~200 RPM sem carga) e baixo custo. |
| **Rodas Principais** | 2x Rodas emborrachadas Ø 65 mm x 26 mm | Excelente coeficiente de atrito em superfícies lisas e diâmetro compatível com vão livre de 20 mm do solo. |
| **Apoio Dianteiro** | 1x Roda Boba (Caster Wheel) Ø 25 mm | Movimentação omnidirecional passiva e baixo atrito de arrasto frontal. |
| **Placa Controladora** | 1x Arduino Uno R3 (Microcontrolador ATmega328P) | Robustez de I/O (5V nativo), 6 saídas PWM para controle de velocidade dos motores e interface serial dedicada. |
| **Driver de Potência** | 1x Módulo Driver Ponte H Dupla L298N | Suporta tensão de operação de até 35V, corrente de pico de 2A por canal e isolamento lógico para proteção do Arduino. |
| **Alimentação** | Pack 2x Baterias Li-ion 18650 (7.4V nominal / 8.4V máx. 4400 mAh) | Alta densidade de energia, descarga contínua estável e facilidade de substituição. |
| **Sensor de Obstáculos** | 1x Sensor Ultrassônico HC-SR04 | Alcance de detecção de 2 cm a 400 cm com resolução de 3 mm; ângulo de abertura cone de ~15°. |
| **Comunicação Sem Fio** | 1x Módulo Bluetooth HC-05 / HC-06 | Comunicação Serial UART (Baudrate padrão 9600 bps) para controle manual por aplicativo mobile. |

---

## 3. Tabela Dimensional Completa (Conforme Aula 15)

Esta tabela cumpre a exigência da atividade prática proposta no slide 13 da **Aula 15 – Mecânica**:

| Componente | Comprimento (mm) | Largura (mm) | Altura (mm) | Massa Aprox. (g) | Forma de Fixação Mecânica |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Motor Esquerdo (TT Amarelo)** | 70,0 | 22,4 | 19,0 | 30 g | 2x Parafusos M3 x 30 mm transversais com porcas e suporte lateral em "L" de PLA/Acrílico |
| **Motor Direito (TT Amarelo)** | 70,0 | 22,4 | 19,0 | 30 g | 2x Parafusos M3 x 30 mm transversais com porcas e suporte lateral em "L" de PLA/Acrílico |
| **Arduino Uno R3 (Controladora)** | 68,6 | 53,4 | 15,0 | 25 g | 4x Parafusos M3 x 8 mm com espaçadores de nylon (standoffs) de 6 mm da base |
| **Ponte H (Módulo L298N)** | 43,5 | 43,5 | 27,0 | 26 g | 4x Parafusos M3 x 10 mm nos cantos da placa com arruelas isolantes |
| **Bateria (Suporte 2x 18650)** | 76,0 | 41,0 | 20,0 | 110 g (c/ células) | Encaixe tipo trilho deslizante (*snap-in*) com fita de fixação rápida (Velcro) para remoção imediata |
| **Sensor Ultrassônico (HC-SR04)** | 45,0 | 20,0 | 15,0 | 9 g | Suporte vertical frontal com recorte para transdutores (Ø 16 mm espaçados em 26 mm) e trava de pressão |
| **Roda Boba (Caster Wheel Ø25mm)**| 38,0 | 32,0 | 35,0 | 22 g | 4x Parafusos M3 x 10 mm com contraporcas fixadas sob a base dianteira |
| **2x Rodas Principais (Ø65mm)** | Ø 65,0 | 26,5 | Ø 65,0 | 38 g cada | Encaixe direto no eixo duplo-D do motor TT travado com parafuso central autoatarraxante |

---

## 4. Distribuição Espacial e Posição dos Componentes

A disposição dos módulos físicos no chassi foi concebida sob a ótica da **engenharia de confiabilidade e centro de gravidade (CG)**:

```
                  [ FRENTE DO ROBÔ ]
       +--------------------------------------+
       |        [ Sensor HC-SR04 ]            |  <- Nível Superior Frontal
       |                                      |
       |         ( Roda Boba Frontal )        |  <- Nível Inferior
       |                                      |
       |       +----------------------+       |
       |       |                      |       |
       | [USB] |    ARDUINO UNO R3    |       |  <- Nível Superior Central
       |       |                      |       |     (Porta USB voltada para a lateral esquerda)
       |       +----------------------+       |
       |                                      |
 [RODA]|   +--------------+    +------------+ |[RODA]
 [ESQ] |   |   PONTE H    |    |  BATERIA   | |[DIR]
 [ TT ]|   |    L298N     |    | (2x 18650) | |[ TT ]
       |   +--------------+    +------------+ |
       +--------------------------------------+
                 [ TRASEIRA DO ROBÔ ]
```

### Justificativas do Posicionamento:
1. **Centro de Gravidade (CG):** A bateria (componente mais pesado com ~110g) e os motores foram posicionados sobre o eixo de tração traseiro, maximizando a aderência das rodas emborrachadas e evitando patinagem ou oscilações de inclinação (*pitching*).
2. **Acessibilidade do Conector USB:** A porta USB Type-B do Arduino Uno fica posicionada para a borda lateral esquerda, garantindo reprogramação via cabo sem necessidade de desmontar componentes.
3. **Isolamento de Ruído Eletromagnético (EMI):** O Arduino Uno fica suspenso em um segundo nível através de espaçadores (*standoffs*), distante dos enrolamentos dos motores DC, minimizando interferências nos pinos analógicos e digitais.
4. **Campo de Visão do Sensor:** O transdutor ultrassônico HC-SR04 está localizado na linha central superior do bico dianteiro, evitando qualquer ponto cego ou bloqueio pelos pneus.
