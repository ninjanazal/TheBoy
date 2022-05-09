#include "cpu.h"

namespace TheBoy {
	/**
	 * @brief Construct a new Cpu object
	 */
	Cpu::Cpu(EmulatorController* ctrl) : emuCtrl(ctrl) {
		regs = std::make_shared<Registers>();
		reset();
		std::cout << "[CPU] ::: Cpu has been created!" << std::endl;
	}


	/**
	 * @brief Resets the current cpu state
	 */
	void Cpu::reset() {
		regs->reset();
		cpuHLT = false;
		currInstruct = nullptr;
		currOpcode = 0x0;

		std::cout << "[CPU] ::: Reseted" << std::endl;

	}

	/**
	 * @brief Defines the cpu Program counter start value
	 * @param entry Defined program count value
	 */
	void Cpu::setPCEntry(bit16 entry){
		regs->PC = entry;
	}

	/**
	 * @brief Defines the cpu iteration
	 */
	void Cpu::step(){
		if(!cpuHLT){
			fetch_inst();
			fetch_data();
			executeInst();
		}
	}


	/**
	 * @brief Get the Register Value object
	 * @param regType Defined register to get
	 * @return bit16 Data on the setted register
	 */
	bit16 Cpu::getRegisterValue(RegisterType regType){
		switch (regType) {
		case REG_A: return regs->A;
		case REG_F: return regs->F;
		case REG_B: return regs->B;
		case REG_C: return regs->C;
		case REG_D: return regs->D;
		case REG_E: return regs->E;
		case REG_H: return regs->H;
		case REG_L: return regs->L;
		
		case REG_SP: return regs->SP;
		case REG_PC: return regs->PC;

		case REG_AF: return TheBoy::reverse16(*((bit16 *)&regs->A));
		case REG_BC: return TheBoy::reverse16(*((bit16 *)&regs->B));
		case REG_DE: return TheBoy::reverse16(*((bit16 *)&regs->D));
		case REG_HL: return TheBoy::reverse16(*((bit16 *)&regs->H));
	
		default: return 0x0;
		}
	}

		/**
	 * @brief Get the Registers pointer
	 * @return const Registers* Pointer to the registers
	 */
	Registers* Cpu::getRegisters() {
		return regs.get();
	}


	/**
	 * @brief Get the Interrupt value
	 * @return true/False Current interrupt state
	 */
	bool Cpu::getInterrupt() {
		return interruptState;
	}

	/**
	 * @brief Set the Interrupt State value
	 * @param iState New interrupt state value
	 */
	void Cpu::setInterruptState(bool iState) {
		interruptState = iState;
	}


	/**
	 * @brief Gets the current Z flag value
	 * @return bit8 Current z value
	 */
	bit8 Cpu::getZFlag(){
		return (GETBIT(regs->F, 7));
	}
	

	/**
	 * @brief Gets the current C flag value
	 * @return bit8 Current C value
	 */
	bit8 Cpu::getCFlag() {
		return (GETBIT(regs->F, 4));
	}


	/**
	 * @brief Get the Curr Instruct object
	 * @return Instruc* Pointer to the current instruction
	 */
	Instruc* Cpu::getCurrInstruct() {
		return currInstruct;
	}


	/**
	 * @brief Get the Fetched Data value
	 * @return bit16 Fetched data value
	 */
	bit16 Cpu::getFetchedData(){
		return intMem.fetchData;
	}


	/**
	 * @brief Updates the cpu program counter to the current fetched data
	 */
	void Cpu::updatePCtoFetched() {
		regs->PC = intMem.fetchData;
	}


	/**
	 * @brief Requests the emulator controller to emulate a defined cycle value
	 * @param cycles Cycle value
	 */
	void Cpu::requestCycles(const int& cycles) {
		emuCtrl->emulCycles(cycles);
	}


	/**
	 * @brief Defined fetch instuction from memory
	 */
	void Cpu::fetch_inst() {
		currOpcode = emuCtrl->getBus()->abRead(regs->PC);
		currInstruct = TheBoy::getByOpcode(currOpcode);
		
		if(currInstruct == nullptr){
			printf("[CPU] ::: Opcode %2.2X failed to load\n", currOpcode);
			fflush(stdout);
		}

		printf("[CPU] ::: ->  OPCODE: %2.2X | PC: %2.2X\n", currOpcode, regs->PC);
		fflush(stdout);

		regs->PC++;
	}

	/**
	 * @brief Defines fetch data for the loaded instruction
	 */
	void Cpu::fetch_data() {
		intMem.fetchData = 0x0;
		intMem.destIsMem = false;

		switch (currInstruct->opMode) {
		case OPMODE_NONE: 	// No memory operation needed			
			break;

		case OPMODE_R:		// Memory operation only over a register
			intMem.fetchData = getRegisterValue(currInstruct->regTypeL);
			break;


		case OPMODE_R_V8:	// Memory operation on a register and 8bit value
			intMem.fetchData = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);

			regs->PC++;
			break;

		case OPMODE_V16: {	// Memory operation on a 16bit value
			bit16 low = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);
			regs->PC++;
			
			bit16 high = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);


			intMem.fetchData = low | (high << 8);
			regs->PC++;
			break;
		}

		default:			// If none, this is a unknow operation mode
			char* m(new char[128] {});
			sprintf(m, "[CPU] ::: Unknown Operation mode on the instruction [OPCODE: %2.2X]\n", currOpcode);
			emuCtrl->forceEmuStop(m);
			delete[] m;
			break;
		}
	}

		/**
	 * @brief Defines the execution process for the instruction with the data
	 */
	void Cpu::executeInst() {
		CpuFuncs::INST_FUNC exe = CpuFuncs::getInstructProcess(currInstruct->insType);
		if(!exe){
			char* m(new char[128] {});
			sprintf(m, "[CPU] ::: Unknown execution function for [OPCODE: %2.2X]\n", currOpcode);
			emuCtrl->forceEmuStop(m);
			delete[] m;
			return;
		}
		exe(this);
	}
}