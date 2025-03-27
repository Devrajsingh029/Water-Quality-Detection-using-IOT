#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "dev";          // Your WiFi SSID
const char* password = "123456789";  // Your WiFi password

WebServer server(80);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27 for a 16x2 LCD

#define ONE_WIRE_BUS 23
#define TURBIDITY_PIN 32  // Analog pin for turbidity sensor
#define SensorPin 35        // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
  

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("TDS:");
   pinMode(SensorPin,INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Display ESP32 IP address on LCD
  lcd.setCursor(0, 1);
  lcd.print("");
  lcd.print(WiFi.localIP());

  // Start DS18B20 sensor
  sensors.begin();

  // Route for root / web page
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getPage());
  });

  // Start server
  server.begin();
  Serial.println("HTTP server started");
  

  delay(1000);
}

void loop() {
  server.handleClient();

  // Read TDS value
  int tdsValue = analogRead(A0); // Assuming TDS sensor connected to A0

  // Read turbidity value
  int turbidityValue = analogRead(TURBIDITY_PIN);
  int val = map(turbidityValue, 0,2800, 5 , 1);

  // Read temperature from DS18B20 sensor
  sensors.requestTemperatures(); // Send the command to get temperature readings
  float tempC = sensors.getTempCByIndex(0); // Assuming only one DS18B20 sensor connected

phsensorv();
  
  // Update LCD
  lcd.setCursor(5, 0);
  lcd.print("      ");  // Clear previous TDS reading
  lcd.setCursor(4, 0);
  lcd.print(tdsValue);

  lcd.setCursor(0, 1);
  lcd.print("T:");
     lcd.print(tempC);


  lcd.setCursor(8, 0);
  lcd.print("Tur:");
  lcd.print("      ");
  lcd.setCursor(12, 0);
  lcd.print(turbidityValue);


  
  delay(500);  // Adjust delay as needed
}

void phsensorv()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/4095/8; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
 
  
   
    lcd.setCursor(8, 1);
  lcd.print("pH:");
  lcd.print("      ");
  lcd.setCursor(12, 1);
  lcd.print(phValue);
 
    }

String getPage() {
  String page = "<!DOCTYPE html>"
                "<html lang='en'>"
                "<head>"
                "  <meta charset='UTF-8'>"
                "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "  <title>Sensor Dashboard</title>"
                "  <style>"
                "    body {"
                "      font-family: Arial, sans-serif;"
                "      background-color: #f0f0f0;"
                "      color: #333;"
                "      margin: 0;"
                "      padding: 0;"
                "    }"
                "    .container {"
                "      max-width: 800px;"
                "      margin: 20px auto;"
                "      padding: 20px;"
                "      background-color: #fff;"
                "      border-radius: 10px;"
                "      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
                "    }"
                "    h1 {"
                "      text-align: center;"
                "      color: #007bff;"
                "    }"
                "    .sensor {"
                "      margin-bottom: 30px;"
                "      padding: 20px;"
                "      background-color: #f8f9fa;"
                "      border-radius: 10px;"
                "      box-shadow: 0 0 5px rgba(0, 0, 0, 0.1);"
                "    }"
                "    .sensor h2 {"
                "      margin-top: 0;"
                "      color: #343a40;"
                "    }"
                "    .sensor-value {"
                "      font-size: 36px;"
                "      font-weight: bold;"
                "      color: #28a745;"
                "    }"
                "    .sensor-label {"
                "      font-size: 18px;"
                "      color: #6c757d;"
                "    }"
                "  </style>"
                "</head>"
                "<body>"
                "  <div class='container'>"
                "    <h1>Sensor Dashboard</h1>"
                "    <div class='sensor'>"
                "      <h2>TDS</h2>"
                "      <div class='sensor-value' id='tds-value'>0</div>"
                "      <div class='sensor-label'>mg/L</div>"
                "    </div>"
                "    <div class='sensor'>"
                "      <h2>pH</h2>"
                "      <div class='sensor-value' id='ph-value'>0.00</div>"
                "      <div class='sensor-label'></div>"
                "    </div>"
                "    <div class='sensor'>"
                "      <h2>Temperature</h2>"
                "      <div class='sensor-value' id='temperature-value'>0.00</div>"
                "      <div class='sensor-label'>°C</div>"
                "    </div>"
                "    <div class='sensor'>"
                "      <h2>Turbidity</h2>"
                "      <div class='sensor-value' id='turbidity-value'>0</div>"
                "      <div class='sensor-label'></div>"
                "    </div>"
                "  </div>"
                "  <script>"
                "    setInterval(function() {"
                "      var tdsValue = " + String(analogRead(A0)) + ";" +
                "      document.getElementById('tds-value').innerText = tdsValue;" +
                "      var tempValue = " + String(sensors.getTempCByIndex(0)) + ";" +
                "      document.getElementById('temperature-value').innerText = tempValue;" +
                 "      var tempValue = " + String(sensors.getTempCByIndex(0)) + ";" +
                "      document.getElementById('temperature-value').innerText = tempValue;" +
                "    }, 2000);"
                  
                "  </script>"
                "  </script>"
                "</body>"
                "</html>";

  return page;
}
