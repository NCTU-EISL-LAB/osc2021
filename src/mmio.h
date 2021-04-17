#ifndef MMIO_H
#define MMIO_H

#define MMIO_BASE 0x3F000000

#define AUX_IRQ_REG (MMIO_BASE + 0x215000)
#define AUX_ENABLES_REG (MMIO_BASE + 0x215004)
#define AUX_MU_IO_REG   (MMIO_BASE + 0x215040)
#define AUX_MU_IER_REG  (MMIO_BASE + 0x215044)
#define AUX_MU_IIR_REG  (MMIO_BASE + 0x215048)
#define AUX_MU_LCR_REG  (MMIO_BASE + 0x21504C)
#define AUX_MU_MCR_REG  (MMIO_BASE + 0x215050)
#define AUX_MU_LSR_REG  (MMIO_BASE + 0x215054)
#define AUX_MU_MSR_REG  (MMIO_BASE + 0x215058)
#define AUX_MU_SCRATCH  (MMIO_BASE + 0x21505C)
#define AUX_MU_CNTL_REG (MMIO_BASE + 0x215060)
#define AUX_MU_STAT_REG (MMIO_BASE + 0x215064)
#define AUX_MU_BAUD_REG (MMIO_BASE + 0x215068)
#define AUX_SPI0_CNTL0_REG  (MMIO_BASE + 0x215080)
#define AUX_SPI0_CNTL1_REG  (MMIO_BASE + 0x215084)
#define AUX_SPI0_STAT_REG   (MMIO_BASE + 0x215088)
#define AUX_SPI0_IO_REG (MMIO_BASE + 0x215090)
#define AUX_SPI0_PEEK_REG   (MMIO_BASE + 0x215094)
#define AUX_SPI1_CNTL0_REG  (MMIO_BASE + 0x2150C0)
#define AUX_SPI1_CNTL1_REG  (MMIO_BASE + 0x2150C4)
#define AUX_SPI1_STAT_REG   (MMIO_BASE + 0x2150C8)
#define AUX_SPI1_IO_REG (MMIO_BASE + 0x2150D0)
#define AUX_SPI1_PEEK_REG   (MMIO_BASE + 0x2150D4)
#define aux(NAME) ((volatile unsigned int *)(AUX_ ## NAME ## _REG))

/* reboot */
#define PM_PASSWORD 0x5A000000
#define PM_RSTC_REG (MMIO_BASE + 0x10001C)
#define PM_WDOG_REG (MMIO_BASE + 0x100024)

/* GPIO */
#define GPFSEL0_REG (MMIO_BASE + 0x200000)
#define GPFSEL1_REG (MMIO_BASE + 0x200004)
#define GPFSEL2_REG (MMIO_BASE + 0x200008)
#define GPFSEL3_REG (MMIO_BASE + 0x20000C)
#define GPFSEL4_REG (MMIO_BASE + 0x200010)
#define GPFSEL5_REG (MMIO_BASE + 0x200014)
#define GPPUD_REG (MMIO_BASE + 0x200094)
#define GPPUDCLK0_REG (MMIO_BASE + 0x200098)

/* interrupt */
#define IRQ_BASIC_REG (MMIO_BASE + 0xB200)
#define IRQ1_REG (MMIO_BASE + 0xB204)
#define IRQ2_REG (MMIO_BASE + 0xB208)
#define FIQ_CONTROL_REG (MMIO_BASE + 0xB20C)
#define ENABLE_IRQS1_REG (MMIO_BASE + 0xB210)
#define ENABLE_IRQS2_REG (MMIO_BASE + 0xB214)
#define ENABLE_BASIC_IRQS_REG (MMIO_BASE + 0xB218)
#define DISABLE_IRQS1_REG (MMIO_BASE + 0xB21C)
#define DISABLE_IRQS2_REG (MMIO_BASE + 0xB220)
#define DISABLE_BASIC_IRQS_REG (MMIO_BASE + 0xB224)


/* timer */
#define CORE0_TIMER_IRQ_CTRL_REG 0x40000040

/* mailbox */
#define MAILBOX_READ_REG (MMIO_BASE + 0xB880)
#define MAILBOX_STATUS_REG (MMIO_BASE + 0xB898)
#define MAILBOX_WRITE_REG (MMIO_BASE + 0xB8A0)

#define mmio(NAME) ((volatile unsigned int *)(NAME ## _REG))
#endif
