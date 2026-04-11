# Arcade — Architecture du Core

## Table des matières

1. [Vue d'ensemble](#1-vue-densemble)
2. [Diagramme de classes](#2-diagramme-de-classes)
3. [Flux de démarrage](#3-flux-de-démarrage)
4. [Boucle principale](#4-boucle-principale)
5. [Chargement d'une librairie dynamique](#5-chargement-dune-librairie-dynamique)
6. [Changement de jeu à chaud](#6-changement-de-jeu-à-chaud)
7. [Changement de librairie graphique à chaud](#7-changement-de-librairie-graphique-à-chaud)
8. [Flux de sortie](#8-flux-de-sortie)
9. [Description des composants](#9-description-des-composants)
10. [Données partagées](#10-données-partagées)
11. [Implémenter une nouvelle librairie](#11-implémenter-une-nouvelle-librairie)

---

## 1. Vue d'ensemble

Le core Arcade est un moteur de plateforme basé sur le **chargement dynamique de plugins** (`.so`).
Il ne connaît jamais les implémentations concrètes des jeux ni des librairies graphiques — il interagit
uniquement via les interfaces `IGame` et `IDisplay`.

```
┌─────────────────────────────────────────────────────┐
│                    arcade (binary)                  │
│                                                     │
│   ┌──────────┐   ┌─────────────┐   ┌─────────────┐ │
│   │   Core   │──▶│ LibManager  │──▶│  DlLoader   │ │
│   └──────────┘   └─────────────┘   └─────────────┘ │
│        │                                   │        │
│        ▼                                   ▼        │
│   ┌──────────────┐              ┌──────────────────┐│
│   │SystemCommand │              │   libdl (dlopen) ││
│   └──────────────┘              └──────────────────┘│
└─────────────────────────────────────────────────────┘
          │                  │
          ▼                  ▼
   ┌────────────┐    ┌─────────────────┐
   │ IGame*.so  │    │  IDisplay*.so   │
   │ (plugins)  │    │   (plugins)     │
   └────────────┘    └─────────────────┘
```

---

## 2. Diagramme de classes

Plutôt qu'un seul schéma surchargé, l'architecture est présentée en **trois vues indépendantes** :
la composition du core, la hiérarchie des jeux et la hiérarchie des displays.
Les interfaces `IGame` et `IDisplay` sont le point de jonction entre ces vues — elles
constituent le contrat avec les plugins `.so`.

### 2.1 Composition du core

Classes et agrégations internes au binaire `arcade`. Les plugins sont volontairement
représentés uniquement par leurs interfaces `IGame` / `IDisplay` : le core ne connaît rien de plus.

```mermaid
classDiagram
    direction LR

    class Core {
        -LibManager _libManager
        -SystemCommand _systemCommand
        -bool _running
        +int run(path)
        +void menu()
    }

    class SystemCommand {
        -callback onExitRequested
        -callback onMenuRequested
        -callback onNextGameRequested
        -callback onNextDisplayRequested
        +void handleSystemEvent(input)
    }

    class LibManager {
        -DlLoader~IGame~ _gameLoader
        -DlLoader~IDisplay~ _displayLoader
        -vector~string~ _games
        -vector~string~ _displays
        +void scanLibs(path)
        +void loadGame(path)
        +void loadDisplay(path)
        +void nextGame()
        +void nextDisplay()
        +IGame* getGame()
        +IDisplay* getDisplay()
    }

    class DlLoader~T~ {
        -void* handle
        +void loadLib(path)
        +T* getInstance(symbol)
        +void unloadLib()
        +bool hasSymbol(path, symbol)$
    }

    class IGame {
        <<interface>>
    }

    class IDisplay {
        <<interface>>
    }

    Core *-- SystemCommand : owns
    Core *-- LibManager : owns
    LibManager *-- "2" DlLoader~T~ : owns
    LibManager o-- IGame : uses
    LibManager o-- IDisplay : uses
```

> `LibManager` détient **deux** `DlLoader` paramétrés (`DlLoader<IGame>` et `DlLoader<IDisplay>`).
> Ils sont regroupés ici en une seule boîte pour garder le schéma lisible.

### 2.2 Hiérarchie des jeux

Contrat `IGame` et classe abstraite `AGame` mutualisant l'état (entités, texte, score, game over).
Les implémentations concrètes (`Snake`, `Pacman`, `Menu`, …) héritent d'`AGame` et sont chargées dynamiquement via `createGame()`.

```mermaid
classDiagram
    direction BT

    class IGame {
        <<interface>>
        +void init()*
        +void stop()*
        +void restart()*
        +void update(input, dt)*
        +vector~Entity~ getEntity()*
        +vector~Text~ getText()*
        +int getScore()*
        +bool isGameOver()*
        +string getName()*
    }

    class AGame {
        <<abstract>>
        #vector~Entity~ _entities
        #vector~Text~ _texts
        #int _score
        #bool _isGameOver
        #string _name
        +vector~Entity~ getEntity()
        +vector~Text~ getText()
        +int getScore()
        +bool isGameOver()
        +string getName()
        #void addEntity(entity)
        #void addText(text)
        #void setScore(score)
        #void clearEntities()
        #void clearTexts()
    }

    class ConcreteGame {
        <<plugin .so>>
        +void init()
        +void stop()
        +void restart()
        +void update(input, dt)
    }

    AGame ..|> IGame : implements
    ConcreteGame --|> AGame : extends
```

### 2.3 Hiérarchie des displays

Même principe côté rendu. `ADisplay` ne fournit que la gestion du nom ;
chaque plugin graphique (`SDL2`, `SFML`, `NCurses`) implémente les méthodes d'affichage.

```mermaid
classDiagram
    direction BT

    class IDisplay {
        <<interface>>
        +void init()*
        +void stop()*
        +string getName()*
        +Input getEvent()*
        +void clear()*
        +void drawEntity(entities)*
        +void drawText(texts)*
        +void display()*
    }

    class ADisplay {
        <<abstract>>
        -string _name
        +string getName()
        #void setName(name)
    }

    class ConcreteDisplay {
        <<plugin .so>>
        +void init()
        +void stop()
        +Input getEvent()
        +void clear()
        +void drawEntity(entities)
        +void drawText(texts)
        +void display()
    }

    ADisplay ..|> IDisplay : implements
    ConcreteDisplay --|> ADisplay : extends
```

### 2.4 Vue condensée des dépendances

Pour une lecture rapide des dépendances inter-composants, sans méthodes ni attributs :

```mermaid
flowchart LR
    Core --> SystemCommand
    Core --> LibManager
    LibManager --> DlLoaderG[DlLoader&lt;IGame&gt;]
    LibManager --> DlLoaderD[DlLoader&lt;IDisplay&gt;]
    DlLoaderG -. loads .-> IGame
    DlLoaderD -. loads .-> IDisplay
    AGame -.->|implements| IGame
    ADisplay -.->|implements| IDisplay
    Snake & Pacman & Menu --> AGame
    SDL2 & SFML & NCurses --> ADisplay
```

---

## 3. Flux de démarrage

```mermaid
sequenceDiagram
    participant main
    participant Core
    participant LibManager
    participant DlLoader
    participant IDisplay
    participant IGame

    main->>Core: new Core()
    Note over Core: Enregistre les callbacks<br/>dans SystemCommand

    main->>Core: run(displayPath)

    Core->>LibManager: loadDisplay
    LibManager->>DlLoader: loadLib
    LibManager->>DlLoader: getInstance
    DlLoader-->>LibManager: IDisplay*
    LibManager->>IDisplay: init

    Core->>LibManager: scanLibs
    Note over LibManager: Parcourt les .so<br/>Classe chaque lib via hasSymbol<br/>Remplit _games[] et _displays[]

    Core->>LibManager: loadGame (arcade_menu.so)
    LibManager->>DlLoader: loadLib
    LibManager->>DlLoader: getInstance
    DlLoader-->>LibManager: IGame*
    LibManager->>IGame: init

    Note over Core: Entrée dans la boucle principale
```

---

## 4. Boucle principale

Chaque frame exécute **trois phases successives** sur les plugins courants
(`IGame` et `IDisplay`). Le `LibManager` est re-interrogé entre les phases
car un input système peut avoir remplacé le jeu ou la display pendant le même tour de boucle.

### 4.1 Les trois phases d'une frame

| # | Phase | Qui fait quoi |
|---|---|---|
| 1 | **Input** | `display.getEvent()` → `SystemCommand` dispatche éventuellement vers `LibManager` (next game / next display / menu / exit) |
| 2 | **Update** | `game.update(input, dt)` fait avancer la logique du jeu courant |
| 3 | **Render** | `display.clear()` → `drawEntity` / `drawText` / `playSound` → `display()` |

### 4.2 Pseudo-code

Version simplifiée de `Core::run` (voir `src/core/core.cpp:183`) :

```cpp
while (_running) {
    // --- Phase 1 : Input + commandes système ---
    IDisplay* display = libManager.getDisplay();          // garde-fou
    Input input = display->getEvent();
    systemCommand.handleSystemEvent(input);               // peut charger un autre jeu/display

    // --- Phase 2 : Update du jeu ---
    display = libManager.getDisplay();                    // re-lecture après swap éventuel
    IGame*  game    = libManager.getGame();
    if (game == nullptr) continue;
    game->update(input, deltaTime);

    // --- Phase 3 : Render ---
    display = libManager.getDisplay();                    // re-lecture à nouveau
    display->clear();
    display->drawEntity(game->getEntity());
    display->drawText  (game->getText());
    display->playSound (game->getSounds());
    display->display();
}
```

> **Règle invariante** : après `handleSystemEvent`, `display` et `game` doivent toujours être
> ré-obtenus via `LibManager`. Les pointeurs précédents peuvent être invalides (lib déchargée).

### 4.3 Séquence d'une frame

Diagramme d'une **itération unique** quand aucun événement système n'interrompt le flux :

```mermaid
sequenceDiagram
    autonumber
    participant Core
    participant Display as IDisplay
    participant SysCmd as SystemCommand
    participant Game as IGame

    Note over Core,Game: Phase 1 — Input
    Core->>Display: getEvent
    Display-->>Core: Input
    Core->>SysCmd: handleSystemEvent
    Note right of SysCmd: Si input système :<br/>déclenche un callback<br/>(nextGame / nextDisplay / menu / exit)

    Note over Core,Game: Phase 2 — Update
    Core->>Game: update

    Note over Core,Game: Phase 3 — Render
    Core->>Display: clear
    Core->>Game: getEntity
    Core->>Game: getText
    Core->>Game: getSounds
    Game-->>Core: état à afficher
    Core->>Display: drawEntity
    Core->>Display: drawText
    Core->>Display: playSound
    Core->>Display: display
```

> Les détails des cas particuliers (`EXIT`, `MENU`, `NEXTGAME`, `NEXTDISPLAY`) sont traités dans
> les sections dédiées [§6](#6-changement-de-jeu-à-chaud), [§7](#7-changement-de-librairie-graphique-à-chaud) et [§8](#8-flux-de-sortie).

---

## 5. Chargement d'une librairie dynamique

Ce diagramme décrit le mécanisme interne de `DlLoader` lors d'un `loadGame` ou `loadDisplay`.

```mermaid
sequenceDiagram
    participant LibManager
    participant DlLoader
    participant libdl
    participant Plugin as .so Plugin

    LibManager->>DlLoader: loadLib
    Note right of DlLoader: unloadLib si un handle<br/>est déjà ouvert
    DlLoader->>libdl: dlopen
    libdl->>Plugin: chargement en mémoire
    libdl-->>DlLoader: handle

    LibManager->>DlLoader: getInstance
    DlLoader->>libdl: dlsym
    libdl-->>DlLoader: pointeur de fonction
    DlLoader->>Plugin: createGame
    Plugin-->>DlLoader: IGame*
    DlLoader-->>LibManager: IGame*

    LibManager->>Plugin: init
```

---

## 6. Changement de jeu à chaud

Déclenché par la touche `NEXTGAME` ou `MENU`.

```mermaid
sequenceDiagram
    participant Core
    participant SystemCommand
    participant LibManager
    participant OldGame as IGame (ancien)
    participant DlLoader
    participant NewGame as IGame (nouveau)

    Note over Core: Input NEXTGAME reçu

    Core->>SystemCommand: handleSystemEvent
    SystemCommand->>Core: onNextGameRequested
    Core->>LibManager: nextGame

    Note over LibManager: Rotation circulaire<br/>sur _games[]

    LibManager->>OldGame: stop
    LibManager->>OldGame: delete
    LibManager->>DlLoader: unloadLib

    LibManager->>DlLoader: loadLib
    LibManager->>DlLoader: getInstance
    DlLoader-->>LibManager: IGame*
    LibManager->>NewGame: init
```

---

## 7. Changement de librairie graphique à chaud

Déclenché par la touche `NEXTDISPLAY`.

```mermaid
sequenceDiagram
    participant Core
    participant SystemCommand
    participant LibManager
    participant OldDisplay as IDisplay (ancien)
    participant DlLoader
    participant NewDisplay as IDisplay (nouveau)

    Note over Core: Input NEXTDISPLAY reçu

    Core->>SystemCommand: handleSystemEvent
    SystemCommand->>Core: onNextDisplayRequested
    Core->>LibManager: nextDisplay

    Note over LibManager: Rotation circulaire<br/>sur _displays[]

    LibManager->>OldDisplay: stop
    LibManager->>OldDisplay: delete
    LibManager->>DlLoader: unloadLib

    Note over DlLoader: -fno-gnu-unique garantit<br/>le déchargement effectif

    LibManager->>DlLoader: loadLib
    LibManager->>DlLoader: getInstance
    DlLoader-->>LibManager: IDisplay*
    LibManager->>NewDisplay: init
```

---

## 8. Flux de sortie

```mermaid
sequenceDiagram
    participant Core
    participant SystemCommand
    participant LibManager
    participant IGame
    participant IDisplay
    participant DlLoader

    Note over Core: Input EXIT reçu

    Core->>SystemCommand: handleSystemEvent
    SystemCommand->>Core: onExitRequested
    Note over Core: _running = false

    Note over Core: Fin de boucle,<br/>destruction de Core

    Core->>LibManager: ~LibManager
    LibManager->>IGame: stop
    LibManager->>IGame: delete
    LibManager->>IDisplay: stop
    LibManager->>IDisplay: delete
    LibManager->>DlLoader: ~DlLoader
```

---

## 9. Description des composants

### `Core`

Point d'entrée de la logique applicative. Orchestre la boucle principale et connecte les callbacks de `SystemCommand` aux actions de `LibManager`.

| Méthode | Rôle |
|---|---|
| `run(path)` | Initialise la display, scanne `./lib`, charge le menu, lance la boucle |
| `menu()` | Recharge `arcade_menu.so` comme jeu courant |
| `loadGame(path)` | Délègue à `LibManager::loadGame` |
| `loadDisplay(path)` | Délègue à `LibManager::loadDisplay` |

---

### `LibManager`

Gère le cycle de vie des plugins (chargement, déchargement, rotation).

| Méthode | Rôle |
|---|---|
| `scanLibs(path)` | Parcourt `./lib`, classe chaque `.so` via `hasSymbol` |
| `loadGame(path)` | Décharge l'ancien jeu, charge le nouveau via `DlLoader<IGame>` |
| `loadDisplay(path)` | Décharge l'ancienne display, charge la nouvelle via `DlLoader<IDisplay>` |
| `nextGame()` | Rotation circulaire dans `_games[]` |
| `nextDisplay()` | Rotation circulaire dans `_displays[]` |

---

### `DlLoader<T>`

Encapsulation C++ de `libdl`. Générique sur le type d'interface.

| Méthode | libdl sous-jacent |
|---|---|
| `loadLib(path)` | `dlopen(path, RTLD_LAZY)` |
| `getInstance(sym)` | `dlsym(handle, sym)` + cast + appel |
| `unloadLib()` | `dlclose(handle)` |
| `hasSymbol(path, sym)` | `dlopen` + `dlsym` + `dlclose` (statique) |

> Le flag `-fno-gnu-unique` (GCC uniquement) est nécessaire pour que `dlclose` décharge
> effectivement la lib. Sans lui, les symboles `GNU_UNIQUE` restent en mémoire.

---

### `SystemCommand`

Traduit les `Input` système en appels de callbacks enregistrés par `Core`.

| Input | Callback déclenché |
|---|---|
| `EXIT` | `onExitRequested` → `_running = false` |
| `MENU` | `onMenuRequested` → charge `arcade_menu.so` |
| `NEXTGAME` | `onNextGameRequested` → `libManager.nextGame()` |
| `NEXTDISPLAY` | `onNextDisplayRequested` → `libManager.nextDisplay()` |

La table de dispatch est un `constexpr std::array` de paires `(Input, méthode*)`, itérée à chaque frame.

---

## 10. Données partagées

Ces structures sont définies dans `src/shared/` et constituent le **contrat de données** entre le core, les jeux et les displays.

### `Input`

```
NONE | UP | DOWN | LEFT | RIGHT | ACTION | MENU | EXIT | RESTART | NEXTGAME | NEXTDISPLAY
```

### `Entity`

```cpp
struct Entity {
    Position position;   // x, y, z (optionnel)
    char asciiChar;      // rendu fallback texte
    string texturePath;  // rendu graphique
    array<uint8_t,4> color; // RGBA
    bool isPlayer;
};
```

### `Text`

```cpp
struct Text {
    Position position;
    string text;
    array<uint8_t,4> color; // RGBA
};
```

---

## 11. Implémenter une nouvelle librairie

### Librairie de jeu

1. Hériter de `AGame`
2. Implémenter `init`, `stop`, `restart`, `update`
3. Utiliser `addEntity`, `addText`, `setScore`, `setIsGameOver` pour exposer l'état
4. Exporter `extern "C" IGame* createGame()`
5. Compiler en `SHARED` → `lib/arcade_<nom>.so`

### Librairie graphique

1. Hériter de `ADisplay`
2. Implémenter `init`, `stop`, `getEvent`, `clear`, `drawEntity`, `drawText`, `display`
3. Appeler `setName(...)` dans `init`
4. Exporter `extern "C" IDisplay* createDisplay()`
5. Compiler en `SHARED` → `lib/arcade_<nom>.so`

### Détection automatique

`LibManager::scanLibs` détecte automatiquement le type via :
- `hasSymbol(path, "createGame")` → classé comme jeu
- `hasSymbol(path, "createDisplay")` → classé comme display
