#include <gxx/debug/dprint.h>
#include "asm/debug/assembler.h"

struct __register_map debug_regmap;

void dasm_regmap_print()
{
	debug_print("fixed_r0_tmp:\t");debug_printhex_uint8(debug_regmap.regs[0]);dln();
	debug_print("fixed_r1_null:\t");debug_printhex_uint8(debug_regmap.regs[1]);dln();

	debug_print("saved_r2:\t");debug_printhex_uint8(debug_regmap.regs[2]);dln();
	debug_print("saved_r3:\t");debug_printhex_uint8(debug_regmap.regs[3]);dln();
	debug_print("saved_r4:\t");debug_printhex_uint8(debug_regmap.regs[4]);dln();
	debug_print("saved_r5:\t");debug_printhex_uint8(debug_regmap.regs[5]);dln();
	debug_print("saved_r6:\t");debug_printhex_uint8(debug_regmap.regs[6]);dln();
	debug_print("saved_r7:\t");debug_printhex_uint8(debug_regmap.regs[7]);dln();
	debug_print("saved_r8:\t");debug_printhex_uint8(debug_regmap.regs[8]);dln();
	debug_print("saved_r9:\t");debug_printhex_uint8(debug_regmap.regs[9]);dln();
	debug_print("saved_r10:\t");debug_printhex_uint8(debug_regmap.regs[10]);dln();
	debug_print("saved_r11:\t");debug_printhex_uint8(debug_regmap.regs[11]);dln();
	debug_print("saved_r12:\t");debug_printhex_uint8(debug_regmap.regs[12]);dln();
	debug_print("saved_r13:\t");debug_printhex_uint8(debug_regmap.regs[13]);dln();
	debug_print("saved_r14:\t");debug_printhex_uint8(debug_regmap.regs[14]);dln();
	debug_print("saved_r15:\t");debug_printhex_uint8(debug_regmap.regs[15]);dln();
	debug_print("saved_r16:\t");debug_printhex_uint8(debug_regmap.regs[16]);dln();
	debug_print("saved_r17:\t");debug_printhex_uint8(debug_regmap.regs[17]);dln();

	debug_print("used_r18:\t");debug_printhex_uint8(debug_regmap.regs[18]);dln();
	debug_print("used_r19:\t");debug_printhex_uint8(debug_regmap.regs[19]);dln();
	debug_print("used_r20:\t");debug_printhex_uint8(debug_regmap.regs[20]);dln();
	debug_print("used_r21:\t");debug_printhex_uint8(debug_regmap.regs[21]);dln();
	debug_print("used_r22:\t");debug_printhex_uint8(debug_regmap.regs[22]);dln();
	debug_print("used_r23:\t");debug_printhex_uint8(debug_regmap.regs[23]);dln();
	debug_print("used_r24:\t");debug_printhex_uint8(debug_regmap.regs[24]);dln();
	debug_print("used_r25:\t");debug_printhex_uint8(debug_regmap.regs[25]);dln();

	debug_print("used_r26_xlo:\t");debug_printhex_uint8(debug_regmap.regs[26]);dln();
	debug_print("used_r27_xhi:\t");debug_printhex_uint8(debug_regmap.regs[27]);dln();
	debug_print("saved_r28_y_lo:\t");debug_printhex_uint8(debug_regmap.regs[28]);dln();
	debug_print("saved_r29_y_hi:\t");debug_printhex_uint8(debug_regmap.regs[29]);dln();
	debug_print("used_r30_z_lo:\t");debug_printhex_uint8(debug_regmap.regs[30]);dln();
	debug_print("used_r31_z_hi:\t");debug_printhex_uint8(debug_regmap.regs[31]);dln();
	
	debug_print("sreg:\t\t");debug_printhex_uint8(debug_regmap.regs[32]);dln();
	debug_print("spl:\t\t");debug_printhex_uint8(debug_regmap.spl);dln();
	debug_print("sph:\t\t");debug_printhex_uint8(debug_regmap.sph);dln();
	debug_print("ret:\t\t");debug_printhex_uint16(debug_regmap.ret);dln();
};
