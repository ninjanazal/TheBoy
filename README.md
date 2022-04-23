# TheBoy
GameBoy color emulator in c++  , created for a case study

---

# Dependencies
> ## Win
> - Bash terminar for running .sh scripts
> - Mingw (gcc compiler): [Mingw web-site](https://www.mingw-w64.org/)
>
> ## VScode
> - If using vsCode, change the c_cpp_properties for the c/c++:
> 	- Add the **`out/lib`** to the include path;
> 	- Set the Mingw `(gcc)` compiler path;
> 	- The the configurationProvider to `"ms-vscode.cpptools"`;


---

# Build Script
Use this script to gather all the information needed, compile e execute;

### Available parameters

- **all**
	- This will force a recompilation for the existing SFMl out result
- **run**
	- This will build and run the compilation result