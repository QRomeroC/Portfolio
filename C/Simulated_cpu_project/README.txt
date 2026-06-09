This directory contains all folders and files pertaining to a simulated cpu project.
Project culmenates in sim5 which is the fully flushed out cpu.

sim1 - simulates a hardware a 32-bit ripple carry adder/subtractor, simulates digital logic gates.
		 - processes all 32 bits one at a time
		 - Uses XOR lgic to compute each sum bit
		 - tracks ripple carry
		 - does addition and subtraction
		 output:
			sum
			carry-out flag
			sign information
			signed overflow flag
		fondations of a ALU

sim2, sim3 - a assortment of simulated logic gates: and, not, or, nand, and xor. additionally
			 uses gates to simulate half adders and full adders.

sim4 - implements pieces of a single-cycle MIPS CPU simulator. Decodes instructions, sets controls
       signals, runs ALU/Memory behavior, updates registers, and caluclates the next PC.
	   Simulates main stages of a MIPS processor: fetch, decode, execute, memory, write-back, and updating pc.	   

sim5 - similar to sim4, but implements a pipeline, utilizing pipeline-register structs to store information between
       cycles. the same five stages are simulated: instruction decode, execute, memory, and write back with additional hazard 
	   mitigation. supports fowarding, stalls, branches/jumps, memory read/write, and register updating.