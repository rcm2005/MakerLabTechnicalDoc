# ⚙️ Especificação Mecânica, Fabricação e Tolerâncias

> **Referência:** Diretrizes das Aulas 14 e 15 da disciplina *Project-based Maker Lab* (FIAP)

---

## 1. Filosofia de Design: Do "Bonito" ao Funcional (Modelo B)

Conforme apresentado em aula, projetos de robótica frequentemente enfrentam a armadilha do **"Modelo A"** (estética automotiva fechada e visualmente atraente, mas estruturalmente ineficiente e de difícil manutenção). O nosso projeto adota integralmente as diretrizes do **"Modelo B"** (simples, modular, acessível e funcional).

```
   ┌──────────────────────────────────────────────┬──────────────────────────────────────────────┐
   │             MODELO A (Problemático)          │             MODELO B (Adotado / Eficiente)   │
   ├──────────────────────────────────────────────┼──────────────────────────────────────────────┤
   │ ❌ Motores inacessíveis (desmonte total)      │ ✅ Motores removíveis por parafusos M3 diretos│
   │ ❌ Porta USB obstruída pela carenagem        │ ✅ Conector USB 100% livre na borda lateral  │
   │ ❌ Bateria embutida / sem troca rápida       │ ✅ Bateria em berço removível tipo gaveta    │
   │ ❌ Parafusos sem espaço para a chave Phillips │ ✅ Acesso axial vertical livre para chaves   │
   │ ❌ Paredes muito finas que quebram facilmente │ ✅ Paredes reforçadas (espessura >= 3.0 mm)  │
   │ ❌ Cabos prensados e sem canaletas           │ ✅ Rasgos integrados de passagem de fios     │
   │ ❌ Manutenção lenta, complexa e frustrante   │ ✅ Manutenção modular em menos de 2 minutos   │
   └──────────────────────────────────────────────┴──────────────────────────────────────────────┘
```

---

## 2. Tabela Oficial de Tolerâncias Dimensionais de Fabricação

Na manufatura aditiva (Impressão 3D FDM - PLA/PETG) e corte a laser (acrílico/MDF), **a dimensão nominal do software CAD difere da dimensão real da peça fabricada** devido à dilatação/retração térmica e ao diâmetro do feixe/bico.

A tabela abaixo define os ajustes dimensionais incorporados nos modelos CAD do chassi:

| Tipo de Ajuste Mecânico | Aplicação no Projeto do Robô | Compensação CAD Recomendada |
| :--- | :--- | :---: |
| **Encaixe Muito Justo (*Press-fit*)** | Travamento de porcas hexagonais M3 em rebaixo cego e berço de sensores | **+0,1 mm a +0,2 mm** |
| **Encaixe Deslizante (*Sliding-fit*)** | Trilho de remoção do suporte de bateria e presilhas modulares | **+0,2 mm a +0,4 mm** |
| **Peças Móveis / Articulações** | Alojamento do pivô da roda boba e folgas dos eixos dos motores | **+0,3 mm a +0,5 mm** |
| **Furos de Passagem para Parafusos M3**| Furos para parafusos dos motores TT, Arduino e Ponte H L298N | **Ø 3,2 mm a 3,4 mm** |
| **Furos para Insert Térmico de Latão M3**| Furos cegos para fusão térmica de roscas metálicas | **Ø 4,0 mm a 4,2 mm** |
| **Espessura Mínima Estrutural** | Base principal, placas secundárias e suportes de tração | **3,0 mm a 3,5 mm** |

---

## 3. Diretrizes de Montagem e Fixação Mecânica

1. **Estrutura Base (Deck Inferior e Superior):**
   - Fabricada em chapa de 3.0 mm de acrílico cortado a laser ou placa de PLA impresso com 100% de preenchimento nos pontos de fixação e 30% *Gyroid* no restante para redução de peso.
   - O arquivo pronto para impressão 3D (FDM) encontra-se compactado em [`../assets/STL_Chassi.zip`](../assets/STL_Chassi.zip) (contém `base rafa.stl`).
   - Distância entre decks mantida em 25 mm através de 4 espaçadores sextavados de nylon ou latão M3x25mm.

2. **Fixação dos Motores TT:**
   - Cada motor DC TT é preso por **2 parafusos longos M3 x 30 mm** que atravessam o corpo do motor e o suporte em "L".
   - Porcas auto-travantes (*nyloc*) ou porcas convencionais com arruela de pressão impedem o afrouxamento decorrente da vibração mecânica contínua dos motores.

3. **Gerenciamento de Chicote Elétrico (Cable Routing):**
   - Aberturas ovais de 25 x 8 mm posicionadas estrategicamente próximas aos bornes da Ponte H e aos pinos do Arduino permitem a passagem limpa dos cabos *jumper*, prevenindo esmagamento ou curto-circuito.

---

## 4. Requisitos para Manutenibilidade (Checklist de Homologação)

Conforme os critérios de aprovação da disciplina:
- [x] **Substituição da Bateria:** Concluível em menos de 10 segundos sem uso de ferramentas.
- [x] **Reprogramação do Firmware:** Cabo USB conecta livremente sem remover nenhum componente ou tampa.
- [x] **Troca da Placa Controladora:** Fixada por parafusos M3 de fácil acesso superior.
- [x] **Substituição de Motor TT:** Troca individual do motor esquerdo ou direito desaparafusando apenas os 2 parafusos laterais dedicados.
- [x] **Desconexão de Sensores:** Todos os cabos utilizam conectores padrão Dupont fêmea/macho removíveis.
