#include <tinyECC.h>

tinyECC ecc;

void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
Serial.println(F("Type above in Serial Input to start!"));

}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available())
  {
    //Encryption
    ecc.plaintext=Serial.readString();
    Serial.println("Plain Text: "+ecc.plaintext);
    ecc.encrypt();
    Serial.println("Cipher Text: "+ecc.ciphertext);
    //Signature Generation
    ecc.genSig();
    Serial.println("Signature:"+String(ecc.Sig[0])+","+String(ecc.Sig[1]));
    
    //Decryption
    ecc.plaintext = F("");
    //decrypts ecc.ciphertext
    ecc.decrypt();
    ecc.ciphertext = F("");
    Serial.println("Decoded: "+ecc.plaintext);
    
    //Signature Verification
    if(ecc.verifySig())
     {
      Serial.println(F("Signature Verified"));  
     }
    else
     {
      Serial.println(F("Signature Rejected"));   
     }

  }

} 