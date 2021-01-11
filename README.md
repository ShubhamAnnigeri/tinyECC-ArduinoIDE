# tinyECC - Elliptic Curve Cryptography for Arduino Micro-controllers.
[![GitHub release](https://img.shields.io/github/release/ShubhamAnnigeri/tinyECC-ArduinoIDE.svg)](https://github.com/ShubhamAnnigeri/tinyECC-ArduinoIDE)
[![license](https://img.shields.io/github/license/ShubhamAnnigeri/tinyECC-ArduinoIDE.svg)](https://github.com/ShubhamAnnigeri/tinyECC-ArduinoIDE/blob/master/license.txt)

This is a library for implementing Elliptic Curve Cryptography (ECC) and Elliptic Curve Digital Signature Algorithm (ECDSA) on Arduino-based microcontrollers

## Features
- Elliptic Curve Cryptography for Arduino-based microcontrollers.
- Elliptic Curve Digital Signature Algorithm.
- Three different mapping tables to choose from as per your application.
- Mapping table is stored in program memory to save RAM space
- Easy-to-use

# How to use Library

Create an object of class tinyECC: 
```
tinyECC e;    // object e of class tinyECC
```
### Encryption
```
e.plaintext = "Hello World!"; // Assign to plaintext

e.encrypt();                   // Encrypt plaintext and store result in ciphertext

Serial.println(e.ciphertext); // Display ciphertext
```
### Decryption
```
e.ciphertext = "........." ;   // Assign encrypted text to ciphertext

e.decrypt();                   // Decrypt ciphertext and store result in plaintext

Serial.println(e.plaintext); // Display plaintext
```
### Signature Generation
```
e.plaintext = "Hello World!"; // Assign to plaintext

e.genSig();                   // Generate Signature

Serial.println(String(e.Sig[0])+","+String(e.Sig[1])); // Display generated Signature (Signature is a 2-element array)
```
### Signature Verificcation
```
e.plaintext = "Hello World!"; // Assign to plaintext

e.Sig[0] = 67, e.Sig[1] = 27;   // Assign Signature

e.verifySig();                 // Returns true if signature verified, else returns false.    
```


# Mapping Tables

There are three mapping table options:
- 1. 256-element mapping table for ASCII-256 characters. This covers all keyboard characters plus special greek and math symbols.
- 2. 128-element mapping table for ASCII-127 characters. This covers all the keyboard and commonly-used characters
- 3. 10-element mapping table for digits from 0 to 9. Use this if you are dealing purely with numbers in your plaintext.

How to set mapping table:

To set mapping table, edit the tinyECC_mapping_table.h file. Uncomment the appropriate line to define your mapping table in the tinyECC_mapping_table.h file as shown below:
```
//Mapping Table for 0-255 ASCII characters. Includes support for special characters. 

#define MAPPING_TABLE_TYPE  MAPPING_TABLE_256

#define MAPPING_TABLE_SIZE  MAPPING_TABLE_256_SIZE

//Mapping Table for 0-127 ASCII characters. Most of the commonly used keyboard characters are covered in this range.

//#define MAPPING_TABLE_TYPE  MAPPING_TABLE_128

//#define MAPPING_TABLE_SIZE  MAPPING_TABLE_128_SIZE

//Mapping Table for '0'-'9' characters. Use when string contains only numbers.

//#define MAPPING_TABLE_TYPE  MAPPING_TABLE_10

//#define MAPPING_TABLE_SIZE  MAPPING_TABLE_10_SIZE
```
### Important Note:
- Encryption and decryption, being intensive processes, will take significant time when executed on Arduino-based microcontrollers. 
- Encryption and decryption times will increase with increase in the lengt of plaintext and ciphertext.  
- Mapping tables occupy space in program memory. The mapping table will occupy 512, 256, or 20 bytes in your program memory based on 256, 128, 10-element mapping tables respectively. 
- Keep a track of plaintext and ciphertext lengths as these can consume significant space in your RAM. Keep plaintext length within limits. 
