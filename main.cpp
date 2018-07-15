// ===============================================================================
//   
//   invio di un file con protocollo XMODEM 
// ===============================================================================


#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#using <System.dll>



struct PacchettoXModem {
    char Soh;       //Start Of header
    char NumP;      //numero del pacchetto
    char NumPNeg;   //numero del pacchetto Negato
    char Data[128]; // dati da inviare
    char CheckSum;  //contiene il checksum di solo idati
};


#pragma warning( disable : 4996 )

using namespace System;
using namespace System::IO::Ports;
using namespace System::Text;

// assegna nome al codice ASCII del tasto ESC della tastiera
#define EOT  4
#define ACK  6
#define NACK 21
#define TASTOESC 27



// Funzione per convertire da Stringa 'standard C' in stringa 'wchar_t'
void str_TO_wchar_t(char *PtrStr, array <wchar_t> ^Buff, int NCar);
// Funzione per convertire da Stringa 'wchar_t' in stringa 'standard C' 
void wchar_t_TO_str(array <wchar_t> ^Buff,char *PtrStr,  int NCar);


// Funzione per convertire da Stringa 'standard C' in stringa '<Byte>'
void str_TO_Byte(char *PtrStr, array <Byte> ^Buff, int NCar);
// Funzione per convertire da Stringa '<Byte>' in stringa 'standard C' 
void Byte_t_TO_str(array <Byte> ^Buff,char *PtrStr,  int NCar);

int TX_XModem(char *NomeFile, SerialPort ^MyPort );


void main (void)  {
    int SommaAscii;
    int Car;          // buffer temporaneo per TX 
    int i; // contatore generico
    int CarInt,CarIntMod;                 //buffer temporaneo per ricezione caratter via seriale    
    SerialPort ^MiaPorta232;    // Variabile di 'tipo' RS232
//    ASCIIEncoding^ ascii = gcnew ASCIIEncoding;
    char MiaStr[1025]={ 'A', 'B' , '{', '}' ,48,49,36,36,'\0' };
    char Messaggio[]="Batti il tasto ESC per interropere la comunicazione\n\n";
    char MiaStrTest[1025];    
    char NomeFile[128];
	char MiaStrMia[100];
	char Str[100];
	int POS=0;

    //array <wchar_t> ^Buff2 = { '?', 'B' , '{', '}', '}', '}'};
    //array <wchar_t> ^Buff_wchar_t = gcnew array <wchar_t> (260);  // 260 caratteri in formato wchar_t
    array <Byte> ^MioBuffByte= gcnew array <Byte> (260);  // 260 caratteri in formato <Byte>

    array <unsigned char>  ^MioBuff_uChar= gcnew array <unsigned char> (260);  // 260 caratteri in formato <unsigned char>

    struct PacchettoXModem MyPacket; //paccheto XModem da inviare


    printf ("\n---------- esempio uso protocollo XMODEM  con Visual Studio Console--------------\n" );
    printf ("\n Utilizzata porta COM3 a 38400,n,8,1\n" );
    //printf ("\nMiaStr       =%s\n", Messaggio);
    
    //str_TO_Byte(Messaggio, MioBuffByte, strlen(Messaggio));
 
    //printf ("\nBuff_wchar_t  =%s<<\n", MioBuffByte);  
    //Console::Write("\nBuff_wchar_t  =");       Console::WriteLine(Buff_wchar_t);
    

   
    // ---- inizializzazione parametri di base per la comunicazione Seriale ----
    MiaPorta232 = gcnew SerialPort();
    MiaPorta232->BaudRate=38400;   // Fisso la velocita di trasmissione
    MiaPorta232->PortName="COM5";   // scelgo la porta disponibile---------------------------------------------------
    MiaPorta232->Encoding->Default;
    MiaPorta232->Open();            // apro la seriale, da questo momento posso RX e TX
    

   // printf (" Digita il nome completo del file da trasmettere " );    
   // gets(NomeFile);
   // //TX_XModem("C:\\cacca\\_MioTESTO.TXT", MiaPorta232);
   // TX_XModem(NomeFile, MiaPorta232);

   // MiaPorta232->Open();            // apro la seriale, da questo momento posso RX e TX
   // // invio via RS232 una stringa di messaggio all'utente remoto
   // //   MiaPorta232->Write(Buff_wchar_t,0,strlen(Messaggio)); // OK OK OK OK!!!!!

   //return;    
    
    // vecchio progrmma di chat
    
    // ==============================================================
    //                  CHAT via RS232 
    // ==============================================================
  
    do {
        // controlla se Φ presente un carattere nel buffer di tastiera
        if (_kbhit()!=0 ) {
            
            // invia un carattere alla volta in tempo reale
            Car=getche();

            MioBuffByte[0]='A';
			MioBuffByte[1]=Car;

			MiaPorta232->Write(MioBuffByte,0,2); // Invia un carattere via seriale OK!!!!!
			
            //Buff_wchar_t[1]='\0';  // per precauzione 
             if (Car==13) {
                printf ("\n");  // stampa una andata a capo nel proprio schermo
                //invia un avanzamento riga al terminale connesso via seriale
                MioBuffByte[0]= 10;
                MiaPorta232->Write(MioBuffByte,0,1); // OK OK OK OK!!!!!
            }
        }
        
		//-------------------------------------------------------------------------------------------------------------------------

		//TRADUTTORE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		if (MiaPorta232->BytesToRead>0) {
            //Φ arrivato un carattere e lo stampa a schermo
            CarInt=MiaPorta232->ReadChar();  //metto in CarInt il carattere in formato ASCII
            putch(CarInt);

			Str[POS]=CarInt;
			POS++;
			Str[6]='\0';
			
			sprintf(MiaStrMia, "%03d",CarInt);  //metto il carattere ASCII dentro stringa
			
			MioBuffByte[0]='\n';
			MioBuffByte[1]='\r';
			MioBuffByte[2]='A';
			MioBuffByte[3]='S';
			MioBuffByte[4]='C';
			MioBuffByte[5]='I';
			MioBuffByte[6]='I';
			MioBuffByte[7]=' ';
			MioBuffByte[8]='-';
			MioBuffByte[9]='-';
			MioBuffByte[10]='-';
			MioBuffByte[11]='>';
			MioBuffByte[12]=' ';
			MioBuffByte[13]=MiaStrMia[0];
			MioBuffByte[14]=MiaStrMia[1];
			MioBuffByte[15]=MiaStrMia[2];
			
			 //do{
			//
			//MiaPorta232->Write(MioBuffByte,0,16); // Invia un carattere via seriale OK!!!!!

			//}while(1);

			//MioBuffByte[1]=CarInt;  //metto nel buffer di scrittura il carattere ASCII

			MiaPorta232->Write(MioBuffByte,0,16); // Invia un carattere via seriale OK!!!!!

            //ORA TRADUCO
			//albero
			if((Str[0]==97)&&(Str[1]==108)&&(Str[2]==98)&&(Str[3]==101)&&(Str[4]==114)&&(Str[5]==111)&&(Str[6]==0)&&(POS==6))
			{
			
				printf("\n\n\n hai scritto albero");
				printf("\n\n\n tree");
				POS=0;
			
			}
			//casale
			if((Str[0]==99)&&(Str[1]==97)&&(Str[2]==115)&&(Str[3]==97)&&(Str[4]==108)&&(Str[5]==101)&&(Str[6]==0)&&(POS=6))
			{
			
				printf("\n\n\n hai scritto casale");
				printf("\n\n\n house");
				POS=0;
			
			}
			
			
			
			if (CarInt==13) printf("\n");
        }
		
		//-------------------------------------------------------------------------------------------------------------------------
		
		////// E' ARRIVATO UN CARATTERE IN ENTRATA
		//////controlla se Φ arrivato un carattere da seriale 
  ////       if (MiaPorta232->BytesToRead>0) {
  ////          //Φ arrivato un carattere e lo stampa a schermo
  ////          CarInt=MiaPorta232->ReadChar();  //metto in CarInt il carattere in formato ASCII
  ////          putch(CarInt);



		////	sprintf(MiaStrMia, "%03d",CarInt);  //metto il carattere ASCII dentro stringa
		////	
		////	MioBuffByte[0]='\n';
		////	MioBuffByte[1]='\r';
		////	MioBuffByte[2]='A';
		////	MioBuffByte[3]='S';
		////	MioBuffByte[4]='C';
		////	MioBuffByte[5]='I';
		////	MioBuffByte[6]='I';
		////	MioBuffByte[7]=' ';
		////	MioBuffByte[8]='-';
		////	MioBuffByte[9]='-';
		////	MioBuffByte[10]='-';
		////	MioBuffByte[11]='>';
		////	MioBuffByte[12]=' ';
		////	MioBuffByte[13]=MiaStrMia[0];
		////	MioBuffByte[14]=MiaStrMia[1];
		////	MioBuffByte[15]=MiaStrMia[2];
		////	
		////	 //do{
		////	//
		////	//MiaPorta232->Write(MioBuffByte,0,16); // Invia un carattere via seriale OK!!!!!

		////	//}while(1);

		////	//MioBuffByte[1]=CarInt;  //metto nel buffer di scrittura il carattere ASCII

		////	MiaPorta232->Write(MioBuffByte,0,16); // Invia un carattere via seriale OK!!!!!

  ////          
		////	
		////	
		////	
		////	if (CarInt==13) printf("\n");
  ////      }

    }while ((CarInt!=TASTOESC)&& (Car!=TASTOESC));
    
    if (Car==TASTOESC)      printf ("\n\n\a\a Programma chat terminato da utente locale");
    if (CarInt==TASTOESC)   printf ("\n\n\a\a Programma chat terminato da utente REMOTO");
                
    MiaPorta232->Close();
    Sleep(2000);
     
} // fine main



// FUNZIONI



// ======================================================================
// Funzione per convertire da Stringa 'standard C' in stringa '<Byte>'
// ======================================================================
// con i <Byte> si possono RX/TX con serialport.write() anche ASCII > 127
// ======================================================================
// <input> PtrStr : indirizzo iniziale dei caratteri della stringa da convertire
// <input> Buff   : indirizzo iniziale dei caratteri della stringa convertita
// <input> NCar   : numero totale di caratteri da  convertire 
void str_TO_Byte(char *PtrStr, array <Byte> ^Buff, int NCar)
{
    int i;

    for (i=0; i < NCar; i++ ) {
        Buff[i]=(unsigned char) *PtrStr;
        PtrStr++;
    }
        
} // fine funzione conversione da char * a Byte


// Funzione per convertire da Stringa '<Byte>' in stringa 'standard C' 
// ======================================================================
// con i <Byte> si possono RX/TX con serialport.write() anche ASCII > 127
// ======================================================================
// <input> Buff   : indirizzo iniziale dei caratteri della stringa da convertire
// <input> PtrStr : indirizzo iniziale dei caratteri della stringa convertita
// <input> NCar   : numero totale di caratteri da  convertire 
void Byte_t_TO_str(array <Byte> ^Buff,char *PtrStr,  int NCar)
{   int i;

    for (i=0; i < NCar; i++ ) {
        *PtrStr=(unsigned char)Buff[i];
        PtrStr++;
    }
        
} // fine funzione conversione da <Byte> a char *




// Funzione per convertire da Stringa 'standard C' in stringa 'wchar_t'
// <input> PtrStr : indirizzo iniziale dei caratteri della stringa da convertire
// <input> Buff   : indirizzo iniziale dei caratteri della stringa convertita
// <input> NCar   : numero totale di caratteri da  convertire 
void str_TO_wchar_t(char *PtrStr, array <wchar_t> ^Buff, int NCar)
{
    int i;

    for (i=0; i < NCar; i++ ) {
        Buff[i]=(unsigned char) *PtrStr;
        PtrStr++;
    }
        
} // fine funzione conversione da char * a wchar_t



// Funzione per convertire da Stringa 'wchar_t' in stringa 'standard C' 
// <input> Buff   : indirizzo iniziale dei caratteri della stringa da convertire
// <input> PtrStr : indirizzo iniziale dei caratteri della stringa convertita
// <input> NCar   : numero totale di caratteri da  convertire 
void wchar_t_TO_str(array <wchar_t> ^Buff,char *PtrStr,  int NCar)
{   int i;

    for (i=0; i < NCar; i++ ) {
        *PtrStr=(char)Buff[i];
        PtrStr++;
    }
        
} // fine funzione conversione da wchar_t a char *


//
//trasmette un file con Protocollo XMODEM
// <input> Nomefile : nome del file,completo di percorso,  da inviare
// <input> MyPort   : porta seriale da cui trasmettere
//                    i parametri di comunicazione devono essere impostati dal prog. Chiamante
// apre e chiude in ogni caso la porta prima di terminare    
// restituisce 0 se tutto OK
int TX_XModem(char *NomeFile, SerialPort ^MyPort )
{
    FILE *FhMioFile;          //puntatore al file da leggere
    struct PacchettoXModem MyPacket; //paccheto XModem da inviare
    int i;                  // contatore generico
    int CarInt;             // carattere letto da seriale
    char BuffReadFile[132]; // buffer per lettura file da disco
    int SommaAscii;         // per calcolo checksum
    unsigned char NPacket=1;// Numero progressivo del pacchetto da inviare
    int NCar;               // nuemro dei caratteri realmente letti da file

    array <Byte> ^MioBuffByte= gcnew array <Byte> (134);  // 134 caratteri in formato <Byte>
                                                          // buffer compatibile con serialport.write()      
    
    MyPort->Open();            


    FhMioFile=fopen(NomeFile, "rb" );
    if (FhMioFile==NULL) {
        MyPort->Close();
    }
    else {
        // attendi il primo NACK da remoto e invia in risposta un pacchetto X-MODEM
        do {
            if (MyPort->BytesToRead>0) {
                //e' arrivato un carattere e lo stampa a schermo
                CarInt=MyPort->ReadChar();
                putch(CarInt);            
            }
           
        } while (CarInt!= NACK);

        printf ("\nricevuto NACK");
        MyPacket.Soh=1;
        
        CarInt=ACK;
        do {
            MyPacket.NumP=NPacket;  // primo paccheto da inviare
            MyPacket.NumPNeg=~MyPacket.NumP;
            if (CarInt==ACK) {
                // leggi da file 128 byte ( o meno) di dati da trasmettere
                NCar=fread(BuffReadFile , 1 ,128, FhMioFile);
            }

            // riempio il pacchetto con stringa nota
            for (i=0; i< NCar; i++) 
                MyPacket.Data[i]=BuffReadFile[i];
    
            //riempio il resto con dati fittizi cio`e ASCII-0
            for (i=NCar; i<128; i++) 
                MyPacket.Data[i]=0; 
    

            printf ("\nfine riempito pacchetto");
            // calcolo checksum
            SommaAscii=0;
            for (i=0; i< 128; i++)  {
                SommaAscii=SommaAscii+MyPacket.Data[i];
            }
            SommaAscii= SommaAscii % 256;
    
            printf ("\nfine sommatoria check");

    
            // trasforma il paccheto in dati inviabili con serial.write()
            MioBuffByte[0]=MyPacket.Soh;
            MioBuffByte[1]=MyPacket.NumP;
            MioBuffByte[2]=MyPacket.NumPNeg;
            for (i=0; i< 128; i++)   MioBuffByte[3+i]= MyPacket.Data[i];
            MioBuffByte[131]=SommaAscii;
    
            // invia pacchetto 
            MyPort->Write(MioBuffByte,0,132); 

            printf ("\n inviato pacchetto N.%4d", NPacket);
            // attendi nuovo NACK e/o ACK
            do {
                if (MyPort->BytesToRead>0) {
                    //e' arrivato un carattere e lo stampa a schermo
                    CarInt=MyPort->ReadChar();
                    putch(CarInt);            
                }
 
                if (CarInt==NACK) {
                    printf (" Ricevuto NACK =====" );
                }
            } while ((CarInt!= NACK )&&  (CarInt!= ACK));

            if (CarInt==ACK) {
                NPacket++;   // incrementa numero del pacchetto per prossimo invio
            }
            // se arivato NACK reinvia lo stesso pacchetto di prima
        } while (!feof(FhMioFile));
        // invia EOT
        MioBuffByte[0]=EOT;
        MyPort->Write(MioBuffByte,0,1); 

        MyPort->Close();            
    }
    return 0;
}