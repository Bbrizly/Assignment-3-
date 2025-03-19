# CSC 4650 Code

Included in this repository is the weekly codebase for CSC4650.

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
