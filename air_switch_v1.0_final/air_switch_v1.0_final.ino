//www.nyebarilmu.com
 
#include <ESP8266WiFi.h>
 
const char* ssid = "SMK BP OFFICE 2"; // Isi dengan nama wifi anad
const char* password = "jagalahhati"; //wifi password nya
WiFiServer server(80);
 
#define pin_relay 0 // pin relaya pada GPIO0

IPAddress ip;
 
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

  client.println("<!DOCTYPE html>");
  client.println("<html lang='en'>");
  client.println("<head>");
  client.println("<meta charset='utf-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x' crossorigin='anonymous'>");
  client.println("<title>Air Switch</title>");
  client.println("</head>");
  client.println("<body class='bg-dark mt-3 d-flex h-100'>");
  client.println("<div class='container d-flex h-100'>");
  client.println("<div class='container mt-3 justify-content-center align-self-center'>");
  client.println("<div class='row'>");
  client.println("<div class='col-12 text-white text-center'>");
  client.println("<h1>Selamat Datang</h1>");
  client.println("<p>Ini adalah Dashboard <b>Air Switch v1.0</b></p>");
  client.println("</div>");
  client.println("</div>");
  client.println("<hr class='text-white'>");
  client.println("<div class='row text-white text-center justify-content-center'>");
  client.println("<div class='col-auto border rounded m-1 shadow'>");
  client.println("<h3 class='p-1 pt-2'>Status : <span class='badge bg-info'>");
  if(value == HIGH) 
  {
    client.print("OFF");
  } 
  else 
  {
    client.print("ON");
  }
  client.println("</span> </h3>");
  client.println("</div>");
  client.println("</div>");
  client.println("<div class='row justify-content-center text-center m-3'>");
  client.println("<div class='col'>");
  client.println("<button class='btn btn-lg rounded-pill btn-outline-light bg-success shadow btn-block w-25'><b> <a class='text-white' href=\"/pin_relay=ON\">ON</a> </b></button>");
  client.println("</div>");
  client.println("</div>");
  client.println("<div class='row justify-content-center text-center m-3'>");
  client.println("<div class='col'>");
  client.println("<button class='btn btn-lg rounded-pill btn-outline-light bg-danger shadow btn-block w-25'><b> <a class='text-white' href=\"/pin_relay=OFF\">OFF</a> </b></button>");
  client.println("</div>");
  client.println("</div>");
  client.println("<hr class='text-white'>");
  client.println("<div class='row'>");
  client.println("<div class='col'>");
  client.println("<footer class='text-center text-white tex'>");
  ip = WiFi.localIP();
  client.println("<span class='badge bg-secondary'>IP Address : ");
  client.println(ip);
  client.println("</span><br>");
  client.println("<small><b>Masipnu</b> Official<br>&copy; 2021</small>");
  client.println("</footer>");
  client.println("</div>");
  client.println("</div>");
  client.println("</div>");
  client.println("</div>");
  client.println("<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js' integrity='sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4' crossorigin='anonymous'></script>");
  client.println("</body>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
 
