// **********************************************************************************
// Author:    Shubham Annigeri	github.com/ShubhamAnnigeri
// co-Author: Sunit Raut   	    github.com/SunitRaut
// **********************************************************************************
// License
// **********************************************************************************
// This program is free software; you can redistribute it 
// and/or modify it under the terms of the GNU General    
// Public License as published by the Free Software       
// Foundation; either version 3 of the License, or        
// (at your option) any later version.                    
//                                                        
// This program is distributed in the hope that it will   
// be useful, but WITHOUT ANY WARRANTY; without even the  
// implied warranty of MERCHANTABILITY or FITNESS FOR A   
// PARTICULAR PURPOSE. See the GNU General Public        
// License for more details.                              
//                                                        
// Licence can be viewed at                               
// http://www.gnu.org/licenses/gpl-3.0.txt
//
// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// **********************************************************************************
// This file: Generate Signature and verify signature for a plaintext string
// **********************************************************************************

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
