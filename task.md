Requirements:
- Zephyr OS
- Firmware that prints received CAN messages to a UART (COM Port, UART Pins Tx, Rx, Serial Port /dev/ttyUSB0..)
- Each printed entry must include a timestamp, CAN ID, DLC, and data.
  - Example: 2026-05-28 12:34:56.789 ID=0x123 DLC=8 DATA=DE AD BE EF 00 11 22 33
- Build-time configurable: CAN peripheral settings, task periods, and CAN IDs.
- Three periodic TX messages with build-time configurable periods; payloads must be
	randomly generated.
- RX message ID to start printing; if undefined, start immediately.
- RX message ID to stop printing; if undefined, continue indefinitely.
- RX message ID that, when received, causes the device to print "hello world".
- Unit Test