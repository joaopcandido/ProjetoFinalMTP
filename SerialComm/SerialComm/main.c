#include <windows.h>
#include <stdio.h>

// Ler caractere
char SerialGetc(HANDLE *hCom)
{
	char rxchar;
	BOOL bReadRC;
	static DWORD iBytesRead;

	bReadRC = ReadFile(*hCom, &rxchar, 1, &iBytesRead, NULL);
	return rxchar;
}

// Escrever caractere
void SerialPutc(HANDLE hCom, char txchar)
{
	BOOL bWriteRC;
	static DWORD iBytesWritten;

	bWriteRC = WriteFile(hCom, &txchar, 1, &iBytesWritten,NULL);

	return;
}

// Ler string
char* SerialGets(HANDLE *hCom)
{
	static char rxstring[256];
	char c;
	int pos = 0;

	while(pos <= 255)
	{ 
		c = SerialGetc(hCom); 
		if (c==13) break;
		if(c == '\r') continue; // discard carriage return 
		rxstring[pos++] = c; 
		if(c == '\n') break; 

	} 
	rxstring[pos] = 0; 
	return rxstring; 
} 
		
// Escrever string
void SerialPuts(HANDLE *hCom, char *txstring) 
{ 
	BOOL bWriteRC; 
	static DWORD iBytesWritten; 
	bWriteRC = WriteFile(*hCom, txstring, strlen(txstring), &iBytesWritten,NULL); 
} 

typedef struct
{
	char nome[50];
	char placa[9];
	char senha[5];
	struct Moradores *next;
} Moradores;

Moradores *F = NULL;
int n = 0, i;

Moradores *alocarNo()
{
	Moradores *novo = NULL;
	novo = (Moradores*)malloc(sizeof(Moradores));
	novo->next = NULL;
	return novo;
}

void cadastrar()
{
	Moradores *atual;
	if (F == NULL)
	{
		F = alocarNo();
		atual = F;
	}
	else
	{
		atual = F;
		if (atual->next == NULL)
		{
			atual->next = (struct Moradores *) alocarNo();
			atual = (Moradores *)atual->next;
		}
		else
		{
			while (atual->next != NULL) atual = (Moradores *)atual->next;
			atual->next = (struct Moradores *)alocarNo();
			atual = atual->next;
		}
	}
}

int main(int argc, char *argv[])
{
   char opcao = '0';
   DCB dcb;
   HANDLE hCom; //referencia
   BOOL fSuccess;
   LPCWSTR LpcCommPort = L"COM2";

   hCom = CreateFile( LpcCommPort,
                    GENERIC_READ | GENERIC_WRITE,
                    0,    // must be opened with exclusive-access
                    NULL, // no security attributes
                    OPEN_EXISTING, // must use OPEN_EXISTING
                    0,    // not overlapped I/O
                    NULL  // hTemplate must be NULL for comm devices
                    );

   if (hCom == INVALID_HANDLE_VALUE) 
   {
       // Handle the error.
       printf ("CreateFile failed with error %d.\n", GetLastError());
       return (1);
   }

   // Build on the current configuration, and skip setting the size
   // of the input and output buffers with SetupComm.

   fSuccess = GetCommState(hCom, &dcb);

   if (!fSuccess) 
   {
      // Handle the error.
      printf ("GetCommState failed with error %d.\n", GetLastError());
      return (2);
   }

   // Fill in DCB: 57,600 bps, 8 data bits, no parity, and 1 stop bit.

   dcb.BaudRate = CBR_9600;     // set the baud rate (velocidade de comunicacao)
   dcb.ByteSize = 8;             // data size, xmit, and rcv
   dcb.Parity = NOPARITY;        // no parity bit
   dcb.StopBits = ONESTOPBIT;    // one stop bit

   fSuccess = SetCommState(hCom, &dcb);

   //

   if (!fSuccess) 
   {
      // Handle the error.
      printf ("SetCommState failed with error %d.\n", GetLastError());
      return (3);
   }

   printf ("Serial port successfully reconfigured.\n");
   
   
   while(1)
   {
	    
	   printf("Dado enviado para o arduino: %c\n", opcao);
	   SerialPutc(hCom, opcao); // envia o dado

	   // Inverte o dado enviado
	   if (opcao=='0')
	   {
		  opcao = '1';
	   }
	   else
	   {
	      opcao = '0';
	   }
	   
	   // Aguarda 1 segundo
	   Sleep(1000);

   } // alterna entre 0 e 1 a cada 1 seg

   CloseHandle(hCom);
   system("PAUSE");
   return (0);
}