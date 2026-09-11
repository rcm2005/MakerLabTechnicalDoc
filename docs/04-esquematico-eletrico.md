# ⚡ Arquitetura Eletroeletrônica e Esquema de Conexões

> **Referência:** Integração de Hardware abordada na Aula 14 da disciplina *Project-based Maker Lab* (FIAP)

---

## 1. Mapeamento de Portas e Pinagem (Arduino Uno R3 / ESP32)

| Componente Periférico | Pino do Periférico | Pino no Arduino Uno | Pino no ESP32 *(Opção)* | Tipo de Sinal | Função do Sinal |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Driver L298N** | `ENA` | `D5` (PWM) | `GPIO 14` (PWM) | Digital (PWM ~980Hz) | Controle de Velocidade Motor Esquerdo |
| **Driver L298N** | `IN1` | `D6` | `GPIO 27` | Digital Output | Direção Sentido Horário Motor Esquerdo |
| **Driver L298N** | `IN2` | `D7` | `GPIO 26` | Digital Output | Direção Sentido Anti-horário Motor Esquerdo |
| **Driver L298N** | `IN3` | `D8` | `GPIO 25` | Digital Output | Direção Sentido Horário Motor Direito |
| **Driver L298N** | `IN4` | `D9` | `GPIO 33` | Digital Output | Direção Sentido Anti-horário Motor Direito |
| **Driver L298N** | `ENB` | `D10` (PWM) | `GPIO 32` (PWM) | Digital (PWM ~490Hz) | Controle de Velocidade Motor Direito |
| **Sensor HC-SR04** | `VCC` | `5V` | `VIN (5V)` | Alimentação | Tensão de Operação 5V |
| **Sensor HC-SR04** | `GND` | `GND` | `GND` | Referência | Terra Elétrico Comum |
| **Sensor HC-SR04** | `TRIG` | `D12` | `GPIO 13` | Digital Output | Pulso de Disparo Ultrassônico (10 µs) |
| **Sensor HC-SR04** | `ECHO` | `D11` | `GPIO 12` | Digital Input | Leitura do Tempo de Retorno da Onda |
| **Sensor TCRT5000 (Esq)** | `DO` | `D4` | `GPIO 18` | Digital Input | Leitura Digital de Pista (Preto/Branco) |
| **Sensor TCRT5000 (Dir)** | `DO` | `A0` / `D13` | `GPIO 19` | Digital Input | Leitura Digital de Pista (Preto/Branco) |
| **Bluetooth HC-05** | `VCC` | `5V` | `VIN (5V)` | Alimentação | Tensão de Operação 5V |
| **Bluetooth HC-05** | `GND` | `GND` | `GND` | Referência | Terra Elétrico Comum |
| **Bluetooth HC-05** | `TXD` | `D2` (SoftSerial RX)| `GPIO 16` (UART2 RX) | Digital Input | Recepção de Comandos do Smartphone |
| **Bluetooth HC-05** | `RXD` | `D3` (SoftSerial TX)| `GPIO 17` (UART2 TX) | Digital Output | Transmissão (Divisor 5V->3.3V) |

---

## 2. Diagrama de Blocos do Sistema (Lógica e Conexões de Controle)

```mermaid
flowchart TD
    subgraph CONTROLE [Unidade de Processamento Central]
        MCU["Microcontrolador<br><b>Arduino Uno R3 / ESP32</b><br>(ATmega328P)"]
    end

    subgraph SENSORES [Sensoriamento do Ambiente]
        US["Sensor Ultrassônico<br><b>HC-SR04</b><br>(Obstáculos Frontais)"]
        S1["Sensor Seguidor de Linha<br><b>TCRT5000 (Esquerdo)</b>"]
        S2["Sensor Seguidor de Linha<br><b>TCRT5000 (Direito)</b>"]
    end

    subgraph COMUNICACAO [Interface Sem Fio]
        BT["Módulo Bluetooth<br><b>HC-05 / HC-06</b><br>(UART Serial 9600 bps)"]
    end

    subgraph POTENCIA [Acionamento e Drivers]
        DRV["Driver de Motor<br><b>Ponte H Dupla L298N</b>"]
        M1(["Motor DC TT Esquerdo<br>(Caixa Redução 1:48)"])
        M2(["Motor DC TT Direito<br>(Caixa Redução 1:48)"])
    end

    %% Sinais de Controle Ponte H
    MCU -- "D5 (PWM ENA) & D10 (PWM ENB)<br>Controle de Velocidade" --> DRV
    MCU -- "D6, D7 (IN1, IN2) - Direção Motor Esq<br>D8, D9 (IN3, IN4) - Direção Motor Dir" --> DRV

    %% Saídas de Potência
    DRV -- "OUT1, OUT2" --> M1
    DRV -- "OUT3, OUT4" --> M2

    %% Sinais dos Sensores
    US -- "Trigger (Pino D12)<br>Echo (Pino D11)" --> MCU
    S1 -- "Sinal Digital (Pino D4)" --> MCU
    S2 -- "Sinal Digital (Pino A0 / D13)" --> MCU

    %% Sinais Bluetooth
    BT -- "TXD -> Pino D2 (RX Software)" --> MCU
    MCU -- "Pino D3 (TX) -> Divisor 1k/2k -> RXD" --> BT
```

---

## 3. Diagrama de Distribuição de Potência (*Power Tree*)

> [!IMPORTANT]
> **GND Comum Obrigatório:** O polo negativo da bateria (GND da Ponte H) deve estar obrigatoriamente interligado ao GND do Arduino para manter a mesma referência de sinal lógico TTL.

### Baterias Consideradas para o Projeto:
1. **Opção Principal (Recomendada):** Pack com **2x Células Li-ion 18650** em série:
   - Tensão Nominal: $7.4\text{ V}$ (Totalmente carregada: $8.4\text{ V}$)
   - Capacidade Típica: $2200\text{ mAh} \sim 4400\text{ mAh}$
   - Justificativa: Alta densidade energética, peso reduzido e excelente taxa de descarga contínua para os motores.
2. **Opção Secundária (Compatível):** Suporte para **4x Pilhas AA Alcalinas** ($6.0\text{ V}$) ou **6x Pilhas AA NiMH** ($7.2\text{ V}$).

```mermaid
flowchart LR
    subgraph FONTE [Fonte de Alimentação Primária]
        BAT["<b>Pack Baterias Li-ion 7.4V</b><br>(2x 18650 em série / 2200mAh)"]
        SW{"<b>Chave Geral On/Off</b><br>Gangorra KCD11"}
    end

    subgraph POTENCIA [Módulo de Potência & Drivers]
        L298["<b>Driver Ponte H L298N</b><br>Borne +12V (VIN) / GND"]
        REG5V["<b>Regulador 78M05 Integrado</b><br>(Fornece 5V Estável)"]
        MOT["<b>2x Motores DC TT (1:48)</b><br>Alimentados em 7.4V"]
    end

    subgraph LOGICA [Placa de Controle & Processamento]
        ARD["<b>Arduino Uno R3 / ESP32</b><br>Alimentação via Pino 5V / VIN"]
    end

    subgraph PERIFERICOS [Sensores & Comunicação 5V]
        US["Sensor HC-SR04 (5V)"]
        TC["Sensores TCRT5000 (5V)"]
        BT["Bluetooth HC-05 (5V)"]
    end

    %% Conexões de Potência
    BAT -- "Polo Positivo (+7.4V)" --> SW
    SW -- "+7.4V Comutado" --> L298
    BAT -- "Polo Negativo (0V / GND)" --> L298
    
    L298 -- "Tensão de Bateria (OUT1-OUT4)" --> MOT
    L298 --> REG5V

    %% Conexões Lógicas e Reguladas
    REG5V -- "+5V DC Regulado" --> ARD
    L298 == "GND Comum Obrigatório" ==> ARD

    %% Distribuição de 5V para Sensores
    ARD -- "+5V Barramento" --> US
    ARD -- "+5V Barramento" --> TC
    ARD -- "+5V Barramento" --> BT

    ARD == "GND Referência Comum" ==> US
    ARD == "GND Referência Comum" ==> TC
    ARD == "GND Referência Comum" ==> BT
```

---

## 4. Considerações sobre Ruído Elétrico e Proteção

1. **Capacitores de Desacoplamento:** Adição de capacitor cerâmico de $100\text{ nF}$ soldado diretamente nos terminais de cada motor TT amarelo para absorver centelhamento e transientes elétricos gerados pela comutação das escovas.
2. **Divisor de Tensão no RX do Bluetooth:** Como o pino RX do módulo HC-05 opera em nível lógico de $3.3\text{ V}$, utiliza-se um divisor resistivo ($R_1 = 1\text{ k}\Omega$, $R_2 = 2\text{ k}\Omega$) entre a saída `D3` ($5\text{ V}$) do Arduino e o pino `RX` do módulo para evitar danos térmicos no chip Bluetooth.
3. **Isolamento de Corrente Reversa (Flyback Diodes):** O circuito da Ponte H L298N conta com uma ponte de 8 diodos de roda-livre (*flyback*) que absorve picos indutivos de força contra-eletromotriz (*Back-EMF*) gerados durante a reversão de giro e frenagem dos motores.
4. **Isolamento do Barramento Lógico:** A alimentação dos motores não passa pelas trilhas do microcontrolador Arduino, garantindo que picos de torque e corrente elevada ($\sim 1\text{ A}$ por motor sob partida/travamento) não causem *brownout* ou *reset* inesperado no processador.
