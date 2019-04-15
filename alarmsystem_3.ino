/*   
 alarma motocicleta pentru Razvan Lazar si Mircea Craciun
 progrma scris de Nicu Florica (niq_ro)
 http;//www.arduinotehniq.com/
 http://nicuflorica.blogspot.ro/
 http://arduinotehniq.blogspot.com
 13.04.2019 - Craiova, Romania
 v.3 - add stored last state (in EEPROM) - 15.04.2019
*/

#include <EEPROM.h> // https://www.arduino.cc/en/Tutorial/EEPROMUpdate
#define adresa 101 // adresa emorare stare

#define pinarmare 8  // pin conectare comanda armare
#define pindezarmare 7  // pin conectare comanda dezarmare
#define pinsenzor 3  // pin conectare senzor (PIR, Doppler)
#define pingoarna 4  // pin conectare comanda claxon / goarna
#define pinstare 5 // pin indicare stare alarms (armat/nearmat
unsigned long tptipat = 10000;  // 30000 = 30s, timp sunat alarma
unsigned long tpinceput; // timp 

byte armare = 1;  // variabila pentru armare/dezarmare
byte miscare = 0; // variabila pentru miscare
int secunda = 0; // variabila pewntru a afla secundele, ca timp  

void setup()
{
Serial.begin(9600); // serial monitor
Serial.println("-----------------");
 Serial.println("Alarm system by Nicu FLORICA (niq_ro) !");
Serial.println("-----------------");
pinMode(pinarmare,INPUT);
pinMode(pindezarmare,INPUT);
pinMode(pinsenzor,INPUT);
digitalWrite(pinarmare,HIGH);
digitalWrite(pindezarmare,HIGH);

pinMode(pingoarna,OUTPUT);
pinMode(pinstare,OUTPUT);
digitalWrite(pingoarna,LOW);
digitalWrite(pinstare,LOW);

 armare = EEPROM.read(adresa);  // citire stare alarma 
 if (armare > 2)
 {
  EEPROM.update(adresa, 0);  // memorare stare alarma ca dezarmata
  armare = 0;  // daca nu s-a mai memorat starea
 }
if (armare == 1)  Serial.println("System armed !");
else
 Serial.println("System in stand-by !"); 
}

void loop() 
{
  
if (digitalRead(pinarmare) == LOW)  // daca s-a apasat butonul de armare
// if (digitalRead(pinarmare) == HIGH)  // daca s-a apasat butonul de armare (radio)
{
 Serial.println("System armed !");
 EEPROM.update(adresa,1);  // memorare stare alarma
 armare = 1;  // starea de alarma va deveni activa 
 miscare = 0; // se considera ca nu e miscare
 digitalWrite(pingoarna, HIGH); // claxonul v-a suna scurt pentr a sti ca s-a activat alarma
 delay(100);
 digitalWrite(pingoarna, LOW);
 delay(2000);
}

if (digitalRead(pindezarmare) == LOW)  // daca s-a apasat butonul de dezarmare
// if (digitalRead(pindezarmare) == HIGH)  // daca s-a apasat butonul de dezarmare (radio)
{
 Serial.println("System in stand-by !");
 EEPROM.update(adresa, 0);  // memorare stare alarma
 digitalWrite(pinstare, LOW);  // sting ledul de indicare armare
 armare = 0;  // starea de alarma va deveni inactiva 
 miscare = 0;  // se considera ca nu mai e miscare
 digitalWrite(pingoarna, HIGH); // claxonul v-a suna scurt, de 2 ori, pentru a sti ca s-a dezactivat alarma
 delay(100);
 digitalWrite(pingoarna, LOW);
 delay(100);
 digitalWrite(pingoarna, HIGH);
 delay(100);
 digitalWrite(pingoarna, LOW);
}

if ((armare == 1) and (miscare == 0))  // daca alarma este activa
{
  digitalWrite(pinstare, HIGH); // aprind led armare
 if (digitalRead(pinsenzor) == HIGH)  // daca s-a activat senzorul de miscare cat timp alarma e activa
 //if (digitalRead(pinsenzor) == LOW)  // daca s-a activat senzorul de miscare cat timp alarma e activa 
 {
   Serial.println("Movements!");
   miscare = 1;  // s=a detecat miscare
   delay(500); // pauza mica
   tpinceput = millis(); // se memoreaza timpul de setectare miscare
 }
else
{
   digitalWrite(pinstare, LOW); //sting led armare
}
}

if ((armare == 1) and (miscare == 1))  // daca alarma e activat si a fost detectata anterior miscare
{
  if ((millis() - tpinceput) < tptipat) // daca e in perioada de avertizare sonora
  {
    secunda = millis()/1000;  // se transforma in secunda timpul
    if (secunda%2 == 1) // daca secunda e numar impar
    {
      digitalWrite(pingoarna, HIGH); // cloaxon alimntat
    }
    else
    {
      digitalWrite(pingoarna, LOW); // cloaxon oprit
    }
  }
  else
  {
    miscare = 0;  // se considera ca nu mai e miscare
    digitalWrite(pingoarna, LOW); // cloaxon oprit
  }
}


} // sfarsit program principal

