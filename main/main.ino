#include <WiFi.h>
#include <string.h>

const char *ssid = "MyESP32AP";
const char *password = "testpassword";

String wifiSSID;
String wifiPasswd;

WiFiServer server(80);

void setup() {
 
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
 
  server.begin();
}
 
void loop() {
  
  
 WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    String currentLine = "";    
 
    while (client.connected()) {
       if (client.available()) {                         
        char c = client.read();
        Serial.write(c);
        if (c == '\n') { 
          if (currentLine.length() == 0) {

            String requestBody;

            while (client.available()) {
              requestBody += (char)client.read();
            }

            if (requestBody.length()) {
              parseRequestBody(requestBody);
     
              //Serial.println(wifiSSID);
              //Serial.println(wifiPasswd); 
              generateHtmlMessagePage(client, "Wifi registered");
            } else {
                generateHtmlLoginForm(client);
            }
            break;
          } else {
            if (currentLine.startsWith("GET /?")) {
              /* parse the get request data here*/
              break;
            }
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   
          currentLine += c; 
        }
      }
    }

    delay(1);
    

    client.stop();
    Serial.println("client disonnected");
  }

}

void generateHtmlResponseHeader (WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println(); 
}

void generateHtmlMessagePage (WiFiClient client, String message) {
    generateHtmlResponseHeader(client);

    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<p>"+message+"<P/><br/>");
    client.println("</html>");
  
}

void generateHtmlLoginForm (WiFiClient client) {

  generateHtmlResponseHeader(client);
  
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<body style='with: 100%;'>");
  client.println("<form method='post' action='/' style='margin: 50px auto; width: 256px;'>");

  client.println("<label>SSID:</label>");
  client.println("<input name='ssid' length=64 type='text'/><br/><br/>");

  client.println("<label>Password:</label>");
  client.println("<input name='password' length=64 type='password'/><br/><br/>");

  client.println("<input type='submit'/>");

  client.println("</form>");
  client.println("</body>");
  client.println("</html>");

  client.println();
}

void parseRequestBody (String requestBody) {
  char str[256];
  requestBody.toCharArray(str, sizeof(str));

  
  char *tokenMem1;

  char *token = strtok_r(str, "&", &tokenMem1);
  while (token != NULL) {
    char *tokenMem2;
    char *param = token;
    char *k = strtok_r(token, "=", &tokenMem2);
    char *v = strtok_r(NULL, "=", &tokenMem2);
    String key = String(k);
    String value = String(v);
    if (key == "ssid") {
      wifiSSID = value; 
    } else if(key == "password"){
      wifiPasswd = String(value);
    }
    token = strtok_r(NULL, "&", &tokenMem1);
  }
}

char* subStr (char* str, char delim, int index) {
  char *act, *sub, *ptr;
  char copy[20];
  int i;

  // Since strtok consumes the first arg, make a copy
  strcpy(copy, str);

  for (i = 1, act = copy; i <= index; i++, act = NULL) {
     //Serial.print(".");
     sub = strtok_r(act, &delim, &ptr);
     if (sub == NULL) break;
  }
  return sub;

}
