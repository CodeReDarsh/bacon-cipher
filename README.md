# Home-made Bacon Cipher 

This is a small program that implements my variant of [Bacon’s cipher](https://en.wikipedia.org/wiki/Bacon%27s_cipher). 

In this version of Bacon’s cipher, I use 6-bit codes (“Bacon codes”) to represent uppercase letters, digits and various symbols. Refer to a table of the [ASCII codes](https://www.asciitable.com/) while reviewing the assigned codes below. The Bacon codes are not randomly assigned.

Bacon codes 0 through 25 represent the 26 uppercase letters of the Latin alphabet:

| Letter | Bacon Code | Binary |
|--------|------------|--------|
| A      | aaaaaa     | 000000 |
| B      | aaaaab     | 000001 |
| C      | aaaaba     | 000010 |
| …      | …          | …      |
| Y      | abbaaa     | 011000 |
| Z      | abbaab     | 011001 |

Bacon codes 26 through 39 represent certain symbols:

| Symbol           | Bacon Code | Binary |
|------------------|------------|--------|
| space            | abbaba     | 011010 |
| !                | abbabb     | 011011 |
| "                | abbbaa     | 011100 |
| #                | abbbab     | 011101 |
| $                | abbbba     | 011110 |
| %                | abbbbb     | 011111 |
| &                | baaaaa     | 100000 |
| ' (single quote) | baaaab     | 100001 |
| (                | baaaba     | 100010 |
| )                | baaabb     | 100011 |
| ,                | baabaa     | 100100 |
| - (minus sign)   | baabab     | 100101 |
| .                | baabba     | 100110 |
| /                | baabbb     | 100111 |

Bacon codes 40 through 49 represent digits:

| Digit | Bacon Code | Binary |
|-------|------------|--------|
| 0     | babaaa     | 101000 |
| 1     | babaab     | 101001 |
| 2     | bababa     | 101010 |
| …     | …          | …      |
| 8     | bbaaaa     | 110000 |
| 9     | bbaaab     | 110001 |

Bacon codes 50 through 52 represent certain symbols:

| Symbol | Bacon Code | Binary |
|--------|------------|--------|
| :      | bbaaba     | 110010 |
| ;      | bbaabb     | 110011 |
| ?      | bbabaa     | 110100 |

Bacon codes 53 through 62 are unused.

Bacon code 63 (bbbbbb, 111111) represents EOM, the “end-of-message” marker.

## Encryption

- done by the encrypt function
- Arguments:
    - plaintext: a null-terminated string to be encrypted. The string contains only ASCII characters listed in the Overview section above with Bacon codes 0 through 52. Any letters present in the plaintext should be treated as uppercase letters and encoded as such.
        
    - ciphertext: a null-terminated string used to encrypt the plaintext according to our modified Bacon’s cipher. A mutable character array is passed as ciphertext so that its contents may be changed.

- Returns: the number of characters from plaintext successfully encoded, or -1 if there was insufficient memory to encode the EOM marker (e.g., use strlen to help you)

- Characters in ciphertext may be modified only to encode characters or the EOM marker.

    ```
    An example:

    plaintext: "Stony Brook University"
    
    ciphertext before function call: "I can store three characters! Yes!"
    
    ciphertext after function call: "i Can StoRe tHRee CHArACTERS! Yes!"
    
    Return value: 3

    Explanation: the letters S, T and O have the Bacon codes 010010, 010011 and 001110, respectively. Note how the cases of the letters in the ciphertext have been set below. The EOM marker is shown in purple.

    0 100 10010 01100 1110111111
    
    i Can StoRe tHRee CHArACTERS! Yes
- See the file tests/unit_tests.c in the provided template repository for additional examples.

## Decryption

- done by the decrypt function
- Arguments:
    - ciphertext: a null-terminated string that encodes a message using Bacon’s cipher.
    - plaintext: a null-terminated string of (possibly) random characters. A mutable character array is passed as plaintext so that its contents may be changed.

- The function null-terminates the plaintext message. If the ciphertext contains more than one EOM marker, the message is decrypted up to the first EOM marker. If the plaintext string cannot store all of the decrypted ciphertext, the first strlen(plaintext) characters of the decrypted ciphertext are stored in plaintext. 

- Returns: the number of encrypted characters successfully decrypted, or one of the following error codes:

    - -1 if the length of the plaintext string is 0 (e.g., as computed by strlen). This means we cannot decrypt any portion of the ciphertext.
    - -2 if the ciphertext is missing the EOM marker. This means the ciphertext is invalid.
    - -3 if the ciphertext contains one or more invalid Bacon codes BEFORE THE EOM MARKER. The string has an EOM marker, in this case. This means the ciphertext is invalid.


- When any error is present in the arguments (meaning that the function is going to return an error code), the function does not change the contents of plaintext.
It also checks for the error cases in the order provided. i.e., if two or more errors are present, then it returns the maximum (i.e., least negative) valid error code.

- Characters in plaintext may be modified only to decrypt the ciphertext.

- An example:

    - ciphertext: "One Of THe mAiN cAuseS of THE fall of The rOman EmpiRe Was THaT LaCking ZeRo, tHey haD no wAy To iNDicaTE SUCCESSful termination of their C programs."
    
    - plaintext before function call: 	"*******************"
    
    - plaintext after function call: 	"--ROBERT FIRTH\0****"
    - Return value: 14

- If the plaintext were printed, the output would simply be: --ROBERT FIRTH

- Explanation: consider the substring containing the first 24 (6 x 4) letters of the string:

    ```One Of THe mAiN cAuseS of THE f

    Treat uppercase letters as 1s and lowercase as 0s:

    One Of THe mAiN cAuseS of THE f
    100 10 110 0101 010001 00 111 0

    Regroup into 6-bit bitstrings:

    100101 100101 010001 001110

    In decimal, these are the values 37, 37, 17, 14. These correspond to the Bacon codes for the characters –-RO, the first four characters of the decrypted text.

- See the file tests/unit_tests.c in the provided repository for additional examples. You can also use the output of text cases for from the encrypt function, including your own test cases, as the input for test cases of decrypt function.

# Notes

Make sure you have [Valgrind](https://valgrind.org/) and [Criterion](https://criterion.readthedocs.io/en/master/setup.html) with it's required dependencies installed in order to run the unit tests.

- To install valgrind enter the following command:

    `sudo apt-get install -y valgrind`

- To install Criterion and it's dependencies enter the following command:
    
    `sudo apt-get install libcriterion-dev && sudo apt-get install -y ninja-build meson cmake pkg-config libffi-dev libgit2-dev`

- To run the unit tests enter the following:

    `make && make test`

- Don't try to clean the build using `make clean` as it causes the tests folder to be deleted for some reason lol.
