/**
 * struct spi_device - Controller side proxy for an SPI slave device
 * @dev: Driver model representation of the device.
 * @controller: SPI controller used with the device.
 * @master: Copy of controller, for backwards compatibility.
 * @max_speed_hz: Maximum clock rate to be used with this chip
 *	(on this board); may be changed by the device's driver.
 *	The spi_transfer.speed_hz can override this for each transfer.
 * @chip_select: Chipselect, distinguishing chips handled by @controller.
 * @mode: The spi mode defines how data is clocked out and in.
 *	This may be changed by the device's driver.
 *	The "active low" default for chipselect mode can be overridden
 *	(by specifying SPI_CS_HIGH) as can the "MSB first" default for
 *	each word in a transfer (by specifying SPI_LSB_FIRST).
 * @bits_per_word: Data transfers involve one or more words; word sizes
 *	like eight or 12 bits are common.  In-memory wordsizes are
 *	powers of two bytes (e.g. 20 bit samples use 32 bits).
 *	This may be changed by the device's driver, or left at the
 *	default (0) indicating protocol words are eight bit bytes.
 *	The spi_transfer.bits_per_word can override this for each transfer.
 * @rt: Make the pump thread real time priority.
 * @irq: Negative, or the number passed to request_irq() to receive
 *	interrupts from this device.
 * @controller_state: Controller's runtime state
 * @controller_data: Board-specific definitions for controller, such as
 *	FIFO initialization parameters; from board_info.controller_data
 * @modalias: Name of the driver to use with this device, or an alias
 *	for that name.  This appears in the sysfs "modalias" attribute
 *	for driver coldplugging, and in uevents used for hotplugging
 * @driver_override: If the name of a driver is written to this attribute, then
 *	the device will bind to the named driver and only the named driver.
 *	Do not set directly, because core frees it; use driver_set_override() to
 *	set or clear it.
 * @cs_gpiod: gpio descriptor of the chipselect line (optional, NULL when
 *	not using a GPIO line)
 * @word_delay: delay to be inserted between consecutive
 *	words of a transfer
 * @cs_setup: delay to be introduced by the controller after CS is asserted
 * @cs_hold: delay to be introduced by the controller before CS is deasserted
 * @cs_inactive: delay to be introduced by the controller after CS is
 *	deasserted. If @cs_change_delay is used from @spi_transfer, then the
 *	two delays will be added up.
 * @pcpu_statistics: statistics for the spi_device
 *
 * A @spi_device is used to interchange data between an SPI slave
 * (usually a discrete chip) and CPU memory.
 *
 * In @dev, the platform_data is used to hold information about this
 * device that's meaningful to the device's protocol driver, but not
 * to its controller.  One example might be an identifier for a chip
 * variant with slightly different functionality; another might be
 * information about how this particular board wires the chip's pins.
 */
struct spi_device {
	struct device		dev;
	struct spi_controller	*controller;
	struct spi_controller	*master;	/* Compatibility layer */
	u32			max_speed_hz;
	u8			chip_select;
	u8			bits_per_word;
	bool			rt;
#define SPI_NO_TX		BIT(31)		/* No transmit wire */
#define SPI_NO_RX		BIT(30)		/* No receive wire */
	/*
	 * TPM specification defines flow control over SPI. Client device
	 * can insert a wait state on MISO when address is transmitted by
	 * controller on MOSI. Detecting the wait state in software is only
	 * possible for full duplex controllers. For controllers that support
	 * only half-duplex, the wait state detection needs to be implemented
	 * in hardware. TPM devices would set this flag when hardware flow
	 * control is expected from SPI controller.
	 */
#define SPI_TPM_HW_FLOW		BIT(29)		/* TPM HW flow control */
	/*
	 * All bits defined above should be covered by SPI_MODE_KERNEL_MASK.
	 * The SPI_MODE_KERNEL_MASK has the SPI_MODE_USER_MASK counterpart,
	 * which is defined in 'include/uapi/linux/spi/spi.h'.
	 * The bits defined here are from bit 31 downwards, while in
	 * SPI_MODE_USER_MASK are from 0 upwards.
	 * These bits must not overlap. A static assert check should make sure of that.
	 * If adding extra bits, make sure to decrease the bit index below as well.
	 */
#define SPI_MODE_KERNEL_MASK	(~(BIT(29) - 1))
	u32			mode;
	int			irq;
	void			*controller_state;
	void			*controller_data;
	char			modalias[SPI_NAME_SIZE];
	const char		*driver_override;
	struct gpio_desc	*cs_gpiod;	/* Chip select gpio desc */
	struct spi_delay	word_delay; /* Inter-word delay */
	/* CS delays */
	struct spi_delay	cs_setup;
	struct spi_delay	cs_hold;
	struct spi_delay	cs_inactive;

	/* The statistics */
	struct spi_statistics __percpu	*pcpu_statistics;

	/*
	 * likely need more hooks for more protocol options affecting how
	 * the controller talks to each chip, like:
	 *  - memory packing (12 bit samples into low bits, others zeroed)
	 *  - priority
	 *  - chipselect delays
	 *  - ...
	 */
};
==============================================================