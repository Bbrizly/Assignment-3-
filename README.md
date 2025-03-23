# CSC 4650 Assignment 3

Hello

# Controls:

Q to switch shoulders
C to switch cameras
Tab to unlock mouse (To press on pause and resume top right)  [alternatively P to pause]


WASD For movement
Shift for faster movement
Left Click to launch projectiles

# What I did:

- Bullet used for all collions
- Projectile interactes with game objects and collects coins. [coins identify it using an identifier projectile component]
- Character cannot walk through walls and lamppost (veocity based movement)
- Data-driven game object creation (found in src/data/scripts/)
- Collision events are called when bullet collisions happen
- Scene pauses correctly (by setting delta time to 0)
- Added teeter totter and have different collision shapes.

## Toubleshooting

### Missing DLLs

If you try and run a debug build but you immediately have missing DLL issues, try:

1. in Visual Studio, selecting `Project > Properties > Configuration Properties > Debugging`.
2. for the Working Directory field, try replacing `$(ProjectDir)` with `$(ProjectDir)/../../` depending on how many levels nested the .sln is from the root `src` directory.

### Missing MSCVR100.dll

Install: [Microsoft Visual C++ Redistributable 2012](https://www.microsoft.com/en-ca/download/details.aspx?id=30679) & [2010](https://www.microsoft.com/en-ca/download/details.aspx?id=26999)

### "Unable to start program ... cannot find the path specified"

1. in Visual Studio, rightclick on `ExampleGame` in the Solution Explorer
2. select `Build`

