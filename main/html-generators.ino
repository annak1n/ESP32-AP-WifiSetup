
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

  client.println("<label style='margin-right: 26px;'>SSID:</label>");
  client.println("<input name='ssid' length=64 type='text'/><br/><br/>");

  client.println("<label>Password:</label>");
  client.println("<input name='password' length=64 type='password'/><br/><br/>");

  client.println("<input type='submit'/>");

  client.println("</form>");
  client.println("</body>");
  client.println("</html>");

  client.println();
}
