#include "bacon.h"

// Add other #includes here if you want.
#define BACON_CODE_SIZE 6

int char_counter(char *c_txt);
void eom_encoder(int start_idx, int end_idx, char *ciphertext);
int encoder(int n_chars, const char *plaintext, char *ciphertext);
int decoder(int bacon_char);

/**
 * @brief  encrypts a message using 6-bit Bacon codes.
 * 
 * @param plaintext a null-terminated string to be encrypted. The string contains only ASCII characters 
 *                  listed with Bacon codes 0 through 52. Any letters present in the plaintext should be 
 *                  treated as uppercase letters and encoded as such.
 * @param ciphertext a null-terminated string used to encrypt the plaintext according to our modified Bacon’s cipher. 
 *                   A mutable character array is passed as ciphertext so that its contents may be changed.
 *
 * @return int the number of characters from plaintext successfully encoded, or
 *         -1 if there was insufficient memory to encode the EOM marker
 */
int encrypt(const char *plaintext, char *ciphertext)
{
    int n_chars; // stores the number of charachters that should be encreypted

    if (strlen(ciphertext) < 6)
        return -1;
    else if (strlen(plaintext) == 0)
        n_chars = 1;
    else if (char_counter(ciphertext) <= (int) strlen(plaintext))
        n_chars = char_counter(ciphertext);
    else
        n_chars = (int) strlen(plaintext) + 1;

    int result = encoder(n_chars, plaintext, ciphertext);
    return result;
}

/**
 * @brief decrypts the ciphertext that was generated using the 6-bit
 *        Bacon codes provided above and stores the decrypted message in plaintext.
 * 
 * @param ciphertext a null-terminated string that encodes a message using Bacon’s cipher.
 * @param plaintext a null-terminated string of (possibly) random characters. 
 *                  A mutable character array is passed as plaintext so that its contents may be changed.
 * @return int the number of encrypted characters successfully decrypted, or one of the following error codes:
 *              
 *              > -1 if the length of the plaintext string is 0 (e.g., as computed by strlen). This means we cannot decrypt any portion of the ciphertext.
 *              > -2 if the ciphertext is missing the EOM marker. This means the ciphertext is invalid.
 *              > -3 if the ciphertext contains one or more invalid Bacon codes BEFORE THE EOM MARKER. The string has an EOM marker, in this case. 
 *                   This means the ciphertext is invalid.
 *              
 * When any error is present in the arguments (meaning that the function is going to return an error code), 
 * the function must not change the contents of plaintext. Checks for the error cases in the order
 * provided. i.e., if two or more errors are present, returns the maximum (i.e., least negative) valid error code.

 */
int decrypt(const char *ciphertext, char *plaintext)
{
    int encoded_char_count = char_counter(ciphertext); // stores the number of characters encrypted in cipher using bacon code

    ////////////////////////////////////////////////ERROR CHECKING BEGINS////////////////////////////////////
    int pltxt_len = (int) strlen(plaintext);
    if (pltxt_len == 0){
        return -1;
    }
    else if (encoded_char_count < 1){
        return -2; // because if it's less than 6 chars in length then it doesn't have the eom marker
    } 
    else if (encoded_char_count == 1){
        int bacon_char = 0, current_idx = 0, bacon_bits = BACON_CODE_SIZE;

        while (bacon_bits > 0 && current_idx < (int) strlen(ciphertext)){
            char ch = ciphertext[current_idx];
            if (ch >= 'A' && ch <= 'Z'){
                bacon_char |= 1;
                bacon_char <<= 1;
                bacon_bits--;
            } 
            else if (ch >= 'a' && ch <= 'z'){
                bacon_char <<= 1;
                bacon_bits--;
            }
            current_idx++;
        }
        
        bacon_char >>=1;
        int decoded_char = decoder(bacon_char);

        if (decoded_char != '\0')
            return -2;
    }
    else if (encoded_char_count > 1)
    {
        int bacon_char;
        int current_idx = 0, temp = encoded_char_count;
        while (temp > 0){
            bacon_char = 0;
            int bacon_bits = BACON_CODE_SIZE;
            while (bacon_bits > 0 && current_idx < (int) strlen(ciphertext)){
                char ch = ciphertext[current_idx];
                if (ch >= 'A' && ch <= 'Z'){
                    bacon_char |= 1;
                    bacon_char <<= 1;
                    bacon_bits--;
                } 
                else if (ch >= 'a' && ch <= 'z'){
                    bacon_char <<= 1;
                    bacon_bits--;
                }
                current_idx++;
            }
            bacon_char >>= 1;
            int temp2 = decoder(bacon_char);
            if (temp2 == '\0'){
                bacon_char = temp2;
                break;
            }
            temp--;
        }

        if (bacon_char == '\0'){
            current_idx = 0;
            temp = encoded_char_count;

            while (temp > 0){
                bacon_char = 0;
                int bacon_bits = BACON_CODE_SIZE;
                
                while (bacon_bits > 0 && current_idx < (int) strlen(ciphertext)){
                    char ch = ciphertext[current_idx];
                    if (ch >= 'A' && ch <= 'Z'){
                        bacon_char |= 1;
                        bacon_char <<= 1;
                        bacon_bits--;
                    } 
                    else if (ch >= 'a' && ch <= 'z'){
                        bacon_char <<= 1;
                        bacon_bits--;
                    }
                    current_idx++;
                }
                bacon_char >>= 1;
                if (decoder(bacon_char) == -3){
                    return -3;
                }
                temp--; 
            }
        } else {
            return -2;
        }
    }
    //////////////////////////////ERROR CHECKING DONE/////////////////////////

    ////////////////////////////ACTUAL DECRYTION BEGINS///////////////////////
    
    if (pltxt_len < encoded_char_count){
        encoded_char_count = pltxt_len;
    }
    
    int current_index = 0, temp_len = encoded_char_count;   // I hesitated from using the var name bacon_char again
    while (temp_len > 0){                                   // becuase I thought it would create problems, otherwise they're
        int bacon_ch = 0;                                   // essentially the same
        int bacon_bits = BACON_CODE_SIZE;
        while (bacon_bits > 0 && current_index < (int) strlen(ciphertext)){
            char ch = ciphertext[current_index];
            if (ch >= 'A' && ch <= 'Z'){
                bacon_ch |= 1;
                bacon_ch <<= 1;
                bacon_bits--;
            } 
            else if (ch >= 'a' && ch <= 'z'){
                bacon_ch <<= 1;
                bacon_bits--;
            }
            current_index++;
        }
        bacon_ch >>= 1;
        int decrypted_ch = decoder(bacon_ch);
        plaintext[(encoded_char_count - temp_len)] = decrypted_ch;
        temp_len--;
    }

    return strlen(plaintext);
}

/**
 * @brief Accepts a bacon code character and converts it to it's ASCII counter part
 * 
 * @param bacon_char an int containg the bacon code binary
 * @return int an int containing the decoded bacon code in ASCII value
 */
int decoder(int bacon_char){
    // if else tree to decrypt bacon character into ascii value
    if (bacon_char >= 0 && bacon_char <= 25)        // for 'A' - 'Z'
        bacon_char = bacon_char + 'A';
    else if (bacon_char >= 26 && bacon_char <= 35)  // for ' ' to ')'
        bacon_char = bacon_char - 26 + ' ';
    else if (bacon_char >= 36 && bacon_char <= 39)  // for ',' to '/'
        bacon_char = bacon_char - 36 + ',';
    else if (bacon_char >= 40 && bacon_char <= 49)  // for '0' to '9'
        bacon_char = bacon_char - 40 + '0';
    else if (bacon_char >= 50 && bacon_char <= 51)  // for ':' to ';'
        bacon_char = bacon_char - 50 + ':';
    else if (bacon_char == 52)                      // for '?'
        bacon_char = '?';
    else if (bacon_char == 63)                      // for 0 to 9
        bacon_char = '\0';
    else                            // if the char turns out to be between 53 to 62
        bacon_char = -3;
    
    return bacon_char;
}

/**
 * @brief helper function to encrypt given ciphertext 
 * 
 * @param n_chars stores the number of characters that should be encrypted
 * @param plaintext constant char array of given ciphertext
 * @param ciphertext unmodified char array of ciphertext to encrypt
 * @return int no of characters encrypted
 */
int encoder(int n_chars, const char *plaintext, char *ciphertext){
    int ch_encoded = 0;
    if (n_chars == 1){
        eom_encoder(0, strlen(ciphertext), ciphertext);
        return 0;
    }
    else if (n_chars > 1){
        int x, ch_bit_count, current_index = 0;
        // this for loop stores the bacon code of each plaintext character one by one into x, runs till first nchar - 1 characters from plain text
        for (int i = 0; i < (n_chars - 1); i++){
            ch_bit_count = 6; // because character in bacon code is 6 bit long (but actually in memory it's stored in a 32-bit integer, so what I mean to say is that relevant part is last 6 bits)
            if (strlen(plaintext) == 0){
                x = 63;
                n_chars = 1;
            }
    
            char ch = plaintext[i];
            if (ch >= 'A' && ch <= 'Z'){
                x = ch - 'A';
                ch_encoded++;
            }
            else if (ch >= 'a' && ch <= 'z'){
                x = ch - 'a';
                ch_encoded++;
            }
            else if (ch >= ' ' && ch <= ')'){
                x = ch - 6; 
                ch_encoded++;
            }
            else if ((ch >= ',' && ch <= '/') || (ch >= '0' && ch <= '9') || ch == ':' || ch == ';'){
                x = ch - 8;
                ch_encoded++;
            }
            else if (ch == '?'){
                x = ch - 11;
                ch_encoded++;
            }

            int mask;
            while (ch_bit_count > 0){
                mask = x & (32 >> (BACON_CODE_SIZE - ch_bit_count)); // 32 because in binary it's last 6 digits are 100000
                if (mask){
                    if ((ciphertext[current_index] >= 'a' && ciphertext[current_index] <= 'z')){
                        char letter = ciphertext[current_index] - 'a' + 'A';
                        ciphertext[current_index] = letter;
                        ch_bit_count--;
                    } 
                    else if ((ciphertext[current_index] >= 'A' && ciphertext[current_index] <= 'Z'))
                        ch_bit_count--;
                    
                } else {
                    if ((ciphertext[current_index] >= 'A' && ciphertext[current_index] <= 'Z')){
                        char letter = ciphertext[current_index] - 'A' + 'a';
                        ciphertext[current_index] = letter;
                        ch_bit_count--;
                    } 
                    else if ((ciphertext[current_index] >= 'a' && ciphertext[current_index] <= 'z')){
                        ch_bit_count--;
                    }
                }

                current_index++;
            }
        }

        eom_encoder(current_index, (int) strlen(ciphertext), ciphertext);
    }
    return ch_encoded;
}

/**
 * @brief encodes the last possible encodable part of the ciphertext with the EOM marker
 *
 * @param start_idx a size_t type value that represents the index of ciphertext from where the EOM encoding starts
 * @param end_idx a size_t type value that represents the index of ciphertext till where the EOM is encoded
 * @param ciphertext a char array that represents the ciphertext to be encoded with the EOM marker
 */
void eom_encoder(int start_idx, int end_idx, char *ciphertext){
    int counter = BACON_CODE_SIZE;
    
    for (int i = start_idx; i < end_idx; i++){
        char ch = ciphertext[i];
        if (counter == 0)
            break;
        if (ch >= 'A' && ch <= 'Z')
            counter--;
        else if (ch >= 'a' && ch <= 'z'){
            ciphertext[i] = ciphertext[i] - 'a' + 'A';
            counter--;
        }
    }
    return;
}

/**
 * @brief calculates the number of Bacon cipher characters the given ciphertext can hold/hide
 *
 * @param c_txt String of given ciphertext
 * @return size_t no of characters that can be hidden in cipher text
 */
int char_counter(char *c_txt){
    int count = 0;
    for (size_t i = 0; i < strlen(c_txt); i++){
        if ((c_txt[i] >= 'A' && c_txt[i] <= 'Z') || (c_txt[i] >= 'a' && c_txt[i] <= 'z'))
            count++;
    }
    count /= 6;
    return count;
}