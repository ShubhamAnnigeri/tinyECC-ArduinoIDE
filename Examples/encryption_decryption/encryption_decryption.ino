#include <tinyECC.h>

tinyECC ecc;

void setup() {
// put your setup code here, to run once:
Serial.begin(9600);

    //Encryption
    ecc.plaintext="Hello World!";
    Serial.println("Plain Text: "+ecc.plaintext);
    ecc.encrypt();
    Serial.println("Cipher Text: "+ecc.ciphertext);

    //Decryption
    ecc.plaintext = F("");
    //decrypts ecc.ciphertext
    ecc.decrypt();
    ecc.ciphertext = F("");
    Serial.println("Decoded: "+ecc.plaintext);
    

}

void loop() {
  // put your main code here, to run repeatedly:

} 
