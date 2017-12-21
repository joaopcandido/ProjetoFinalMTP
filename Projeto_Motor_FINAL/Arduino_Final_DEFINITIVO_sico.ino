int Entrada1 = 12;
int Entrada2 = 10;
int EntradaLed = 13;
int Botao = 8;
int led2 = 7;
int botao2 = 4;
int infra1 = 3;
int infra2 = 5;
//int infra3 = 6;
char state = '0';
void setup()
{
  pinMode(Entrada1, OUTPUT);
  pinMode(Entrada2, OUTPUT);
  pinMode(EntradaLed, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(Botao, INPUT);
  pinMode(botao2, INPUT);
  pinMode(infra1, INPUT);
  pinMode(infra2, INPUT);
  //pinMode(infra3, INPUT);
  analogWrite(Entrada1, 0);
  analogWrite(Entrada2, 0);
  Serial.begin(9600);
}
boolean buttonState = 0;
boolean saveState = 0;
boolean botao2State = 0;
boolean saveState2 = 0;
void loop()
{
  int x, y, z;
  x = digitalRead(infra1);
  y = digitalRead(infra2);
  //z = digitalRead(infra3);
  if (Serial.available() > 0)
  {
    state = Serial.read();

    switch (state)
    {
      case '0':
        digitalWrite(led2, HIGH);
        buttonState = digitalRead(Botao);
        z = buttonState;
        if (z == 1)
        {
          if (x == 0)
          {
            digitalWrite(EntradaLed, 1);
            analogWrite(Entrada1, 255);
            analogWrite(Entrada2, 0);
          }
          Serial.write('0');
          do
          {
            delay(1);
          } while (y == 0);
          if (y == 1)
          {
            digitalWrite(EntradaLed, 1);
            analogWrite(Entrada1, 0);
            analogWrite(Entrada2, 0);
          }
        }
        else
        {
          if (y == 1)
          {
            delay(1000);
            digitalWrite(EntradaLed, 0);
            analogWrite(Entrada1, 0);
            analogWrite(Entrada2, 255);
          }
          do
          {
            delay(1);
          } while (x == 1);
          if (x == 0)
          {
            digitalWrite(EntradaLed, 0);
            analogWrite(Entrada1, 0);
            analogWrite(Entrada2, 0);
          }
        }
        break;
      case '1':
        digitalWrite(led2, LOW);
        Serial.write('1');
        analogWrite(Entrada1, 0);
        analogWrite(Entrada2, 0);
        break;

      default:
        break;
    }
  }
}

