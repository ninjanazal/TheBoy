# TheBoy
GameBoy color emulator in c++  , created for a case study

### Usefull links
- [gddev](https://www.gbdev.io/) (Game Boy Development community)
- [Gameboy CPU (LR35902) instruction set](https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html)
- [Game Boy: Complete Technical Reference by gekkio](https://gekkio.fi/files/gb-docs/gbctr.pdf)
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
> 	- C/C++ configuration settings exemple
> ---
>	```json
>	{
>	    "configurations": [
>	        {
>	            "name": "Win32",
>	            "includePath": [
>	                "${workspaceFolder}/**",
>					"${workspaceFolder}/out/include/**"
>	            ],
>	            "defines": [
>	                "_DEBUG",
>	                "UNICODE",
>	                "_UNICODE"
>	            ],
>	            "windowsSdkVersion": "10.0.19041.0",
>	            //"compilerPath": ".../Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.31.31103/bin/Hostx64/x64/cl.exe",
>	            "compilerPath": "...\\mingw64\\bin\\gcc.exe",
>	            "cStandard": "c17",
>	            "cppStandard": "c++17",
>	            //"intelliSenseMode": "windows-msvc-x64",
>				"intelliSenseMode": "gcc-x64",
>	            "configurationProvider": "ms-vscode.cmake-tools"
>	        }
>	    ],
>	    "version": 4
>	}
>	```


---

# Build Script
Use this script to gather all the information needed, compile e execute;

### Available parameters

```./buildme.sh -h```
- *-r (--run)       | Will run the compilation result
- *-c (--clearall)  | Will clear the previous compilation and rebuild
- *-t (--target=)   | Changes the target generator
	- Current vailable generators: mingw

---

## Current Development state

- [x] Cartridge header map
	- [x] Cartridge memory layout 
- [x] Address Bus implementation
	- [x] Bus functions read/write
	- [x] Addres bus major memory values maped
	- [x] Read/Write 16bit version functions
- [ ] Cpu Implementation
	- [ ] OPCode translation (150 / 256)
	- [ ] Instruction implementation (doing)
	- [x] Stack implementation (Push/Pop 8&16bit)
- [x] Ram
	- [x] Working RAM allocation and operations
	- [x] High RAM allocation and operations


---
### ScreenShots
``` Cartridge header out ```
![Cartridge Header](prtSc/CartridgeHeaderMapPNG.PNG)

``` OpCode Execution (JP) ```
![Cartridge Header](prtSc/JumpInstructionCpuExecution.PNG)