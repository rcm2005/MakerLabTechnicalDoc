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

| Componente | Comprimento (mm) | Largura (mm) | Altura (mm) | Massa Estimada (g) | Forma de Fixação Mecânica Adotada | Datasheet / Referência Técnica |
| :--- | :---: | :---: | :---: | :---: | :--- | :---: |
| **Placa Arduino Uno R3** | 68,6 | 53,4 | 15,0 | 25 g | 4x Parafusos M3 x 8 mm em espaçadores (*standoffs*) de nylon de 6 mm | [Datasheet Uno R3 (ATmega328P)](https://docs.arduino.cc/resources/datasheets/A000066-datasheet.pdf) |
| **Placa ESP32 (DevKit V1)** *(Opção)* | 51,0 | 28,0 | 14,0 | 10 g | 4x Parafusos M3 ou Encaixe direto em mini Protoboard | [Datasheet ESP32 (Espressif)](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) |
| **Motor DC Gear (TT Motor 1:48)** | 70,0 | 22,4 | 19,0 | 30 g (cada) | 2x Parafusos M3 x 30 mm passantes com suporte "L" lateral | [Specs TT Motor DC](https://components101.com/motors/tt-motor-smart-car-gear-motor) |
| **Ponte H (Driver L298N)** | 43,5 | 43,5 | 27,0 | 26 g | 4x Parafusos M3 x 10 mm com arruelas isolantes na base | [Datasheet L298N (ST)](https://www.sparkfun.com/datasheets/Robotics/L298_H_Bridge.pdf) |
| **Sensor Ultrassônico (HC-SR04)** | 45,0 | 20,0 | 15,0 | 9 g | Suporte vertical em PLA/Acrílico com trava de pressão (*snap-fit*) | [Datasheet HC-SR04](https://components101.com/sensors/hc-sr04-ultrasonic-sensor) |
| **Sensor Seguidor de Linha (TCRT5000)** | 35,0 | 10,0 | 15,0 | 5 g (cada) | Parafusos M3 na face inferior do chassi voltados para o solo | [Datasheet TCRT5000 (Vishay)](https://www.vishay.com/docs/83760/tcrt5000.pdf) |
| **Módulo Bluetooth (HC-05 / HC-06)** | 37,3 | 15,5 | 4,0 | 4 g | Fita dupla-face 3M / presilha no deck superior | [Datasheet HC-05](https://components101.com/wireless/hc-05-bluetooth-module) |
| **Slot de Bateria (Pack 2x 18650)** | 76,0 | 41,0 | 20,0 | 110 g (c/ células) | Encaixe tipo gaveta/trilho deslizante e cinta de velcro rápida | [Datasheet Li-ion 18650](https://www.ineltro.ch/media/downloads/sae/panasonic/18650.pdf) |
| **Slot de Bateria (4x AA)** *(Alternativo)* | 58,0 | 62,0 | 15,0 | 85 g (c/ pilhas) | Fita dupla face industrial ou parafusos M3 escareados | [Specs Suporte 4x AA](https://components101.com/misc/aa-battery-holder) |
| **Roda Boba (Caster Wheel Ø25mm)** | 38,0 | 32,0 | 35,0 | 22 g | 4x Parafusos M3 x 10 mm fixados na parte inferior dianteira | [Specs Roda Boba](https://components101.com/misc/caster-wheel-mechanics) |
| **2x Rodas Tratoras (Ø65mm)** | Ø 65,0 | 26,5 | Ø 65,0 | 76 g (par) | Acoplamento sob pressão no eixo duplo "D" c/ parafuso central | [Specs Roda 65mm TT](https://components101.com/misc/smart-car-wheel) |
| **Chave Gangorra Geral (KCD11)** | 15,0 | 10,5 | 19,0 | 3 g | Encaixe por pressão (*snap-in*) na borda lateral do deck superior | [Specs Chave KCD11](https://components101.com/switches/rocker-switch) |

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
