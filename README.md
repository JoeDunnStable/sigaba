# Sigaba a Simulator of the ECM Mark II Cipher Machine

## Introduction

The ECM Mark II cipher machine was the most secure cipher machine used by the US
during World War II and into the 1950's.  There is no known case of the cipher being
broken during its years of operation, and it was only replaced because it was too slow
to handle modern communication needs.  The machine was used by both the US Army
and the Navy.  The Army referred to the machine as SIGABA, which is the name I've used
in the code.  The machine's details were not declassified until the 1990's and as a result
its existence is not as well known as its Axis counterparts, the German Enigma machine
and the Japanese Purple machine.

A much more detailed description of the ECM Mark II by Rich Pekelney can be fournd 
at https://maritime.org/tech/ecm2.htm.

## Simulation of the ECM Mark II

As part of Rich Pekelney's examination of the cipher machine that was temporarily on 
loan to the San Francisco Maritime Natonal Park he developed a Java applet to simulate 
the behavior of the machine (https://maritime.org/tech/ecmapp.htm).  His simulation 
includes the following preface;

> This program emulates an ECM Mark II (aka SIGABA).  It emulates the CSP-2900 version
> that is now on USS Pampanito SS-383 in San Francisco.  The emulation is based on the
> artifact (the machine) not engineering drawings or other documentation.  This leaves
> the possibility that some of the behavior is a bug in this particular machine rather
> than the generic design.  All the documents I have are consistent with the machine's
> functioning, but the documents are not a complete machine definition.

Unfortunately, modern browsers have dropped support for applets, so the program no
longer functions as originally intended.  The present package is a port of the 
cryptological engine in the Java code to C++, with a command line interface to input
the keying information.

## Program Options

The keying information is input using the program options.  As usual a description of
the options can be obtained by entering -h or --help.

The machine contains 15 rotors, which are used to permute the inputs to outputs.
There are 10 large rotors with 26 contacts on each side that can be placed in
either the five rotor cipher bank or the five rotor control bank.  In either case the rotors
can be placed in either normally (N) or reversed (R).  Thus the argument --cipherOrder might
appear as 7N1R4N2R1N indicating that the first rotor position contains rotor 7 in the
normal orientation, the second rotor contains rotor 1 in the reversed orientation, etc.
There are also 5 small rotors with 10 contacts on each side, which are placed in the 
index bank and are described similarly.

The machine can operate in three modes (input through the --machine option), CSP889,
CSP2900 or CSPNONE.

The start position of each rotor is also input by --cipherPos, --controlPos, or --indexPos.
For the cipher and control rotors the position is given by five letters A through Z, and
for the index rotors the position is given by five numbers 0-9.

The -e option is used to encrypt and the -d option is used to decrypt.

Finally the --text option is used for the input text.

For instance the following invocation uses the default options for everything  except
the cipher position:

    sigaba -e --cipherPos "ABCDE" --text "HELLO WORLD"
    
The output should be:
  
    PWTKZ PQXMA C

The program also automates the initialization procedure used by the US Navy.
On the actual machine this invovled rotating each control rotor one step at a time until
the rotor was at the target position, while allowing the cipher rotors to step as usual.
The following code shows how to generate that result

    sigaba -e --navyInit --controlPos ABCDE --text "HELLO WORLD"
    
The result should be

    COTUC RAXLV I
    
## Internal Method
The internal method of rotor wiring generates rotors for which the permutations generated
by each position of the rotor rotation are as different as possible from each other.  The
machine supplied to the San Francisco National Maritime Park did not include control
and cipher rotors with wiring that might be used in encryption.  Rich Pekelney developed
his own wiring table for use in his simulator, but he did not use the internal method.
It's not known whether the cipher and control 
rotors in use by the US were wired using the internal method, but the index rotors supplied
to Pekelney were so wired so it's likely that the cipher and control rotors were wired 
using the internal method also.  This package contains a program to check whether
a given wiring scheme satisfies the internal method and a second program that generates
random wiring schemes that do satisfy the method.

## Compilation

The components of the basic program are contained in the sigaba subdirectory.  The only
prerequisites are the Boost headers and libraries.  The test_sigaba subdirectory contains code
that compares the output of the code in the sigaba directory to the output of the Java
code.  Finally the internal method subdirectory contains a class InternalMethod to
generate rotor wiring using the internal method and a class Checker to check whether a
given wiring is a proper permutation and the amount by which it differs from the 
internal method.

To allow for convenient ports to different platforms the directories contain meson.build
files that can be used with the meson system (available at https://mesonbuild.com).

## Acknowlegments

A big thank you to Rich Pekelney who reverse engineered the machine and developed 
the original Java simulator.  Rich has graciously agreed to the use of the MIT license
for this distribution.


