#include <OneWire.h>
#include <DallasTemperature.h>
#include "twilio.hpp"
#include <Wifi.h>

/*First we have to make sure our physical system matches our code. Set the
following variables to be whatever pin you're plugged into. On the board, it
will be called D-something. For example, the code below says the TDS sensor
is plugged into D4, and the temperature sensor is in D22.*/

//YOUR CODE HERE
#define tdsSensorPin 34 //your code here
#define temperatureSensorPin 22 //your code here

/* VREF is the analog reference voltage of the analog-digital converter. This
is the value that the device uses for comparision when trying to create a scale.
For a lot of microcontrollers, this value is 3.3V. An ESP32 uses a reference
voltage of 1.1V.*/
#define VREF 1.1 //analog reference voltage of the ADC
#define buffer_size 30 //the size of our arrays

/* We are using a pre-made library to read our temperature sensor. This lets
us setup an instance so our code knows there is a OneWire device attatched. */
OneWire oneWire((int)temperatureSensorPin);
DallasTemperature sensors(&oneWire);


// EVERYTHING HERE IS FOR PART 2. DON'T WORRY ABOUT UNCOMMENTING YET!
// PART TWO: We are using a premade library to be able to text. We need to
// start an instance of a Twilio object too.
Twilio *twilio;

// These are all values from Twilio so that our code can communicate with
// a secure server.
static const char *account_sid = "AC481ca6cd712d9155726d806f18501cac";
static const char *auth_token = "165d018768ff186f2ef36fe975ead595";
static const char *from_number = "+15876085566";
static const char *to_number = "+12508128736";

// To connect to the internet, we need both the network's name and
// the password to log in.
static const char *ssid = "myPhone";
static const char *password = "g4a246djfjg";

// This is the fun part! We need a string to send as a text when we choose
// to send a message.
static const char *message = "Hello from my ESP32!";


/*Any code that we want to run only when the code starts needs to be in the 
setup function. For us, this is where we do the following:
1. Start the output with Serial.begin(channel)
2. Tell each pin whether it is input or output (Input is listening for something, output is saying something)
3. Start the DallasTemperature sensors
*/
void setup() {
  Serial.begin(115200); //1
  pinMode(tdsSensorPin, INPUT); //2
  sensors.begin(); //3

  //PART TWO! DON'T WORRY YET!


  
  Serial.print("Connecting to WiFi network ;");
  Serial.print(ssid);
  Serial.println("'...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    delay(500);
  }
  Serial.println("Connected!");

  while(true){
      /*Here we fetch the temperature from our sensor and store it in a variable using
    getTempCByIndex(). It doesn't support Farenheit, so it would have to be converted
    using the formula (temp_c x 1.8) + 32 = temp_f)*/
    sensors.requestTemperatures();
    //float temperature = 0; //your code here
    float temperature = sensors.getTempCByIndex(0);

    float value = analogRead(tdsSensorPin);
    float voltage =  value * ((float)VREF / 1024.0);

    /*When we talk about dissolved solids, we use something called standard ambient
    temperature and pressure, or SATP. This is 1 atmosphere, and 25 degrees Celsius.
    Temperature effects how disolved solids behave in water, and their conductivity.
    We want our units in parts per million, which relies on talking about it in SATP.
    So, we can compensate for the difference between the real temperature and 25 degrees
    with a math formula.

    compensatedValue = ourValue / (1.0 + 0.02*(realTemperature - 25.0))

    Good thing we know the temperature of the water!*/

    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //your code here: temperature compensation formula
    float compensationVoltage = voltage / compensationCoefficient; //temperature compensation

    /*The conversion is a quadrinomial equation. Why? Good question! In an ideal world it would
    be linear, but because resistance and current interactions, it needs to be approximated. */
    float v3 = 133.42 * compensationVoltage * compensationVoltage * compensationVoltage;
    float v2 = 255.86 * compensationVoltage * compensationVoltage;
    float v1 = 857.39 * compensationVoltage;
    float tdsValue = (v3 - v2 + v1) * 0.5; //convert voltage value to tds value
    
    /*Write some print statements to see what your numbers are like!*/    
    Serial.print("TDS Voltage:");
    Serial.print(voltage, 2);
    Serial.println("V");

    Serial.print("TDS Value:");
    Serial.print(tdsValue, 0);
    Serial.println("ppm");
    
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.println("ºC");

    if(tdsValue > 1400){

      //PART TWO! COMMENTED OUT UNTIL LATER!

      twilio = new Twilio(account_sid, auth_token);
      delay(1000);

      String response;
      bool success;
      
      success = twilio->send_message(to_number, from_number, message, response);
      if(success){
        Serial.println("Sent message successfully!");
      }else{
        Serial.println(response);
      }
      break;
    }
  }
}

/*Any code that we want to run consistantly needs to be in the loop function.
For us, this is where we do all the work.*/
void loop(){}

//This is a calculation to get the average
int getMeanNum(int array[], int len){
  float count;
  for(int i = 0; i < len; i++){
    count = count + array[i];
  }
  count = count/len;
  return count;
}

// This is a median filtering algorithm
int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
  bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}