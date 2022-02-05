# ECO

A solution for [2021 ICCAD contest problem A].

[2021 ICCAD contest problem A]: http://iccad-contest.org/2021/Problems/Problem_A_20210224_0208.pdf

# Get Started with ECO

You need a C++ compiler with C++17 support, [GNU Bison] and [Flex] to compile [Parser-Verilog].
```bash
~$ flex -o./verilog_lexer.yy.cc parser-verilog/verilog_lexer.l 
~$ bison -d -o verilog_parser.tab.cc parser-verilog/verilog_parser.yy
```

 * Note: If you are a MacOS user, you may need to delete all the keyword `register` in `verilog_lexer.yy.cc`.

[GNU Bison]:           https://www.gnu.org/software/bison/
[Flex]:                https://github.com/westes/flex 
[Parser-Verilog]:      https://github.com/OpenTimer/Parser-Verilog

You also need a lirary of [abc].
To compile ABC as a static library, type `make libabc.a`.
```bash
~$ git clone https://github.com/berkeley-abc/abc.git
~$ cd abc
~$ make libabc.a
~$ cd ..
```

[abc]:  https://github.com/berkeley-abc/abc

To build the program
 * Copy abc.rc and libabc.a to the working directory
 * Run `g++ -std=c++17 -Iparser-verilog/ -I. verilog_parser.tab.cc verilog_lexer.yy.cc main.cpp -o eco -lstdc++fs libabc.a -lm -ldl -lreadline -lpthread`
 * For MacOS users run `g++ -std=c++17 -Iparser-verilog/ -I. verilog_parser.tab.cc verilog_lexer.yy.cc main.cpp -o eco -lc++ libabc.a -lm -ldl -lreadline -lpthread`

```bash
~$ cp abc/{libabc.a,abc.rc} .
~$ g++ -std=c++17 -Iparser-verilog/ -I. verilog_parser.tab.cc verilog_lexer.yy.cc main.cpp -o eco -lstdc++fs libabc.a -lm -ldl -lreadline -lpthread
```

To run the eco program, give it `R1.v`, `R2.v`, `G1.v` with the logic network in verilog. For example (Please refer to [2021 ICCAD contest problem A] for further information.):

```bash
~$ ./eco benchmark/test1/r1.v benchmark/test1/r2.v benchmark/test1/g1.v
Module name = top
Module name = top
runeco temp/g1.v temp/r2.v temp/weight.txt

Considering target 0 (out of 1)...
Proved that the problem has a solution.  Time =     0.00 sec
Found one feasible set of 3 divisors.  Time =     0.00 sec
Starting cost = 3.
Minimized support to 3 supp vars.  Time =     0.00 sec
Finished enumerating 2 cubes.
The network is unchanged by fast extract.
Tar00: top      : i/o =      3/      1  and =       2  lev =    2 (2.00)  mem = 0.00 MB

The ECO solution was verified successfully.  Time =     0.02 sec
The network is unchanged by fast extract.
top      : i/o =      3/      1  and =       2  lev =    2 (2.00)  mem = 0.00 MB
Completed 50 iterations without improvement in 0.25 seconds.
Completed 50 iterations without improvement in 0.23 seconds.
Completed 50 iterations without improvement in 0.23 seconds.
Completed 50 iterations without improvement in 0.27 seconds.
top      : i/o =      3/      1  and =       2  lev =    2 (2.00)  mem = 0.00 MB

Patch   : in = 3  out = 1 : pi_in = 3  po_out = 1 : tfi = 0  tfo = 2
Added   : gate =   2 : c0 = 0  c1 = 0  buf =  0  inv =  0  two-input =   2

Patch has 3 inputs: 0=a(w=1) 1=b(w=1) 2=c(w=1)
Total weight = 3  Total runtime =     1.08 sec

Finished dumping resulting file "out.v".

Hierarchy reader flattened 1 instances of logic boxes and left 0 black boxes.
Networks are equivalent.  Time =     0.02 sec
```

The result file will be `out.v` and `patch.v` in your working directory.

# Support
This program is basically running on Linux system. There will be some issue to deal with if you are a MacOS or Windows system user.