# SIC-XE-system-programming Proj1. SIX/XE Machine Shell
## Sogang Univ. CSE4100 System Programming SIC/XE project
 * Writer : Yunje Lee
 * Student ID : 20141303
 * Summary : SIX/XE Machine의 기본 shell 명령어와 memory, opcode 관련 명령어를 수행합니다.

## How to compile

```
$ make
```

## How to run

```
$ ./20141303.out
```

## How to clean object files and exe

```
$make clean
```

## Commands

```
$ h[elp]    //show all valid commands
$ d[ir]   //show all files on current directory
$ q[uit]    //quit and exit program
$ hi[story]   //show history of valid commands
$ du[mp] [start, end]   //dump data from 'start' address to 'end' address
$ e[dit] address, value   //edit corresponding 'address' data with 'value'
$ f[ill] start, end, value    //fill corresponding data with 'start' and 'end' address with 'value'
$ reset   //fill whole memory with zero
$ opcode mnemonic   //output corresponding opcode with input mnemonic
$ opcodelist    //show all opcode mnemonic list
```
