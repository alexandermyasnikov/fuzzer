
# Fuzzer

Author: Alexander Myasnikov

mailto:myasnikov.alexander.s@gmail.com

git:https://gitlab.com/amyasnikov/fuzzer

Version: 1.0.0



### What is fuzzer?

Fuzzer is a utils which mixes input and random data.
It is help to find bugs in your programs.



### Building

```
make
```



### Usage

Read from stdin. Write to stdout:

```
cat my_dump | ./fuzzer
```

Read from file my_dump_in. Write to file my_dump_out. Seed is 42. Factor is 1000:

```
./fuzzer -r my_dump_in -w my_dump_out -s 42 -f 1000
```



