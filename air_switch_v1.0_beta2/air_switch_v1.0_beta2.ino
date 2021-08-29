//www.nyebarilmu.com
 
#include <ESP8266WiFi.h>
 
const char* ssid = "SMK BP OFFICE 2"; // Isi dengan nama wifi anad
const char* password = "jagalahhati"; //wifi password nya
WiFiServer server(80);
 
#define pin_relay 0 // pin relaya pada GPIO0
 
void setup() 
{
  Serial.begin(115200); //baud komunikasi
 
  pinMode(pin_relay,OUTPUT); //didefinisikan sebagai output
  digitalWrite(pin_relay, LOW); //nilai awalnya low = tidak aktif
 
  //Memulai menghubungkan ke wifi
  Serial.println();
  Serial.println();
  Serial.print("Menghubungkan dengan wifi...");
  Serial.println(ssid);
 
 //prosedur koneksi ke wifi yang dijadikan target dan password
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("."); //..........
  }
  Serial.println("");
  Serial.println("terhubung ke WiFi Anda");
  server.begin();
  Serial.println("Siap digunakan");
 
  //menampilkan alamat IP address
  Serial.print("Gunakan IP ini sebagai kendali Relay : ");
  Serial.print("https://192.168.0.100/"); //ketikan di browser hp atau komputer
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() 
{
  //cek kondisi jika terhubung dengan module
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  // menunggu sampai module relay programming kirim data
  Serial.println("Client Baru");
  while(!client.available())
  {
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Mencocokan permintaan awal
  int value = LOW;
  if (request.indexOf("/pin_relay=ON") != -1)  
  {
    Serial.println("Kondisi_Relay=ON");
    digitalWrite(pin_relay,LOW);
    value = LOW;
  }
  if (request.indexOf("/pin_relay=OFF") != -1)  
  {
    Serial.println("Kondisi_Relay=OFF");
    digitalWrite(pin_relay,HIGH);
    value = HIGH;
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  this is a must
  client.println("<!DOCTYPE HTML>");
  client.println("<html style='color: #FF8C00; background: #202020; font-family: montserrat'>");

  client.println("<head>");
  client.println("<title>Air Switch</title>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("</head>");
  client.println("<br>");

  client.println("<h1 align='center'>Selamat Datang</h1>");
  client.println("<p align='center'>Ini adalah Dashboard <b>Air Switch v1.0</b></p>");
  client.println("<br><hr><br>");
  client.println("<h2 align='center' style='color: white'>Status : ");
  if(value == HIGH) 
  {
    client.print("OFF");
  } 
  else 
  {
    client.print("ON");
  }
  client.println("</h2>");
  client.println("<br>");

  client.println("<div align='center'>");
  client.println("<button style='background: #2E8B57; width: 25%; border-radius: 12px'><a href=\"/pin_relay=ON\"><h2 style='color: white'>ON</h2></a></button>");
  client.println("<br><br>");
  client.println("<button style='background: #DC143C; width: 25%; border-radius: 12px'><a href=\"/pin_relay=OFF\"><h2 style='color: white'>OFF</h2></a></button>");
  client.println("</div>");
  client.println("<br><br><hr><br>");

  client.println("<div align='center'>");
  client.println("<footer>");
  client.println("<b>Masipnu </b>Official");
  client.println("<br>");
  client.println("&copy; 2021 ");
  client.println("</footer>");
  client.println("</div>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
 
