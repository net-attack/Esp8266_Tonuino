#include <SPI.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <MFRC522.h>
#define SS_PIN          15         // Pin: D8
#define RST_PIN         0          // Pin: D3

long chipID;


MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial mySoftwareSerial(4, 2); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  mySoftwareSerial.begin(9600);

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.println("Boot RFID-Reader...");
  // SPI-Bus initialisieren
  SPI.begin();

  // MFRC522 initialisieren
  mfrc522.PCD_Init();

  //Kurze Pause nach dem Initialisieren   
  delay(10);
  Serial.println("");

  // Details vom MFRC522 RFID READER / WRITER ausgeben
  mfrc522.PCD_DumpVersionToSerial();  


Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).
 // myDFPlayer.volumeUp(); //Volume Up
 // myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  
  //----Mp3 control----
//  myDFPlayer.sleep();     //sleep
//  myDFPlayer.reset();     //Reset the module
//  myDFPlayer.enableDAC();  //Enable On-chip DAC
//  myDFPlayer.disableDAC();  //Disable On-chip DAC
//  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15



  
  Serial.println("RFID-Chip auflegen, um UID anzuzeigen...");



  
}

void loop() {

  if(Serial.available()){
    readCommand();
  }

   if (myDFPlayer.available()) {
      printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
    }
  // put your main code here, to run repeatedly:
  // Sobald ein Chip aufgelegt wird startet diese Abfrage
  if (mfrc522.PICC_IsNewCardPresent()){
  
    //CardID resetten
    chipID = 0;
    
    mfrc522.PICC_ReadCardSerial();

    // Hier wird die ID des Chips in die Variable chipID geladen
    for (byte i = 0; i < mfrc522.uid.size; i++){
      chipID=((chipID+mfrc522.uid.uidByte[i])*10);
    }

    //... und anschließend ausgegeben
    Serial.println(chipID);
    switch(chipID){
      case 564780:
        myDFPlayer.play(1);
        break;
      case 2096050:
        myDFPlayer.play(2);
        break;
      case 287250:
        myDFPlayer.play(3);
        break;
      case 291250:
        myDFPlayer.play(4);
        break;
      case 2511950:
        myDFPlayer.play(5);
        break;
      case 2504950:
        myDFPlayer.play(6);
        break;
      case 419450:
        myDFPlayer.play(7);
        break;
      case 268280:
        myDFPlayer.play(8);
        break;
      case 1166680:
        myDFPlayer.play(9);
        break;
      case 961580:
        myDFPlayer.play(10);
        break;
      case 815250:
        myDFPlayer.play(11);
        break;
      case 2036670:
        myDFPlayer.play(12);
        break;
      case 2349250:
        myDFPlayer.play(13);
        break;
      case 1707280:
        myDFPlayer.play(14);
        break;
      case 467950:
        myDFPlayer.play(15);
        break;
      case 1266950:
        myDFPlayer.play(16);
        break;
      case 417450:
        myDFPlayer.play(17);
        break;
        
      default:
        break;
    }



    // Danach 5 Sekunden pausieren, um mehrfaches lesen /ausführen zu verhindern
    delay(5000);
      
  }

}

void readCommand(){
  char cmd = ' ';
  int value1, value2 = 0;
  cmd = Serial.read();
  value1 = Serial.parseInt();
  value2 = Serial.parseInt();
 
  switch(cmd){
    case 'h': 
      printHelp();
      break;
      
    case '>':
      myDFPlayer.next();
      break;
      
    case '<':
      myDFPlayer.previous();
      break;
      
    case 'p':
      if(value2 == 0) myDFPlayer.play(value1);
      else if(value1 != 0) myDFPlayer.playFolder(value1, value2);
      break;
    case 'P':
      if(value2 == 0) myDFPlayer.playMp3Folder(value1); 
      else if(value1 != 0)myDFPlayer.playLargeFolder(value1, value2);
      break;
    
    case '+':
      myDFPlayer.volumeUp(); 
      break;
      
    case '-':
      myDFPlayer.volumeDown();
      break;
      
    case 'v':
      myDFPlayer.volume(value1);
      break;
      
    case 'b':
      myDFPlayer.pause();
      break;
    case 's':
      myDFPlayer.start();
      break;
          
    case 'z':
      myDFPlayer.sleep();
      break;
      
    case 'L':
      if(value1 == 0) myDFPlayer.enableLoopAll();
      else myDFPlayer.loopFolder(value1);
      break;
    
    case 'l':
      if(value1 == 0) myDFPlayer.disableLoopAll();
      else myDFPlayer.loop(value1);
      break;
    
    case 'A':
      myDFPlayer.advertise(value1);
      break;
    
    case 'a':
      myDFPlayer.stopAdvertise();
      break;
    
    case 'q':
      if(value1 == 1) Serial.println(myDFPlayer.readState()); 
      else if(value1 == 2) Serial.println(myDFPlayer.readVolume());
      else if(value1 == 3) Serial.println(myDFPlayer.readEQ());
      else if(value1 == 4) Serial.println(myDFPlayer.readFileCounts());
      else if(value1 == 5) Serial.println(myDFPlayer.readFolderCounts());
      else if(value1 == 6) Serial.println(myDFPlayer.readCurrentFileNumber());
      break;
    default:
      Serial.println("Ungültiges Kommando");
      break;
  }
}
void printHelp(){
  Serial.println("DFPlayer Commands:");
  Serial.println(" h - help");
  Serial.println(" > - next ");
  Serial.println(" < - previous");
  Serial.println(" p3 - play");
  Serial.println(" p3,5 - play folder 3, file 5");
  Serial.println(" P3,5 - play large folder 3, file 5");
  Serial.println(" P3 - play file 3 in MP3 folder"); 
  Serial.println(" + - volume up");
  Serial.println(" - - volume down");
  Serial.println(" v10 - set volume to 10");
  Serial.println(" b - Pause");
  Serial.println(" s - start ");
  Serial.println(" z - sleep ");
  Serial.println(" L - enable loop all");
  Serial.println(" l - disable loop all");
  Serial.println(" L3 - loop folder 3");
  Serial.println(" l3 - loop file 3");
  Serial.println(" A3 - advertise file 3");
  Serial.println(" a - stop advertise "); 
  Serial.println(" qx - query No. x");
  Serial.println("     x = 1 - read state");
  Serial.println("     x = 2 - read volume");
  Serial.println("     x = 3 - read equalizer");
  Serial.println("     x = 4 - read file counts");
  Serial.println("     x = 5 - read current file number");
  Serial.println("     x = 6 - read file counts in folder");
  Serial.println("     x = 7 - read folder counts");
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }  
}
