/* My deck WiFi monitor and Activity suggestor.

 very helpful for i2c lcd: https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/
 single to double quote: https://tools.knowledgewalls.com/convert-single-quotes-to-double-quotes-online
 
 ashraf minhaj
 ashraf_minhaj@yahoo.com
*/

#include "WiFi.h"
#include <Wire.h> 
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>


const char* ssid     = "";
const char* password = "";

String act_url = "https://www.boredapi.com/api/activity";
String dt_url  = "http://worldtimeapi.org/api/timezone/Asia/Dhaka";
String asm_husna_url = "https://api.aladhan.com/v1/asmaAlHusna/77";
/*{"abbreviation":"+06","client_ip":"0.0.0.0","datetime":"2022-09-27T00:55:08.473344+06:00","day_of_week":2,"day_of_year":270,"dst":false,"dst_from":null,"dst_offset":0,"dst_until":null,"raw_offset":21600,"timezone":"Asia/Dhaka","unixtime":1664218508,"utc_datetime":"2022-09-26T18:55:08.473344+00:00","utc_offset":"+06:00","week_number":39}*/ 
/*{"code":200,"status":"OK","data":[{"name":"\u0627\u0644\u0652\u0648\u064e\u0627\u0644\u0650\u064a","transliteration":"Al Waali","number":77,"en":{"meaning":"The Governor"}}]}*/

String asmaul_husna[] = {
  "Ar-Rahmaan", "Ar-Raheem", "Al-Malik", "Al-Quddus", "As-Salam",
  "Al-Mu’min", "Al-Muhaymin", "Al-Aziz", "Al-Jabbar",
  "Al-Mutakabbir", "Al-Khaaliq", "Al-Baari", "Al-Musawwir",
  "Al-Ghaffaar", "Al-Qahhaar", "Al-Wahhab", "Ar-Razzaq", "Al-Fattah",
  "Al-Alim", "Al-Qaabid", "Al-Baasit", "Al-Khaafid", "Ar-Rafi",
  "Al-Mu’izz", "Al-Muzil", "As-Sami’", "Al-Baseer", "Al-Hakam",
  "Al-Adl", "Al-Lateef", "Al-Khabeer", "Al-Haleem", "Al-Azeem",
  "Al-Ghafoor", "Ash-Shakoor", "Al-Aliyy", "Al-Kabeer", "Al-Hafiz",
  "Al-Muqeet", "Al-Haseeb", "Al-Jaleel", "Al-Kareem", "Ar-Raqeeb",
  "Al-Mujeeb", "Al-Waasi’", "Al-Hakeem", "Al-Wadud", "Al-Majeed",
  "Al-Ba’ith", "Ash-Shaheed", "Al-Haqq", "Al-Wakeel", "Al-Qawwiyy",
  "Al-Mateen", "Al-Waliyy", "Al-Hameed", "Al-Muhsee", "Al-Mubdi",
  "Al-Mueed", "Al-Muhyi", "Al-Mumeet", "Al-Hayy", "Al-Qayyoom",
  "Al-Waajid", "Al-Maajid", "Al-Waahid", "Al-Ahad", "As-Samad",
  "Al-Qaadir", "Al-Muqtadir", "Al-Muqaddim", "Al-Mu’akhkhir",
  "Al-Awwal", "Al-Akhir", "Az-Zaahir", "Al-Baatin", "Al-Waali",
  "Al-Muta’ali", "Al-Barr", "At-Tawwaab", "Al-Muntaqim", "Al-Afuww",
  "Ar-Ra’oof", "Maalik-ul-Mulk", "Zul-Jalaali-wal-Ikram",
  "Al-Muqsit", "Al-Jaami’", "Al-Ghaniyy", "Al-Mughni", "Al-Maani’",
  "Ad-Daarr", "An-Naafi’", "An-Noor", "Al-Haadi", "Al-Badi’",
  "Al-Baaqi", "Al-Waaris", "Ar-Rasheed", "As-Saboor"
  };
 
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
byte Check[] = {
  B00000,B00001,B00011,B10110,B11100,B01000,B00000,B00000
  };

byte Cross[] = {B10001,B01010,B00100,B01010,B10001,B00000,B00000,B00000};

String quote = "";
String dt = "";

// to make delays without interruptions
unsigned long previousMillis = 0;    // will store last time it was updated
const long interval = 20000;         // interval at which to iterate(milliseconds)


String add_space(String text, int max_len){
  int text_len = text.length();
  if(text_len < max_len){
    for( int i=text_len; i< max_len-1; i++){
      text += " ";
      }
    return text;
   }
}

void lcd_update(bool stat, bool online_stat, String text, String dt){
  String text1 = "";
  String text2 = "";
  
  // lcd.clear();
  if(stat != 1){
    lcd.clear();
    delay(200);
  }
  
  lcd.setCursor(0,0);
  lcd.print(add_space(asmaul_husna[random(99)], 19 ));
  //lcd.print(ssid);

  lcd.setCursor(18,0);
  if (stat == 1){
    // lcd.createChar(3, Check);
    lcd.write(0);
    //lcd.noBlink();
  }
  if (stat == 0){
    lcd.write(1);
    //lcd.blink();
  }

  lcd.setCursor(19, 0);
  if (online_stat == 1){
    // lcd.createChar(3, Check);
    lcd.write(0);
    lcd.noBlink();
  }
  if (online_stat == 0){
    lcd.write(1);
    //lcd.blink();
  }

  lcd.setCursor(0, 1);
  lcd.print(dt);

//  lcd.setCursor(17, 1);
//  lcd.print(online_stat);

  if (text.length() > 20){
    text1 = text.substring(0, 20);
    text2 = add_space(text.substring(20, 40), 20);
    // text2 = add_space(text2);
    // text.length();

    lcd.setCursor(0,2);
    lcd.print(text1);
    lcd.setCursor(0,3);
    lcd.print(text2);
    }
   if (text.length() <= 20){
    text1 = add_space(text.substring(0, 20), 20);
    text2 = "                    ";
    lcd.setCursor(0,2);
    lcd.print(text1);
    lcd.setCursor(0,3);
    lcd.print(text2);
   }

  if (online_stat == 0){
    //lcd.write(1);
    lcd.setCursor(19, 0);
    lcd.blink();
  }
}


String get_quote(String url){
  /* Check time and net */ 
    HTTPClient http;
    http.begin(url.c_str());
    
    // Send HTTP GET request
    int httpResponseCode = http.GET();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    if (httpResponseCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      int end_index = payload.indexOf(",");
      String activity = payload.substring(13, end_index-1);

      if(activity.length() > 40){
        return get_quote(url);
      }
      
      return activity;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        }
      
     // Free resources
     http.end();

   return "nothing";
}

void setup() {
  Serial.begin(115200);

  Serial.println("Connecting with LCD, and wifi");
  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  
  // Print a message to the LCD.
  lcd.backlight();

  lcd.createChar(0, Check);
  lcd.createChar(1, Cross);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    lcd_update(0, 0, quote, dt);
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
   bool online_stat     = 0;
   String wifi_stat     = "trying";
   String payload       = "";
   int httpResponseCode = 0;
   dt                   = "";
   
   
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    
    lcd_update(0, 0, quote, dt);
    
    WiFi.disconnect();
    delay(200);
    WiFi.reconnect();
    delay(2000);
  }

  Serial.println("WiFi Connected");

  /* Check time and net */ 
  HTTPClient http;
  http.begin(dt_url.c_str());
      
  // Send HTTP GET request
  httpResponseCode = http.GET();

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
    
  if (httpResponseCode == 200) {
    payload = http.getString();
    Serial.println(payload);
    online_stat = 1;

    //{"abbreviation":"+06","client_ip":"103.229.86.0","datetime":"2022-09-27T00:55:08.473344+06:00",
    int start_index = payload.indexOf("datetime");
    dt = payload.substring(start_index+11, start_index+27);
    Serial.println(dt);
    }
    else {
      online_stat = 0;
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      
   // Free resources
   http.end();

   // check interval
   unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
    // save the last time we iterated
    previousMillis = currentMillis;
    
    Serial.println("Get Quote if net present");
    if (online_stat != 0){
      Serial.println("Net present, getting quote");
      String text = get_quote(act_url);
      if (text != "none"){
        quote = text;
        } 
      }
   }
  
  
  lcd_update(1, online_stat, quote, dt);
  delay(5000);
  }
