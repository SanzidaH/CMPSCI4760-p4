# CMPSCI-4760 P4

## Project Goal:

* Simulating the process scheduling part of an operating system.
* Using message Queue.

## How to run
Commands to run project from hoare server:

```
$ make
$ ./oss 
```

## Git

https://github.com/SanzidaH/CMPSCI4760-p4.git

## Task Summary

> OSS will spawn child processes.
> OSS will schedule processes using multi level round robin. 
> OSS and child processes will exchange messages using message queue.

## Update
> Process and OSS are just communicating thorugh message. It gets some error at some point though. But, atleast OSS and process are exchanging messages for sometime. I decided to work on this project again so that I can get comfortable with message queue (but still not fully). 
> The program receives alarm signal way earlier in hoare than my machine, even after I increase the termination time. The output file is also different in my machine and hoare machine. It seems like it does not progress at all (PID is not increasing much), but it progress in my machine. I am not sure what can be the reason.  
