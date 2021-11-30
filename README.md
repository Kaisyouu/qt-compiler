# compiler

#### 0. Intro

This is written for ECNU 2021 Autumn's course "Practice on Compilation Principles". Based on QT5, it's a simple X0 language(self-defined) compiler.

#### 1. logs

##### 1.1 QProcess

To start a single-line UNIX shell script in QT5, simply use QProcess to start a process.

```c++
QProcess bash_proc;
bash_proc.start("../src/com mytest.x0");
bash_proc.waitForStarted();
bash_proc.waitForFinished();
QString output = bash_proc.readAllStandardOutput();
QString error = bash_proc.readAllStandardError();
```

* Always remember to collect output and error messages
* Your shell is working on `build-ProjectName-QtVersion-64bit-Debug`
* Don't try to use `cd` to change working directory, it doesn't work
* DO NOT support multi-line scripts

##### 
