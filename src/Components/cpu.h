#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "addressbus.h"
#include "instruc_funcs.h"


namespace TheBoy {
	class EmulatorController;
/*
	16-bit	Hi	Lo	Name/Function
	----------------------------------
	AF	A	-	Accumulator & Flags
	BC	B	C	BC
	DE	D	E	DE
	HL	H	L	HL
	SP	-	-	Stack Pointer
	PC	-	-	Program Counter/Pointer
	----------------------------------

As shown above, most registers can be accessed either as one 16-bit register,
 or as two separate 8-bit registers.

	[Flags]
	----------------------------------
	7	z	Zero flag
	6	n	Subtraction flag (BCD)
	5	h	Half Carry flag (BCD)
	4	c	Carry flag
	----------------------------------

*/
	typedef struct {
		bit8 A;
		bit8 F;
		bit8 B;
		bit8 C;
		bit8 D;
		bit8 E;
		bit8 H;
		bit8 L;
		bit16 SP;
		bit16 PC;

		/**
		 * @brief Resets the registers values
		 */
		void reset(){
			A = 0x01;
			F = B = C = D = E = H = L = 0x0;
			PC = 0x0;
			SP = 0xDFFF;
		}
	} Registers;

	/**
	 * @brief Holds the Cpu definitions
	 */
	class Cpu {
	public:
		/**
		 * @brief Construct a new Cpu object
		 */
		Cpu(EmulatorController* ctrl);


		/**
		 * @brief Destroy the Cpu object
		 */
		~Cpu() = default;

		/**
		 * @brief Defines the cpu Program counter start value
		 * @param entry Defined program count value
		 */
		void setPCEntry(bit16 entry);

		/**
		 * @brief Defines the cpu iteration
		 */
		void step();

		/**
		 * @brief Get the Register Value object
		 * @param regType Defined register to get
		 * @return bit16 Data on the setted register
		 */
		bit16 getRegisterValue(RegisterType regType);


		/**
		 * @brief Set the Register Value
		 * @param type Registor type to be set
		 * @param value Value to be set
		 */
		void setRegisterValue(RegisterType regType, bit16 value);


		/**
		 * @brief Get the Interrupt value
		 * @return true/False Current interrupt state
		 */
		bool getMasterInterrupt();


		/**
		 * @brief Set the Interrupt State value
		 * @param iState New interrupt state value
		 */
		void setInterruptMasterState(bool iState);


		/**
		 * @brief Gets the current Z flag value
		 * @return bit8 Current z value
		 */
		bit8 getZFlag();


		/**
		 * @brief Gets the current C flag value
		 * @return bit8 Current C value
		 */
		bit8 getCFlag();


		/**
		 * @brief Get the Current OP Code value
		 * @return bit8 Current OP Code value
		 */
		bit8 getCurrentOPCode();


		/**
		 * @brief Set the Flags values
		 * @param z Zero flag
		 * @param n Subtraction flag (null flag)
		 * @param h Half Carry flag
		 * @param c Carry flag
		 */
		void setFlags(bit8 z, bit8 n, bit8 h, bit8 c);


		/**
		 * @brief Get the Curr Instruct object
		 * @return Instruc* Pointer to the current instruction
		 */
		Instruc* getCurrInstruct();

		/**
		 * @brief Get the Fetched Data value
		 * @return bit16 Fetched data value
		 */
		bit16 getFetchedData();


		/**
		 * @brief Gets if the internal memory marks that fetched is memoryLoc
		 * @return true/false Is memory location
		 */
		bool getDestenyIsMem();


		/**
		 * @brief Get the Memory Dest internal memory value
		 * @return bit16 Memory destination
		 */
		bit16 getMemoryDest();


		/**
		 * @brief Updates the cpu program counter to the current fetched data
		 */
		void updatePCtoFetched();


		/**
		 * @brief Requests the emulator controller to emulate a defined cycle value
		 * @param cycles Cycle value
		 */
		void requestCycles(const int& cycles);


		/**
		 * @brief Requests Writes to a defined address
		 * @param addr Target address to be written
		 * @param val Value to be setted on the address 
		 */
		void requestBusWrite(bit16 addr, bit8 val);


		/**
		 * @brief Requests Writes to a defined address
		 * @param addr Target address to be written
		 * @param val 16bit Value to be setted on the address 
		 */
		void requestBusWrite16(bit16 addr, bit16 val);


		/**
		 * @brief Request Read to a defined addres
		 * @param addr Target read address
		 * @return bit8 Readed value
		 */
		bit8 requestBusRead(bit16 addr);

		/**
		 * @brief Get the Cpu IE Register value
		 * @return bit8 Current IE register value
		 */
		bit8 getCpuIERegister();

		/**
		 * @brief Set the Cpu IE Register value
		 * @param val New Ie register value
		 */
		void setCpuIERegister(bit8 val);


		/**
		 * @brief Push a 8bit data to the next stack pointer location, decrement the sp val
		 * @param data Data to be pushed
		 */
		void push(bit8 data);

		/**
		 * @brief Push a 16bit data to the next stack pointer location, decrement the sp val
		 * @param data Data to be pushed
		 */
		void push16(bit16 data);

		/**
		 * @brief Pop a 8bit data from the current stack pointer location, increment sp val
		 * @return bit8 Current stack pointer data location
		 */
		bit8 pop();


		/**
		 * @brief Pop a 16bit data from the current stack pointer location, increment sp val
		 * @return bit8 Current stack pointer data location
		 */
		bit16 pop16();

	private:

	/**
	 * @brief Pointer to the emulator controller
	 */
	EmulatorController* emuCtrl;


	/**
	 * @brief Shared pointer to the registers values
	 */
	std::shared_ptr<Registers> regs;

	/**
	 * @brief Defined and declared instruction memory map
	 */
	struct InternMem {
		bit16 fetchData;
		bit16 memDest;
		bool destIsMem;
	} intMem;


	/**
	 * @brief Marks if the current cpu has been halted, its in idle mode
	 */
	bool cpuHLT;

	/**
	 * @brief Marks if the Cpu is currently suspended by an interrupt handler
	 */
	bool interruptMasterState;


	/**
	 * @brief Cpu Interrupt Enable register (0xFFFF) on memory
	 */
	bit8 interruptEnable;


	/**
	 * @brief Marks the current opcode instruction
	 */
	bit8 currOpcode;


	/**
	 * @brief Pointer to the current target instruction 
	 */
	Instruc *currInstruct;


	/**
	 * @brief Resets the current cpu state
	 */
	void reset();


	/**
	 * @brief Defined fetch instuction from memory
	 */
	void fetch_inst();


	/**
	 * @brief Defines fetch data for the loaded instruction
	 */
	void fetch_data();


	/**
	 * @brief Defines the execution process for the instruction with the data
	 */
	void executeInst();
	};
}

#endif