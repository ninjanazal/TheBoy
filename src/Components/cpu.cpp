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
	 * @brief Set the Register Value
	 * @param type Registor type to be set
	 * @param value Value to be set
	 */
	void Cpu::setRegisterValue(RegisterType regType, bit16 value){
		switch (regType) {
		case REG_A: regs->A = value & 0xFF;	return;
		case REG_F: regs->F = value & 0xFF; return;
		case REG_B: regs->B = value & 0xFF; return;
		case REG_C: regs->C = value & 0xFF; return;
		case REG_D: regs->D = value & 0xFF; return;
		case REG_E: regs->E = value & 0xFF; return;
		case REG_H: regs->H = value & 0xFF; return;
		case REG_L: regs->L = value & 0xFF; return;
		
		case REG_SP: regs->SP = value; return;
		case REG_PC: regs->PC = value; return;

		case REG_AF: *((bit16 *)&regs->A) = TheBoy::reverse16(value); return;
		case REG_BC: *((bit16 *)&regs->B) = TheBoy::reverse16(value); return;
		case REG_DE: *((bit16 *)&regs->D) = TheBoy::reverse16(value); return;
		case REG_HL: *((bit16 *)&regs->H) = TheBoy::reverse16(value); return;
	
		default: return;
		}
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
	 * @brief Set the Flags values
	 * @param z Zero flag
	 * @param n Subtraction flag (null flag)
	 * @param h Half Carry flag
	 * @param c Carry flag
	 */
	void Cpu::setFlags(bit8 z, bit8 n, bit8 h, bit8 c){
		using namespace CpuFuncs;
		if(z != -1) { SETBIT(regs->F, 7, z); }
		SETBIT(regs->F, 6, n);
		SETBIT(regs->F, 5, h);
		SETBIT(regs->F, 4, c);

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
	 * @brief Gets if the internal memory marks that fetched is memoryLoc
	 * @return true/false Is memory location
	 */
	bool Cpu::getDestenyIsMem(){
		return intMem.destIsMem;
	}

	/**
	 * @brief Get the Memory Dest internal memory value
	 * @return bit16 Memory destination
	 */
	bit16 Cpu::getMemoryDest(){
		return intMem.memDest;
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
	 * @brief Writes to a defined address
	 * @param addr Target address to be written
	 * @param val Value to be setted on the address 
	 */
	void Cpu::requestBusWrite(bit16 addr, bit8 val){
		emuCtrl->getBus()->abWrite(addr, val);
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


		case OPMODE_R_R:	// Memory operation on two registers
			intMem.fetchData = getRegisterValue(currInstruct->regTypeR);
			break;


		case OPMODE_R_V8:	// Memory operation on a register and 8bit value
			intMem.fetchData = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);

			regs->PC++;
			break;

		case OPMODE_R_V16:	// Memory operation on a register and 16biy value
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

		case OPMODE_V8:		// Memory operation on 8bit value
			intMem.fetchData = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);
			regs->PC++;
			break;
		

		case OPMODE_AR_R:	// Memory operation on registor adress and registor
			intMem.fetchData = getRegisterValue(currInstruct->regTypeR);
			intMem.memDest = getRegisterValue(currInstruct->regTypeL);
			intMem.destIsMem = true;

			/*
			For register C
Load to the address specified by the 8-bit C register, data from the 8-bit A register. The full 16-bit absolute
address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of C,
so the possible range is 0xFF00-0xFFFF.
			*/
			if(currInstruct->regTypeL == REG_C){
				intMem.memDest |= 0xFF00;
			}
			break;


		case OPMODE_R_AR: {	// Memory operation on a registor and a  memory registor address
			bit16 address = getRegisterValue(currInstruct->regTypeR); 

			/*
			For register C
Load to the address specified by the 8-bit C register, data from the 8-bit A register. The full 16-bit absolute
address is obtained by setting the most significant byte to 0xFF and the least significant byte to the value of C,
so the possible range is 0xFF00-0xFFFF.
			*/
			if(currInstruct->regTypeL == REG_C){
				address |= 0xFF;
			}
			intMem.fetchData = emuCtrl->getBus()->abRead(address);
			emuCtrl->emulCycles(1);
			break;
		}

		case OPMODE_R_HLI:	// memory operation on a registor and the HL register, incrementing
			intMem.fetchData = emuCtrl->getBus()->abRead(currInstruct->regTypeL);
			emuCtrl->emulCycles(1);
			setRegisterValue(REG_HL, getRegisterValue(REG_HL) + 0x01);
			break;


		case OPMODE_R_HLD:	// Memory operation on registor and the HL register, decrementing
			intMem.fetchData = emuCtrl->getBus()->abRead(currInstruct->regTypeL);
			emuCtrl->emulCycles(1);
			setRegisterValue(REG_HL, getRegisterValue(REG_HL) - 0x01);
			break;


		case OPMODE_HLI_R:	// Memory operation on HL register from register, incrementing
			intMem.fetchData = emuCtrl->getBus()->abRead(currInstruct->regTypeR);
			intMem.memDest = emuCtrl->getBus()->abRead(currInstruct->regTypeL);
			intMem.destIsMem = true;
			setRegisterValue(REG_HL, getRegisterValue(REG_HL) + 0x01);
			break;


		case OPMODE_HLD_R:	// Memory operation on HL register from register, decrementing
			intMem.fetchData = emuCtrl->getBus()->abRead(currInstruct->regTypeR);
			intMem.memDest = emuCtrl->getBus()->abRead(currInstruct->regTypeL);
			intMem.destIsMem = true;
			setRegisterValue(REG_HL, getRegisterValue(REG_HL) - 0x01);
			break;


		case OPMODE_R_A8: 	// Memory operation on registor from 8bit memory address
			intMem.fetchData = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);
			regs->PC++;
			break;

		case OPMODE_A8_R:	// Memory operation on 8bit address to registor
			intMem.memDest = emuCtrl->getBus()->abRead(regs->PC) | 0xFF00; // 8bit address value
			intMem.destIsMem = true;
			emuCtrl->emulCycles(1);
			regs->PC++;
			break;


		case OPMODE_HL_SPR:	// Memory operation on SP and Hl registers
			intMem.fetchData = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);
			regs->PC++;
			break;


		case OPMODE_A16_R:{	// Memory operation on registor to 16bit address
			bit16 low = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);
			regs->PC++;
			
			bit16 high = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);

			intMem.memDest = low | (high << 8);~
			regs->PC++;

			intMem.destIsMem = true;
			intMem.fetchData = emuCtrl->getBus()->abRead(currInstruct->regTypeR);
			emuCtrl->emulCycles(1);
			break;
		}

		case OPMODE_AR_V8:	// Memory operation on 8bit value to registor address
			intMem.fetchData = emuCtrl->getBus()->abRead(regs->PC);
			emuCtrl->emulCycles(1);
			regs->PC++;
			intMem.memDest = getRegisterValue(currInstruct->regTypeL);
			intMem.destIsMem = true;
			break;

		case OPMODE_AR:		// Memory operation on registor address
			intMem.memDest = getRegisterValue(currInstruct->regTypeL);
			intMem.destIsMem = true;
			intMem.fetchData = emuCtrl->getBus()->abRead(intMem.memDest);
			emuCtrl->emulCycles(1);
			break;
		


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
		if(!exe || currInstruct->insType == INST_NONE){
			char* m(new char[128] {});
			sprintf(m, "[CPU] ::: Unknown execution function for [OPCODE: %2.2X]\n", currOpcode);
			emuCtrl->forceEmuStop(m);
			delete[] m;
			return;
		}
		exe(this);
	}
}