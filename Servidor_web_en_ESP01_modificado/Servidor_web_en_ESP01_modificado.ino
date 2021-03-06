/*################## Servidor web en SP01 ########################
* Filename: ESP-01_Ej2.ino
* Descripción: Enciende/apaga LED desde navegador
* Autor: Jose Mª Morales
* Revisión: 11-06-2018
* Probado: ARDUINO UNO r3 - IDE 1.8.2 (Windows7)
* Web: www.playbyte.es/electronica/
* Licencia: Creative Commons Share-Alike 3.0
* http://creativecommons.org/licenses/by-sa/3.0/deed.es_ES
* ##############################################################
*/

 /*comentario de prueba*/
#include <SoftwareSerial.h>
const int Rx = 3;   // Pin3 -> RX, conectar con Tx del modulo
const int Tx = 2;   // Pin2 -> TX, conectar con Rx del modulo
#define SSID  "Pardos"    // Nombre de la red
#define PASS  "53707246"  // Password
#define PORT_SERVER  "80"     // Puerto
SoftwareSerial ESP01(Rx,Tx); // Establece comunicación serie en pines Rx y Tx
 
 
void setup() {
 
    Serial.begin(9600);  // monitor serial del arduino
    ESP01.begin(9600);   // baud rate del ESP8266
 
    Serial.println("lED del pin 13 de Arduino OFF");
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW); 
 
    init_mod();         // Configura el modulo
}
//========================================================
 
 
 
void loop() {
/* 
  if (Serial.available()) {
    char c = Serial.read();// manda al modulo lo escrito en el serial monitor
    ESP01.print(c);
  }
*/ 
  if(ESP01.available()) {    // revisar si hay mensaje del ESP01
 
      char c = ESP01.read();
      Serial.print(c);      // escribe lo que recibe del modulo
   
      if(ESP01.find("+IPD,"))   data_server();  // revisar si el servidor recibio datos
  }
 
}
//#######################################################
 
 
 
void init_mod(){
 
    Serial.println("=========================");
    Serial.println("Configurando modulo ESP01");
    Serial.println("=========================");
    Serial.println("Reseteando modulo");
    sendData("AT+RST\r\n",2000);
    Serial.println("_________________________");
    Serial.println("Ajustando velocidad del modulo");
    sendData("AT+CIOBAUD=9600\r\n",3000);
    Serial.println("_________________________");
    Serial.println("Comprobando si adoptó la velocidad");
    sendData("AT+CIOBAUD?\r\n",1000);             //*******
    Serial.println("_________________________");
    Serial.println("Configurando como cliente");
    sendData("AT+CWMODE=1\r\n",1000);
    Serial.println("_________________________");
    Serial.println("SSID y password para conectarse a red");
    sendData("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n",8000);
    Serial.println("_________________________");
    Serial.println("forzar dirección IP");
    sendData("AT+CIPSTA=\"192.168.0.150\"\r\n",2000);
    Serial.println("_________________________");
    /*algo diferente*/
    /*comentario escrito por laura*/
    /*Serial.println("obtener direccion IP");     
    sendData("AT+CIFSR\r\n",2000); */        //Eliminada para evitar IP al azar
    Serial.println("Comprobar IP forzada");
    sendData("AT+CIPSTA?\r\n",1000);
    Serial.println("_________________________");
    Serial.println("configuramos el servidor en modo multiconexion");
    sendData("AT+CIPMUX=1\r\n",1000);
    Serial.println("_________________________");
    Serial.println("servidor 1 en el puerto 80"); 
    sendData("AT+CIPSERVER=1,"PORT_SERVER"\r\n",1000);
    Serial.println("=========================");
}
//========================================================
 
 
void sendData(String comando, const int timeout)  {
// Envia comando al ESP01 y verifica la respuesta dentro del tiempo timeout
 
 long int time = millis(); // medir el tiempo actual para verificar timeout
 ESP01.print(comando);     // enviar el comando al ESP01
 
  while( (time+timeout) > millis()) { //mientras no haya timeout
 
      while(ESP01.available()) {// mientras haya datos por leer, lee los datos disponibles
        char c = ESP01.read();  // leer el siguiente caracter
        Serial.print(c);
      }
  } 
 return;
}
 
 
void data_server(){
 
    delay(2000);    // espera que lleguen los datos hacia el buffer 2000 por 1000
    int conexionID = ESP01.read()-48; // obtener el ID de la conexión para poder responder
    ESP01.find("led=");               // buscar el texto "led="
    int state = (ESP01.read()-48);    // Obtener el estado del pin a mostrar
    digitalWrite(13, state);          // Cambia estado del pin
    
    while(ESP01.available())  {
        char c = ESP01.read();
        Serial.print(c);
      }
 
 //responder y cerrar la conexión para que el navegador no se quede cargando 
 
    String html = ""; // página web a enviar
    if (state==1) html += "<h1>LED_13 = encendido!</h1>";
    else {html += "<h1>LED_13 = apagado!</h1>";}
      
    String cmd_Webpage = "AT+CIPSEND="; // comando para enviar página web
    cmd_Webpage += conexionID; // esto no es para el modulito simo para la página web y probablemente lo de las siguientes lineas tambien
    cmd_Webpage += ",";
    cmd_Webpage += html.length();
    cmd_Webpage += "\r\n";
    sendData(cmd_Webpage,1000);
    sendData(html,1000);
   
    String cmd_close = "AT+CIPCLOSE=";  // comando para terminar conexión
    cmd_close += conexionID;
    cmd_close += "\r\n";
    sendData(cmd_close,3000);
 
}

/*nuevo codigo añadido por Laura para probar html */

<<<<<<< HEAD
cliente.println("HTTP/1.1 200 OK");
cliente.println("Content-Type: text/html");
cliente.println(); //Pagina web en html
cliente.println("<html>");
  cliente.println("<head>");
    cliente.println("<title>JUGANDO CON LED</title>");
  cliente.println("</head>");
  cliente.println("<body>");
    cliente.println("<h1 style="font-family: sans-serif; color:teal;">Juguemos con un Led!</h1>");
    cliente.println("<h3 style="font-family: sans-serif; color:darkslategray;">Cual es el estado del led?</h3>");
    cliente.print(state);
    cliente.println("<input type="submit" value="ON" onclick="location.href" name="" style="border:none; background-color:gold; padding:15px; font-size:16px;">");
    client.println("<input type="submit" value="OFF" onclick="location.href" name="" style="border:none; background-color:black; color:white; padding:15px; font-size:16px;">");
=======
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(); //Pagina web en html
client.println("<html>");
  client.println("<head>");
    client.println("<title>JUGANDO CON LED</title>");
  client.println("</head>");
  client.println("<body>");
    client.println("<h1 style="font-family: sans-serif; color:teal;">Juguemos con un Led!</h1>");
    client.println("<h3 style="font-family: sans-serif; color:darkslategray;">Cual es el estado del led?</h3>");
    client.print(state);
    client.println("<button type="button" style="border:none; background-color:gold; padding:15px; font-size:16px;">encender</button>");
    client.println("<button type="button" style="border:none; background-color:black; color:white; padding:15px; font-size:16px;">apagar</button>");
>>>>>>> 798055e6b736d5e3eb874d2c4ae8b76abd45fc72
  client.println("</body>");
client.println("</html>");
