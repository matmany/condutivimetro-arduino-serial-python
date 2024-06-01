//Programa: Display LCD 16x2
//Autor: MakerHero
 
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float tdsValue = 0;
void setup()
{
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
}
 
void loop()
{
  //Limpa a tela
  lcd.clear();
  //Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(3, 0);
  //Envia o texto entre aspas para o LCD
  lcd.print(tdsValue);
  lcd.setCursor(3, 1);
  lcd.print(" LCD 16x2");
  delay(5000);
  tdsValue = 1+tdsValue;
}