#include <tinyECC.h>

tinyECC ecc;

void setup() {
// put your setup code here, to run once:
Serial.begin(9600);

    //Encryption
    ecc.plaintext="Hello World!";
    Serial.println("Plain Text: "+ecc.plaintext);

    //Signature Generation
    ecc.genSig();
    Serial.println("Signature:"+String(ecc.Sig[0])+","+String(ecc.Sig[1]));
    
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

void loop() {
  // put your main code here, to run repeatedly:

} 
