###############
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 by Waldorf Electronics
 * Copyright (C) 1995 - 2000, 01, 03 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 * Copyright (C) 2007  Maciej W. Rozycki
 */
#include <linux/module.h>
#include <linux/param.h>
#include <linux/smp.h>

#include <asm/compiler.h>
#include <asm/war.h>

inline void __delay(unsigned int loops)
{
	__asm__ __volatile__ (
	"	.set	noreorder				\n"
	"	.align	3					\n"
	"1:	bnez	%0, 1b					\n"
	"	subu	%0, 1					\n"
	"	.set	reorder					\n"
	: "=r" (loops)
	: "0" (loops));
}
EXPORT_SYMBOL(__delay);

/*
 * Division by multiplication: you don't have to worry about
 * loss of precision.
 *
 * Use only for very small delays ( < 1 msec).  Should probably use a
 * lookup table, really, as the multiplications take much too long with
 * short delays.  This is a "reasonable" implementation, though (and the
 * first constant multiplications gets optimized away if the delay is
 * a constant)
 */

void __udelay(unsigned long us)
{
	unsigned int lpj = raw_current_cpu_data.udelay_val;

	__delay((us * 0x000010c7ull * HZ * lpj) >> 32);
}
EXPORT_SYMBOL(__udelay);

void __ndelay(unsigned long ns)
{
	unsigned int lpj = raw_current_cpu_data.udelay_val;

	__delay((ns * 0x00000005ull * HZ * lpj) >> 32);
}
EXPORT_SYMBOL(__ndelay);


############

#define raw_current_cpu_data cpu_data[raw_smp_processor_id()]

cpu_data[cpu].udelay_val = loops_per_jiffy;


./init/main.c:239:unsigned long loops_per_jiffy = (1<<12);
./init/main.c:241:EXPORT_SYMBOL(loops_per_jiffy);

./drivers/cpufreq/cpufreq.c:274: * adjust_jiffies - adjust the system "loops_per_jiffy"
./drivers/cpufreq/cpufreq.c:276: * This function alters the system "loops_per_jiffy" for the clock
./drivers/cpufreq/cpufreq.c:277: * speed change. Note that loops_per_jiffy cannot be updated on SMP
./drivers/cpufreq/cpufreq.c:279: * per-CPU loops_per_jiffy value wherever possible.
./drivers/cpufreq/cpufreq.c:291:		l_p_j_ref = loops_per_jiffy;
./drivers/cpufreq/cpufreq.c:293:		dprintk("saving %lu as reference value for loops_per_jiffy; "
./drivers/cpufreq/cpufreq.c:299:		loops_per_jiffy = cpufreq_scale(l_p_j_ref, l_p_j_ref_freq,
./drivers/cpufreq/cpufreq.c:301:		dprintk("scaling loops_per_jiffy to %lu "
./drivers/cpufreq/cpufreq.c:302:			"for frequency %u kHz\n", loops_per_jiffy, ci->new);

./Documentation/cpu-freq/core.txt:35:kernel "constant" loops_per_jiffy is updated on frequency changes
./Documentation/kernel-parameters.txt:1289:			Sets loops_per_jiffy to given constant, thus avoiding
./Module.symvers:2149:0x00000000	loops_per_jiffy	vmlinux	EXPORT_SYMBOL

