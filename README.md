# AwesomeModbusMaster
A cross-plattform Modbus master (client) with register scanning and logging functionality (the perfect tool for anyone dealing with incomplete/inaccurate Modbus device specs)

## Feature set / Why another Modbus master?

### Scanning/detection features

When connecting to third-party devices with Modbus interface, often we face the problem of inaccurate/incomplete specifications, for example:

- mixup of PLC addresses like (Register 40351) and Modbus address 0350 with function code 03  (off-by-one error, as PLC addresses are 1-based, wheres Modbus register count starts with 0)
- mixup/invalid coding for 32-bit types, for example specs say _float 32-bit little endian byte swap_  but in fact we get "Float cdab" encoding
- slave ID is unknown or set to a different value (for example, when the default slave ID had been changed and someone forgot to note the new slave ID...)
- serial connection settings are unclear (well, manually testing baud rate, parity, stop bits etc. is time consuming, how about letting the software manually scan for a valid combination)

So, main feature of the software is to determine correct modbus settings and find out, which registers are in use, and possibly also the correct encoding of 32-bit/64-bit multi-register values.

### Logging feature

The software allows continuous polling of designated Modbus registers (or multi-register values), plotting the time evolution of these values or showing tabulated values, and logging these to file.
Either starting from the scanning window, you can directly select the values of interest and add them to the list of monitored values, or manually specify function code, register and encoding to watch.
Also, the polling interval can be selected as well as auto-reconnect (for example, if you do longer Modbus logging and the connection to the device is temporarily lost, you would want to continue logging,
as soon as the connection can be re-established, right)

### Target group

- engineers and PLC programmers
- automatition engineers


## Download/Installation

- see releases page or download section on webpage

## Building

- cmake build, see scripts in subdirectory `build`

## Licensing

Open-Source tool, free of charge, but feel free to use the _Donate_ button on the webpage.


