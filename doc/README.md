# Sigaba a Simulator of the ECM Mark II Cipher Machine

## Introduction

The ECM Mark II cipher machine was the most secure cipher machine used by the US
during World War II and into the 1950's.  There is no known case of the cipher being
broken during its years of operation, and it was only replaced because it was too slow
to handle modern communication needs.  The machine was used by both the US Army
and the Navy.  The army referred to the machine as Sigaba, which is the name I've used
in the code.  The machine's details were not declassified until the 1990's and as a result
its existence is not as well known as its Axis counterparts, the German Enigma machine
and the Japanese Purple machine.

A much more detailed description of the ECM Mark II by Rich Pekelney can be fournd 
at https://maritime.org/tech/ecm2.htm.

## Simulation of the ECM Mark II

As part of his examination of the machine that was temporatily on loan to the San Francisco
Maritime Natonal Park Rich Pekelney developed a Java applet to simulate the behavior of 
the machine (https://maritime.org/tech/ecmapp.htm).  His simulation includes the following
preface;

"This program emulates an ECM Mark II (a"ka SIGABA).  It emulates the CSP-2900 version
that is now on USS Pampanito SS-383 in San Francisco.  The emulation is based on the
artifact (the machine) not engineering drawings or other documentation.  This leaves
the possibility that some of the behavior is a bug in this particular machine rather
than the generic design.  All the documents I have are consistent with the machine's
functioning, but the documents are not a complete machine definition."

Unfortunately, modern browsers have dropped support for applets, so the program no
longer functions as originally intended.  The present package is a port of the 
cryptological engine in the Java code to C++, with a command line interface to input
the keying information.

## Program Options

Keying information was normally distributed on paper sheets which contain the setup
for each day's encryption, which might vary by classifciation level.  In this simulation
the keying information is input using the program options.  As usual a description of
the options can be obtained by entering -h or -help.

The machine contains 15 rotors that are used to permute the inputs to outputs.
There are 10 large rotors with 26 contacts on each side that can be placed in
either the cipher bank or the control bank.  In either case the rotors can be placed
in either normally (N) or reversed (R).  Thus the argument -cipherOrder might
appear as 7N1R4N2R1N indicating that the first rotor position contains rotor 7 in the
normal orientation, the second rotor contains rotor 1 in the reversed orientation, etc.
There are also 5 small rotors with 10 contacts on each side which are placed in the 
index bank and are describe dsimilarly.

The machine can operate in three modes (input through the -machine option), CSP889,
CSP2900 or CSPNONE.

The start position of each rotor is also input by -cipherPos, -controlPos, or -indexPos.
For the cipher and control rotors the position is given by five letter A through Z, and
for the index rotors the positon is given by five numbers 0-10.

The -e option is used to encrypt and the -d option is used to decrypt.

Finally the -input option is used for the input text.

The present version does not attempt to replicate one version of initialization procedure 
that requred the use of the 1 to 5 keys on the machine.

## Compilation

The componets of the basic program are oontained in the sigaba subdirectory.  The only
prequisite is the Boost headers and library's.  There's also a test_sigabe directory, which 
compares the output of the code in the sigaba directory to the output of the Java code.

## Acknowlegments

A big thank you to Rich Pekelney who reverse engineered the machine and developed 
the origianl Java simulator.  Rich has graciously agreed to the use of the MIT license
for this distribution.


