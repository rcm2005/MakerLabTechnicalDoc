# 📐 Croqui e Layout Estrutural do Chassi

> **Referência:** Atividade 2 da Aula 14 & Aula 15 — *Project-based Maker Lab*

---

## 1. Visualização do Croqui Técnico

O diagrama vetorial com cotas milimétricas em padrão de engenharia encontra-se renderizado abaixo e disponível em alta resolução no arquivo [`assets/chassi_croqui_tecnico.svg`](../assets/chassi_croqui_tecnico.svg).

![Croqui Técnico Estrutural](../assets/chassi_croqui_tecnico.svg)

---

## 2. Layout em Camadas (Vista Explodida Estrutural)

A arquitetura física foi desenhada em dois planos horizontais (*Dual-Deck Architecture*) para maximizar o aproveitamento da área superficial e proteger os circuitos de controle:

```mermaid
graph TD
    subgraph Camada Superior [Deck Superior - Controle & Sensoriamento]
        US[Sensor Ultrassônico HC-SR04 Frontal]
        ARD[Arduino Uno R3 / ESP32]
        BT[Módulo Bluetooth HC-05]
        SW[Chave Geral Liga/Desliga]
    end

    subgraph Camada Intermediaria [Espaçadores Standoffs M3 x 25mm]
        ST1[Espaçador Dianteiro Esq]
        ST2[Espaçador Dianteiro Dir]
        ST3[Espaçador Traseiro Esq]
        ST4[Espaçador Traseiro Dir]
    end

    subgraph Camada Inferior [Deck Inferior - Potência & Tração]
        RB[Roda Boba Frontal Ø25mm]
        BAT[Pack Bateria 2x 18650 7.4V]
        PH[Ponte H L298N c/ Dissipador]
        ME[Motor TT Amarelo Esquerdo + Roda Ø65mm]
        MD[Motor TT Amarelo Direito + Roda Ø65mm]
    end

    Camada Superior --> Camada Intermediaria --> Camada Inferior
```

---

## 3. Coordenadas e Cotas Principais de Furação (Grid Cartesiano)

Considerando a origem $(X=0, Y=0)$ no vértice frontal esquerdo do chassi principal de $150 \times 200\text{ mm}$:

| Componente / Furo | Coordenada X (mm) | Coordenada Y (mm) | Diâmetro Furo (mm) | Observações |
| :--- | :---: | :---: | :---: | :--- |
| **Suporte HC-SR04** | $75,0$ (Centro) | $5,0$ | Ranhura $45 \times 20$ | Encaixe vertical com trava |
| **Roda Boba (4 furos)** | $60,0 / 90,0$ | $30,0 / 60,0$ | $\varnothing\ 3,3$ | Furos passantes M3 |
| **Espaçador Standoff 1** | $15,0$ | $20,0$ | $\varnothing\ 3,3$ | Fixação do deck superior |
| **Espaçador Standoff 2** | $135,0$ | $20,0$ | $\varnothing\ 3,3$ | Fixação do deck superior |
| **Fixação Arduino Uno (4x)**| $50,0 \sim 105,0$ | $70,0 \sim 130,0$ | $\varnothing\ 3,2$ | Padrão geométrico oficial Uno R3 |
| **Ponte H L298N (4 furos)** | $20,0 / 60,0$ | $145,0 / 185,0$| $\varnothing\ 3,3$ | Próximo aos motores |
| **Suporte Bateria (2 furos)**| $85,0 / 135,0$ | $145,0 / 185,0$| $\varnothing\ 3,4$ | Berço removível |
| **Suporte Motor TT Esquerdo**| $5,0$ | $135,0 \sim 175,0$ | $\varnothing\ 3,3$ (2x) | Parafusos transversais M3x30 |
| **Suporte Motor TT Direito** | $145,0$ | $135,0 \sim 175,0$ | $\varnothing\ 3,3$ (2x) | Parafusos transversais M3x30 |
| **Espaçador Standoff 3** | $15,0$ | $190,0$ | $\varnothing\ 3,3$ | Fixação do deck superior |
| **Espaçador Standoff 4** | $135,0$ | $190,0$ | $\varnothing\ 3,3$ | Fixação do deck superior |

---

## 4. Análise Cinemática e Estabilidade

- **Bitola (Largura entre rodas):** $W = 165\text{ mm}$ (de centro a centro dos pneus).
- **Distância entre Eixos (Wheelbase):** $L = 135\text{ mm}$ (do eixo de tração até o ponto de contato da roda boba).
- **Raio de Giro Mínimo:** $R_{\text{min}} = 0\text{ mm}$ (rotação no próprio eixo por inversão de rotação dos motores $V_L = -V_R$).
- **Vão Livre do Solo (*Ground Clearance*):** $20,0\text{ mm}$, suficiente para transpor pequenas irregularidades e pistas de teste da universidade.
- **Distribuição de Massa:** $65\%$ da massa concentrada sobre o eixo de tração e $35\%$ sobre a roda boba dianteira, garantindo tração máxima sem empinamento nas acelerações.
