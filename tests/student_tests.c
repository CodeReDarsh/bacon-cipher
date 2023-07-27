#include "unit_tests.h"

TestSuite(student_suite, .timeout=TEST_TIMEOUT);

Test(base_output, decrypt_empty_string, .description="Plaintext string is length 0.") {
    char plaintext_act[] = "";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "i Can StoRe tHRee CHArACTERS! Yes!";
    decrypt(ciphertext, plaintext_act);  
    char *plaintext_exp = "";
    cr_expect_str_eq(plaintext_act, plaintext_exp, "plaintext was:           %s\nbut it should have been: %s", plaintext_act, plaintext_exp);
}
Test(base_return, decrypt_empty_string, .description="Plaintext string is length 0.") {
    char plaintext_act[] = "";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "i Can StoRe tHRee CHArACTERS! Yes!";
    int count_act = decrypt(ciphertext, plaintext_act);  
    int count_exp = -1;
    cr_expect_eq(count_act, count_exp, "Return value was %d, but it should have been %d.\n", count_act, count_exp);
}

Test(base_output, decrypt_short_ciphertext, .description="Ciphertext is shorter than 6 alphabets") {
    char plaintext_act[] = "bababababa";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "i Can ,,,,21398798572039578 S";
    decrypt(ciphertext, plaintext_act);  
    char *plaintext_exp = plaintext_act;
    cr_expect_str_eq(plaintext_act, plaintext_exp, "plaintext was:           %s\nbut it should have been: %s", plaintext_act, plaintext_exp);
}
Test(base_return, decrypt_short_ciphertext, .description="Ciphertext is shorter than 6 alphabets") {
    char plaintext_act[] = "bababababa";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "i Can ,,,,21398798572039578 S";
    int count_act = decrypt(ciphertext, plaintext_act);  
    int count_exp = -2;
    cr_expect_eq(count_act, count_exp, "Return value was %d, but it should have been %d.\n", count_act, count_exp);
}

Test(base_output, decrypt_plaintext_empty_and_short_ciphertext, .description="plaintext empty + Ciphertext is shorter than 6 alphabets") {
    char plaintext_act[] = "";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "i Can ,,,,21398798572039578 S";
    decrypt(ciphertext, plaintext_act);  
    char *plaintext_exp = "";
    cr_expect_str_eq(plaintext_act, plaintext_exp, "plaintext was:           %s\nbut it should have been: %s", plaintext_act, plaintext_exp);
}
Test(base_return, decrypt_plaintext_empty_and_short_ciphertext, .description="plain text empty + Ciphertext is shorter than 6 alphabets") {
    char plaintext_act[] = "";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "i Can ,,,,21398798572039578 S";
    int count_act = decrypt(ciphertext, plaintext_act);  
    int count_exp = -1;
    cr_expect_eq(count_act, count_exp, "Return value was %d, but it should have been %d.\n", count_act, count_exp);
}

Test(base_output, decrypt_cipher_invalid, .description="cipher invalid") {
    char plaintext_act[] = "aaaaaaaaaaa";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "ABCDe ,,,.,., F EOMCHA";
    decrypt(ciphertext, plaintext_act);  
    char *plaintext_exp = plaintext_act;
    cr_expect_str_eq(plaintext_act, plaintext_exp, "plaintext was:           %s\nbut it should have been: %s", plaintext_act, plaintext_exp);
}
Test(base_return, decrypt_cipher_invalid, .description="inavalid") {
    char plaintext_act[] = "aaaaaaaaaaa";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "ABCDe ,,,.,., F EOMCHA";
    int count_act = decrypt(ciphertext, plaintext_act);  
    int count_exp = -3;
    cr_expect_eq(count_act, count_exp, "Return value was %d, but it should have been %d.\n", count_act, count_exp);
}

Test(base_output, decrypt_multiple_eom, .description="cipher has multiple eom markers but can accomodate the shit") {
    char plaintext_act[] = "********************";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "One Of T He mAiN cAuseS EOMCHA of THE f all of The rOman EmpiRe Was THaT LaCking ZeRo, tHey haD no wAy To iNDicaTE SUCCESSful termination of their C programs.";
    decrypt(ciphertext, plaintext_act);  
    char *plaintext_exp = "--R";
    cr_expect_str_eq(plaintext_act, plaintext_exp, "plaintext was:           %s\nbut it should have been: %s", plaintext_act, plaintext_exp);
}
Test(base_return, decrypt_multiple_eom, .description="cipher has multiple eom markers but can accomodate the shit") {
    char plaintext_act[] = "********************";
    for (unsigned int i = 0; i < strlen(plaintext_act); i++)
        plaintext_act[i] = (char)(rand() % 200 + 33);
    char *ciphertext = "One Of T He mAiN cAuseS EOMCHA of THE f all of The rOman EmpiRe Was THaT LaCking ZeRo, tHey haD no wAy To iNDicaTE SUCCESSful termination of their C programs.";
    int count_act = decrypt(ciphertext, plaintext_act);  
    int count_exp = 3;
    cr_expect_eq(count_act, count_exp, "Return value was %d, but it should have been %d.\n", count_act, count_exp);
}