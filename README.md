# 🌀 Maze of UCSC
*A 3D Multi-Floor Maze Runner Game — Advanced Snake and Ladders Simulation*

## 📘 Overview
**Maze of UCSC** is a Python-based, text-driven maze simulation inspired by Snake and Ladders.  
Three players (A, B, and C) compete to capture a flag located somewhere within a **three-floor maze**, navigating through **stairs, poles, walls, consumable cells**, and the mysterious **Bawana** area.

Gameplay is rule-based, deterministic (seeded randoms), and structured around dice rolls, direction logic, and event triggers.  

---

## 🧩 Features
✅ Multi-floor 3D maze structure (10×25 grid per floor)  
✅ Dynamic stairs, poles, and wall handling  
✅ Dual dice system (movement + direction)  
✅ Event-driven area — **Bawana** with random effects  
✅ Movement points & consumable logic  
✅ Loop detection for infinite stair/pole traversal  
✅ Text-based output with detailed player logs  

---

## 🧱 Maze Structure
Each cell is referenced as:
```
[floor, width_index, length_index]
```
- Floor numbers: 0, 1, 2  
- Width range: 0–9  
- Length range: 0–24  

### Floor Layout Summary
| Floor | Description |
|:------:|:-------------|
| **0** | Ground floor, includes the starting area & Bawana |
| **1** | Two 10×8 blocks connected by a 4×9 bridge |
| **2** | 10×9 rectangular top area |

---

## 🧍 Players
| Player | Start (w,l) | First Maze Cell | Initial Direction |
|:-------:|:-------------|:----------------|:------------------|
| A | (6,12) | (5,12) | North |
| B | (9,8) | (9,7) | West |
| C | (9,16) | (9,17) | East |

Each player:
- Rolls **two dice**:
  - 🎲 *Movement Dice* → values 1–6  
  - 🧭 *Direction Dice* → faces: {N, E, S, W, Empty, Empty}  
- Must roll a `6` to enter the maze.  
- Loses or gains movement points depending on cells traversed.  

---

## 🗂️ Input Files
All configurations are defined externally in `/data/`:

| File | Purpose | Example Format |
|:------|:---------|:----------------|
| `stairs.txt` | Stair connections | `[0,4,5,2,0,10]` |
| `poles.txt` | Poles (vertical movement) | `[0,2,5,24]` |
| `walls.txt` | Impassable walls | `[1,0,2,8,2]` |
| `flag.txt` | Flag position | `[2,5,13]` |
| `seed.txt` | Random seed | `12345` |

---

## 🧮 Movement & Logic
- Each player starts with **100 movement points**.  
- Each dice roll costs **2 points** if movement is blocked.  
- Passing through cells consumes or adds points based on random consumable/bonus values.  

| Cell Type | Effect |
|:-----------|:--------|
| Consumable | Deducts 0–4 points |
| Bonus | Adds +1–5 points |
| Multiplier | Doubles or triples remaining points |

If movement points drop ≤0, the player is transported to **Bawana**.

---

## 🍽️ The Bawana Area
A 12-cell special zone on the ground floor (6,20–9,24).

| Effect | Description |
|:--------|:-------------|
| **Food Poisoning** | Miss 3 turns, reappear randomly |
| **Disoriented** | 50 points, random movement for 4 turns |
| **Triggered** | Moves ×2 distance for 50 points |
| **Happy** | Gains 200 points |
| **Bonus Cells** | Random +10–100 points |

---

## ⚙️ Advanced Rules
- Stairs can **change direction** every 5 rounds.  
- Players can **capture** each other by landing on the same cell.  
- Infinite stair/pole loops trigger a **safe reset** to start zone.  
- Multiple stairs from one block: choose closest to flag (by distance).  

---

## 💬 Example Console Output
```text
A is at starting area and rolls 6 → enters maze at [0,5,12].
A rolls 3 and moves North by 3 → now at [0,2,12].
A moved 3 cells costing 8 points → remaining 92 points, direction North.

B rolls 5 → blocked by wall at [1,7,5].
B remains at [1,7,5], loses 2 movement points.

C lands on [0,3,3] (stair cell).
C takes stairs to [1,6,7] (floor 1).
```

### Bawana Example
```text
A movement points depleted → transporting to Bawana.
A eats from Bawana and is disoriented (50 points).
A rolls 4 → disoriented → moves randomly East → now at [0,9,22].
```

---

## 🏗️ Project Structure
```
maze_ucsc/
├── main.py
├── maze/
│   ├── __init__.py
│   ├── parser.py
│   ├── player.py
│   ├── game_controller.py
│   ├── cell.py
│   └── utils.py
├── data/
│   ├── stairs.txt
│   ├── poles.txt
│   ├── walls.txt
│   ├── flag.txt
│   └── seed.txt
└── README.md
```

---

## ▶️ How to Run
### Requirements
- Python ≥ 3.9  
- No external dependencies (pure standard library)

### Steps
```bash
# Clone or copy project folder
git clone https://github.com/<your_username>/maze-ucsc.git
cd maze-ucsc

# Ensure data/ files are correctly filled
# Run the main program
python main.py
```

Optional: Add a `--seed` argument for reproducibility:
```bash
python main.py --seed 42
```

---

## 🧠 Suggested Improvements
- Visual grid rendering using **pygame** or **tkinter**  
- Leaderboard & score logging  
- AI player simulation  
- Save/load game state  
- Web version using Flask or Node.js  

---

## 📜 License
MIT License — for academic use under UCSC coursework (Game Development / Simulation Assignment).  
