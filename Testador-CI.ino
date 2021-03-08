#include <LiquidCrystal.h>

LiquidCrystal lcd(16, 17, 18, 19, 20, 21); 

int pin[12][2]={{54,57},{55,58},{56,59},{62,65},{63,66},{64,67},{23,3},{1,4},{2,5},{7,10},{8,11},{9,12}}; 


int tempo = 100;   
int tempodis = 3000;
int tempotes = 4000;

int estado;
int limite;

int apareceu1 = 0;
int apareceu2 = 0;
int apareceu3 = 1;

int botao = 6;   

int controleTipo = 1;
int controlePorta1 = 1;
int controlePorta2 = 1;

int ledgreen = 14;
int ledred = 15;


String line1 = "   Aperte o botao para testar o CI    ";
String frase[7] = {"nada", "CI  7408  AND", "CI  7400 NAND", "CI  7432  OR", "CI  7486  XOR","CI  7404  NOT", "CI  7402  NOR"};      

int confereCI(int qualTipo, int qualPorta);
int Tipo1 (int A, int AA, int B, int BB, int Y, int YY);
int Tipo2 (int A, int AA, int Y, int YY);
int Tipo3 (int A, int AA, int B, int BB, int Y, int YY);
void scroll_inf();


void setup() {

pinMode(ledgreen, OUTPUT);
pinMode(ledred, OUTPUT);
digitalWrite(ledgreen, LOW);
digitalWrite(ledred, LOW);

pinMode(botao, INPUT_PULLUP);
for(int i=0; i<12; i++){ 
  pinMode(pin[i][0], INPUT);
}

  lcd.begin(16,2);
       
}




void loop() {

 
  switch(controleTipo){
    case 1:
    limite = 4;
    estado = confereCI(controleTipo, controlePorta1);
    break;
    case 2:
    limite = 6;
    estado = confereCI(controleTipo, controlePorta2);
    break;
    case 3:
    limite = 4;
    estado = confereCI(controleTipo, controlePorta1);
  }


  if (estado == 5){    
    int certeza, porta;
    porta = controlePorta2;
    certeza = confereCI( 1, porta);
    if (certeza) estado = 0;
  }
  

  while (estado){

    digitalWrite(ledgreen, HIGH);
    apareceu1 = 0;
    
    if (apareceu3){ 
    switch (estado){
    case 1:
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CI  7408  AND"); 
    break;
    case 2:
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CI  7400 NAND");
    break;
    case 3:
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CI  7432  OR");
    break;
    case 4:
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CI  7486  XOR");
    break;
    case 5:
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CI  7404  NOT");
    break;
    case 6:
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CI  7402  NOR");
    break;
    }
    
    lcd.setCursor(3,1);
    lcd.print("detectado");
    delay(tempodis);
    }
    apareceu3 = 0;              
    if(!apareceu2){
      
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print(frase[estado]);
      lcd.setCursor(1, 1);
      lcd.print("Aperte o botao");
      
    }
    apareceu2 = 1;            


    
    if (!digitalRead(botao)){  
      
      digitalWrite(ledgreen, LOW);
      digitalWrite(ledred, HIGH);
      int danificado = 0;
      int n1;
      int n2;

      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Testando");
      lcd.setCursor(4,1);
      lcd.print("o  CI...");

      delay(tempodis);
      
      for (int i=1; i<=limite; i++){
        n1 = confereCI(controleTipo, i);
        if ( n1 == 0 ) danificado++ ;   
      }

      
      if (danificado){
        int boa;                       
        boa = limite - danificado;
        if ( boa==1 ){
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("Apenas a porta");
          lcd.setCursor(6,1);
          lcd.print("esta boa");
        }
        else{
          lcd.clear();
          lcd.setCursor(1,0);
          lcd.print("As portas"); 
          lcd.setCursor(6,1);
          lcd.print("estao boas");    
        }
        
        for (int i=1; boa; i++){                      
          n2 = confereCI(controleTipo, i);
          if ( n2 ){   

            if (boa == 5) {
              lcd.setCursor(11,0);
              lcd.print(i); 
              lcd.setCursor(12,0);
              lcd.print(",");
            }
            else if (boa == 4) { 
              lcd.setCursor(13,0);
              lcd.print(i);
              lcd.setCursor(14,0);
              lcd.print(",");
            }     
            else if (boa == 3) {
              lcd.setCursor(0,1);
              lcd.print(i);
              lcd.setCursor(1,1);
              lcd.print(","); 
            }
            else if (boa == 2) { 
              lcd.setCursor(2,1);
              lcd.print(i);
              lcd.setCursor(3,1);
              lcd.print("e");
            }
            else{
              lcd.setCursor(4,1);
              lcd.print(i);
            }
              
            boa--;
          }  
        }      
      }
      else {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Todas as portas");
        lcd.setCursor(3,1);
        lcd.print("estao boas");
      }

      delay(tempotes); 
      apareceu2 = 0;           
      apareceu3 = 1;   

      digitalWrite(ledgreen, HIGH);
      digitalWrite(ledred, LOW);
    }
   
    if (estado == 5) estado = confereCI(controleTipo, controlePorta2);
    else estado = confereCI(controleTipo, controlePorta1);   
         
  }


  if (!apareceu1){

    digitalWrite(ledgreen, LOW);

    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Insira");
    lcd.setCursor(1,1);
    lcd.print("Algum CI 74XX");
    
  }
  
  apareceu1 = 1;
  apareceu2 = 0;
  apareceu3 = 1;




  if (controleTipo == 2) controlePorta2++;
  
  if (controlePorta2 > limite) controlePorta2 = 1;
  
  if (controleTipo == 3) controlePorta1++;
  
  if (controlePorta1 > limite) controlePorta1 = 1;

  
  controleTipo++;  
  delay(100);                             
  if (controleTipo > 3) controleTipo = 1;

}











int confereCI(int qualTipo, int qualPorta)
{  
                                            
int situacao;

switch(qualTipo){                           
    case 1:
    if (qualPorta == 1) situacao = Tipo1( pin[0][1], pin[0][0], pin[1][1], pin[1][0], pin[2][0], pin[2][1]);
    else if (qualPorta == 2) situacao = Tipo1( pin[3][1], pin[3][0], pin[4][1], pin[4][0], pin[5][0], pin[5][1]);
    else if (qualPorta == 3) situacao = Tipo1( pin[7][1], pin[7][0], pin[8][1], pin[8][0], pin[6][0], pin[6][1]);
    else situacao = Tipo1( pin[10][1], pin[10][0], pin[11][1], pin[11][0], pin[9][0], pin[9][1]);
    break;
    
    case 2:
    if (qualPorta == 1) situacao = Tipo2( pin[0][1], pin[0][0], pin[1][0], pin[1][1] );
    else if (qualPorta == 2) situacao = Tipo2( pin[2][1], pin[2][0], pin[3][0], pin[3][1] );
    else if (qualPorta == 3) situacao = Tipo2( pin[4][1], pin[4][0], pin[5][0], pin[5][1] );
    else if (qualPorta == 4) situacao = Tipo2( pin[7][1], pin[7][0], pin[6][0], pin[6][1] );
    else if (qualPorta == 5) situacao = Tipo2( pin[9][1], pin[9][0], pin[8][0], pin[8][1] );
    else situacao = Tipo2( pin[11][1], pin[11][0], pin[10][0], pin[10][1] );
    break;
    
    case 3:
    if (qualPorta == 1) situacao = Tipo3( pin[1][1], pin[1][0], pin[2][1], pin[2][0], pin[0][0], pin[0][1]);
    else if (qualPorta == 2) situacao = Tipo3( pin[4][1], pin[4][0], pin[5][1], pin[5][0], pin[3][0], pin[3][1]);
    else if (qualPorta == 3) situacao = Tipo3( pin[6][1], pin[6][0], pin[7][1], pin[7][0], pin[8][0], pin[8][1]);
    else situacao = Tipo3( pin[9][1], pin[9][0], pin[10][1], pin[10][0], pin[11][0], pin[11][1]);
    break;    
  }

  return situacao;
  
}














int Tipo1 (int A, int AA, int B, int BB, int Y, int YY)
{   
  bool s[2][2];    
  bool t[2][2];    
    //pinos pseudo constantes
    digitalWrite(AA, LOW);
    digitalWrite(BB, LOW);
    digitalWrite(Y, HIGH);
    
    //pinos metamorfos
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(YY, INPUT);
    digitalWrite(YY, LOW);


    

  
  for(int i=0; i<2; i++){   //primeira leitura
    digitalWrite(A, i);
    for(int j=0; j<2; j++){
      digitalWrite(B, j);
      delay(tempo);
      s[i][j] = digitalRead(Y);
    }
  }
  

  for(int i=0; i<2; i++){  //segunda leitura
    digitalWrite(A, i);
    for(int j=0; j<2; j++){
      digitalWrite(B, j);
      delay(tempo);
      t[i][j] = digitalRead(Y);
    }
  }

       if(s[0][0]==t[0][0]&&s[0][1]==t[0][1]&&s[1][0]==t[1][0]&&s[1][1]==t[1][1]&&s[0][0]==0&&s[0][1]==0&&s[1][0]==0&&s[1][1]==1) return 1; //AND 7408
  else if(s[0][0]==t[0][0]&&s[0][1]==t[0][1]&&s[1][0]==t[1][0]&&s[1][1]==t[1][1]&&s[0][0]==1&&s[0][1]==1&&s[1][0]==1&&s[1][1]==0) return 2; //NAND 7400
  else if(s[0][0]==t[0][0]&&s[0][1]==t[0][1]&&s[1][0]==t[1][0]&&s[1][1]==t[1][1]&&s[0][0]==0&&s[0][1]==1&&s[1][0]==1&&s[1][1]==1) return 3; //OR 7432
  else if(s[0][0]==t[0][0]&&s[0][1]==t[0][1]&&s[1][0]==t[1][0]&&s[1][1]==t[1][1]&&s[0][0]==0&&s[0][1]==1&&s[1][0]==1&&s[1][1]==0) return 4; //XOR 7486
  else return 0;    
  
}
  










int Tipo2 (int A, int AA, int Y, int YY)  
{
  bool s[2];
  bool t[2];

    //pinos pseudo constantes
    digitalWrite(AA, LOW);
    digitalWrite(Y, HIGH);

    //pinos metamorfos
    pinMode(A, OUTPUT);
    pinMode(YY, INPUT);
    digitalWrite(YY, LOW);
    

  for(int i=0; i<2; i++){  //primeira leitura
    digitalWrite(A, i);
    delay(tempo);
    s[i] = digitalRead(Y);
  }
  
  for(int i=0; i<2; i++){  //segunda leitura
    digitalWrite(A, i);
    delay(tempo);
    t[i] = digitalRead(Y);
  }
  
  if(s[0]==t[0]&&s[1]==t[1]&&s[0]==1&&s[1]==0) return 5; //NOT 7404
  else return 0;
    
}









int Tipo3 (int A, int AA, int B, int BB, int Y, int YY)  
{

  bool s[2][2];
  bool t[2][2];
  
    //pinos pseudo constantes
    digitalWrite(Y, HIGH);
    digitalWrite(AA, LOW);
    digitalWrite(BB, LOW);
    
    //pinos metamorfos
    pinMode(YY, INPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    digitalWrite(YY, LOW);



    for(int i=0; i<2; i++){  //primeira leitura
    digitalWrite(A, i);
    for(int j=0; j<2; j++){
      digitalWrite(B, j);
      delay(tempo);
      s[i][j] = digitalRead(Y);
    }
  }
  

  for(int i=0; i<2; i++){  //segunda leitura
    digitalWrite(A, i);
    for(int j=0; j<2; j++){
      digitalWrite(B, j);
      delay(tempo);
      t[i][j] = digitalRead(Y);
    }
  }
    
    if(s[0][0]==t[0][0]&&s[0][1]==t[0][1]&&s[1][0]==t[1][0]&&s[1][1]==t[1][1]&&s[0][0]==1&&s[0][1]==0&&s[1][0]==0&&s[1][1]==0) return 6; //NOR 7402             
    else return 0;
 
}
