# SFML-Physics

Un moteur de physique 2D minimaliste construit en **C++17** avec **SFML 3**.  
Le projet propose une architecture orientée composants (à la Unity) permettant de composer facilement des objets avec de la gravité, des collisions, du rendu, etc.

---

## Fonctionnalités

- **Architecture composants** : chaque `GameObject` peut embarquer n'importe quelle combinaison de composants.
- **Physique** : gravité, vélocité, rebond sur les bords de l'écran ou sur des murs circulaires.
- **Collisions AABB** : détection et callbacks configurables via `ColliderComponent`.
- **Rendu** : formes rectangulaires (`BoxRendererComponent`) et circulaires (`CircleRendererComponent`).
- **Gestion de scènes** : empilement de scènes avec `Engine::pushScene`.
- **Console de débogage** en temps réel (titre de la fenêtre).

---

## Prérequis

| Outil | Version minimale |
|-------|-----------------|
| CMake | 3.16 |
| Compilateur C++ | Support C++17 (GCC 9+, Clang 10+, MSVC 2019+) |
| Git | Pour le téléchargement automatique de SFML |

> SFML 3.0.2 est téléchargé et compilé automatiquement par CMake via `FetchContent` — aucune installation manuelle n'est nécessaire.

---

## Compilation

### Linux / macOS (Unix Makefiles)

```bash
cmake --preset linux-debug
cmake --build --preset linux-debug
./build/sfml_test
```

### Windows — Visual Studio 2022

```powershell
cmake --preset windows-vs2022
cmake --build --preset windows-vs2022-debug   # Debug
cmake --build --preset windows-vs2022-release # Release
```

Ou ouvrez directement la solution `SFML-Physics.sln` avec Visual Studio.

---

## Contrôles

| Touche | Action |
|--------|--------|
| `Espace` | Spawner un nouveau mover (balle avec gravité) |
| `F1` | Activer / désactiver la console de débogage |
| `↑` *(debug actif)* | Augmenter la gravité de +100 |
| `↓` *(debug actif)* | Diminuer la gravité de -100 |

---

## Architecture

```
src/
├── main.cpp               # Point d'entrée : config + scène
├── Ball.cpp / Ball.h      # GameObject balle (héritage)
├── Platform.cpp / Platform.h
└── Engine/
    ├── Engine.h/cpp       # Boucle principale, gestion de fenêtre
    ├── Scene.h            # Conteneur de GameObjects
    ├── GameObject.h       # Nœud de base, gestion des composants
    ├── Component.h        # Interface de composant
    ├── CollisionSystem.h  # Détection AABB O(n²)
    ├── scenes/
    │   └── PhysicsScene.h/cpp  # Scène de démonstration
    └── Composants :
        ├── TransformComponent.h       # Position / rotation
        ├── VelocityComponent.h        # Déplacement par vélocité
        ├── GravityComponent.h         # Gravité (axe Y)
        ├── ColliderComponent.h/cpp    # Hitbox AABB + callbacks
        ├── BoxRendererComponent.h     # Rendu rectangle
        ├── CircleRendererComponent.h  # Rendu cercle
        ├── RotationComponent.h        # Rotation continue
        ├── OrbitWallsBounceComponent.h      # Rebond sur les bords de la fenêtre
        ├── SpriteBoundsBounceComponent.h    # Rebond basé sur les limites du sprite
        └── CircleWallComponent.h            # Mur circulaire
```

### Cycle de vie d'un composant

```
onStart()   → appelé une fois lors du premier update
onUpdate(dt)→ appelé chaque frame (logique)
onRender()  → appelé chaque frame (dessin)
```

---

## Exemple rapide

```cpp
// Créer un objet avec gravité et rendu circulaire
auto& ball = scene.createObject("ball");
ball.addComponent<engine::TransformComponent>(sf::Vector2f{400.f, 100.f});
ball.addComponent<engine::VelocityComponent>(sf::Vector2f{150.f, 0.f});
ball.addComponent<engine::GravityComponent>(600.f);
ball.addComponent<engine::CircleRendererComponent>(20.f, sf::Color::Cyan);
ball.addComponent<engine::ColliderComponent>(sf::Vector2f{40.f, 40.f});
ball.addComponent<engine::OrbitWallsBounceComponent>();
```

---

## Licence

Ce projet est fourni à des fins éducatives et d'expérimentation. Aucune licence particulière n'est définie.
