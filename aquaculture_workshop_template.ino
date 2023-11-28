#include <OneWire.h>
#include <DallasTemperature.h>

/*First we have to make sure our physical system matches our code. Set the
following variables to be whatever pin you're plugged into. On the board, it
will be called D-something. For example, the code below says the TDS sensor
is plugged into D4, and the temperature sensor is in D22.*/

//YOUR NUMBERS HERE
#define tdsSensorPin 0
#define temperatureSensorPin 0
#define buzzer 0

/* VREF is the analog reference voltage of the analog-digital converter. This
is the value that the device uses for comparision when trying to create a scale.
For a lot of microcontrollers, this value is 3.3V. An ESP32 uses a reference
voltage of 1.1V.*/
#define VREF 5 //analog reference voltage of the ADC
#define buffer_size 30 //the size of our arrays

/* We are using a pre-made library to read our temperature sensor. This lets
us setup an instance so our code knows there is a OneWire device attatched. */
OneWire oneWire((int)temperatureSensorPin);
DallasTemperature sensors(&oneWire);

/*Any code that we want to run only when the code starts needs to be in the 
setup function. For us, this is where we do the following:
1. Start the output with Serial.begin(channel)
2. Tell each pin whether it is input or output (Input is listening for something, output is saying something).
   Uses pinMode() to set INPUT or OUTPUT for the TDS sensor and the buzzer.
3. Start the DallasTemperature sensors with sensors.begin()
*/
void setup() {
  //1
  //2
  //2
  //3
}

/*Any code that we want to run consistantly needs to be in the loop function.
For us, this is where we do all the work.*/
void loop(){

    /*Here we fetch the temperature from our sensor and store it in a variable using
    getTempCByIndex(). It doesn't support Farenheit, so it would have to be converted
    using the formula (temp_c x 1.8) + 32 = temp_f)

    sensors.requestTemperatures() is a sensor function to get the sensors to send data
    to the Arduino. sensors.getTempCByIndex(index) is a function to return the temperature.
    The index tells us which sensor we are getting data from. Since we only have one, it's
    index is 0.*/

    sensors.requestTemperatures();
    float temperature = 0; //YOUR CODE HERE

    Serial.print("Temperature=");
    Serial.println(temperature);

    /*Next we have to stop using libraries and deal with our TDS sensor by ourselves.
    analogRead(pin) is a function to read analog data from a pin on the Arduino. You defined
    the pin on line 10. Then we have to convert the number to a readable voltage.*/

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

    float compensationCoefficient = 0; //YOUR CODE HERE: temperature compensation formula
    float compensationVoltage = voltage / compensationCoefficient; //temperature compensation

    /*The conversion is a quadrinomial equation. Why? Good question! In an ideal world it would
    be linear, but because resistance and current interactions, it needs to be approximated. */
    float v3 = 133.42 * compensationVoltage * compensationVoltage * compensationVoltage;
    float v2 = 255.86 * compensationVoltage * compensationVoltage;
    float v1 = 857.39 * compensationVoltage;
    float tdsValue = 0; //YOUR CODE HERE - convert v values to tds value
    
    /*Write some print statements to see what your numbers are like!*/  

    Serial.println("end with a new line"); //how to end with a new line (enter)
    Serial.print("example print statement: "); //how to print text (no enter)
    Serial.println(tdsValue); //how to print a variable

    /*This if statement is our alert condition. It currently does nothing,
    but in an industrial setting we would need some alert for when our tank has too much
    gunk in it. What do you think a good threshold would be?*/

    /*We can use an if/else comparision to decide when to run code. We can then use
    digitalWrite() to write a pin (our buzzer) to be high (on) or low (off) when we want
    it to be.*/

    if(tdsValue > 0){

    }else{

    }
}


/*The following code is all for optimization. If you would like to explore it or use these
functions, you're welcome to.*/

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