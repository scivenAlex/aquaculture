#include <OneWire.h>
#include <DallasTemperature.h>
#include "twilio.hpp"
#include <Wifi.h>

/*First we have to make sure our physical system matches our code. Set the
following variables to be whatever pin you're plugged into. On the board, it
will be called D-something. For example, the code below says the TDS sensor
is plugged into D4, and the temperature sensor is in D22.*/
#define tdsSensorPin 0 //YOUR CODE HERE
#define temperatureSensorPin 0 //YOUR CODE HERE

/* VREF is the analog reference voltage of the analog-digital converter. This
is the value that the device uses for comparision when trying to create a scale.
For a lot of microcontrollers, this value is 3.3V. An ESP32 uses a reference
voltage of 1.1V.*/
#define VREF 1.1 //analog reference voltage of the ADC
#define buffer_size 30 //the size of our arrays

/* We are using a pre-made library to read our temperature sensor. This lets
us setup an instance so our code knows there is a OneWire device attatched. 
OneWire oneWire(pin) is a function to create a OneWire object. You named the
pin on line 9! DallasTemperature sensors(&oneWire) is a function to associate
a OneWire object with a sensor.*/

//YOUR CODE HERE


/*Everything here is for part two. Don't uncomment it yet!
// We are using a premade library to be able to text. We need to
// start an instance of a Twilio object too.
Twilio *twilio;

// These are all values from Twilio so that our code can communicate with
// a secure server.
static const char *account_sid = "AC481ca6cd712d9155726d806f18501cac";
static const char *auth_token = "165d018768ff186f2ef36fe975ead595";
static const char *from_number = "+15876085566";
static const char *to_number = "+17786788792";

// To connect to the internet, we need both the network's name and
// the password to log in.
static const char *ssid = "text"; //YOUR CODE HERE
static const char *password = "text"; //YOUR CODE HERE

// This is the fun part! We need a string to send as a text when we choose
// to send a message.
static const char *message = "text"; //YOUR CODE HERE
*/

void setup() {

  /*Any code that we want to run only when the code starts needs to be before
  we make a loop function. For us, this is where we do the following:
  1. Start the output with Serial.begin(channel).
  2. Tell each pin whether it is input or output (Input is listening for something, output is saying something)
  3. Start the DallasTemperature sensors with sensors.begin()
  */

  //YOUR CODE HERE

  /*PART TWO: We need to connect to the wifi, and we need to loop and do nothing
  until we are connected, so that the code can't move on.*/

  //YOUR CODE HERE: PART TWO  

  /*Any code that we want to run consistantly needs to be in the loop.
  For us, this is where we do all the work.*/
  while(true){
    /*Here we fetch the temperature from our sensor and store it in a variable using
    getTempCByIndex(). It doesn't support Farenheit, so it would have to be converted
    using the formula (temp_c x 1.8) + 32 = temp_f).
    sensors.requestTemperatures() is a sensor function to get the sensors to send data
    to the ESP32. sensors.getTempCByIndex(index) is a function to return the temperature.
    The index tells us which sensor we are getting data from. Since we only have one, it's
    index is 0.*/
    
    //YOUR CODE HERE
    float temperature = 0; //YOUR CODE HERE

    /*Next we have to stop using libraries and deal with our TDS sensor by ourselves.
    analogRead(pin) is a function to read analog data from a pin on the ESP32. You defined
    the pin on line 8. Then we have to convert the number to a readable voltage.*/

    float value = 0; //YOUR CODE HERE
    float voltage =  value * ((float)VREF / 1024.0);

    /*When we talk about dissolved solids, we use something called standard ambient
    temperature and pressure, or SATP. This is 1 atmosphere, and 25 degrees Celsius.
    Temperature effects how disolved solids behave in water, and their conductivity.
    We want our units in parts per million, which relies on talking about it in SATP.
    So, we can compensate for the difference between the real temperature and 25 degrees
    with a math formula.

    compensatedValue = ourValue / (1.0 + 0.02*(realTemperature - 25.0))

    Good thing we know the temperature of the water!*/

    float compensationCoefficient = 0; //YOUR CODE HERE
    float compensationVoltage = 0; //YOUR CODE HERE

    /*The conversion is a quadrinomial equation. Why? Good question! In an ideal world it would
    be linear, but because resistance and current interactions, it needs to be approximated. Here
    is the formula:

    ppmValue = (133.42v^3 - 255.86v^2 + 857.39v)/2 where v is the voltage

    It's a good idea to split this into multiple variables to make it easier to read!*/

    float tdsValue = 0; //YOUR CODE HERE
    
    /*Write some print statements to see what your numbers are like! On a microcontroller IDE
    like this, we use Serial.print() to print things. We can also use Serial.println() if we
    want to start a new line after.*/

    //YOUR CODE HERE

    /*PART TWO: We need a condition to send a message! Use an if statement.*/
    
    //YOUR CODE HERE: PART TWO

    /*PART TWO: In the if statement, we can create a new instance of Twilio with Twilio(sid, token).
    We also need a delay so that it has time to create the instance.*/

    //YOUR CODE HERE: PART TWO  

    /*PART TWO: Using twilio->send_message(to, from, message, response), we can send the message to
    a phone! We also want an if statement, so we can check the if the text was successful. The function returns
    a true or false statement.*/

    /*IMPORTANT! Don't send a text in an infinite loop. That means make sure you use break immediately after you
    send a text. This will make the program stop once you've sent a text. It costs money to send texts and we have
    a limited amount, so be careful. If we send too many texts we run out of money, and if we run out of money no
    one else can send any messages.
    How to use the break value can be seen below. If you're worried about it, just ask an instructor before you test
    your code.*/

    //YOUR CODE HERE: PART TWO  

    if(tdsValue > 1000)
      //send text message
      break;
  }

}

//Don't use this! It should be empty, but it has to be here!
void loop() {}

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