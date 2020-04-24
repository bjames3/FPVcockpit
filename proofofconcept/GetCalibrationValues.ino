/* 
** Use this program to get the calibration values to plug into the FlySkyModule2
** program. When flying, if the vehicle goes in the opposite direction of the stick movement
** reverse the channel.
** 
** 
**      // Below is code pulled from FlySkyModule2 where calibration values need to be entered. 
**      // below are idealized values, but no 2 potentiometers are perfect or the same
**	g_aPins[0].setCalibration( 0, 512,  1023);   // aileron
**	g_aPins[1].setCalibration(0, 512,  1023);    // elevator
**	g_aPins[2].setCalibration( 0, 512,  1023);   // throttle mid calculated using   (hi - lo)/2 + lo
**	g_aPins[3].setCalibration( 0, 512,  1023);   // rudder
**      g_aPins[0].setReverse(true);                  //reverse aileron (comment out if not needed)
**      g_aPins[1].setReverse(true);	              //reverse elevator (comment out if not needed)
**      //g_aPins[2].setReverse(true);	              //reverse throttle  (uncomment if needed)
**      //g_aPins[3].setReverse(true);	              //reverse rudder    (uncomment if needed)
**      
**
**   Set Serial Monitor to 9600 baud.
 */

const int xAxis = 0;   // analog input from left/right joystick 
const int yAxis = 1;   // analog input from up/down joystick 
const int zAxis = 3;
const int slider = 2;

const int delayPeriod = 5000;


// int xValue = 0; 
// int yValue = 0; 
int sliderValue = 0;

int maxLeftValue = 0;
int maxRightValue = 0;
int maxUpValue = 0;
int maxDownValue = 0;
int lPedalDwnValue = 0;
int rPedalDwnValue = 0;
int maxSliderValue = 0;
int minSliderValue = 0;

int xCenterValue = 0;
int yCenterValue = 0;
int zCenterValue = 0;

float xCenterVoltage;
float yCenterVoltage;
float zCenterVoltage;

float voltage;
float delta;

float throttleVoltage;
int throttleOutPin = 3;

void setup() {
  // configure the serial connection:
  Serial.begin(9600);
  
  pinMode(throttleOutPin, OUTPUT);
 
  //Read joystick center(rest) values
  xCenterValue = analogRead(xAxis);
  yCenterValue = analogRead(yAxis); 
  zCenterValue = analogRead(zAxis);
  
  //Calculate center value voltages
  xCenterVoltage = xCenterValue / 204.6;
  yCenterVoltage = yCenterValue / 204.6;
  zCenterVoltage = zCenterValue / 204.6;
  
  //Obtain max values
  Serial.println("Move joystick max left");
  delay(delayPeriod);
  maxLeftValue = analogRead(xAxis);
  
  
  Serial.println("Move joystick max right");
  delay(delayPeriod);
  maxRightValue = analogRead(xAxis);
  
  
  Serial.println("Move joystick max forward");
  delay(delayPeriod);
  maxUpValue = analogRead(yAxis);
  
  
  Serial.println("Move joystick max back");
  delay(delayPeriod);
  maxDownValue = analogRead(yAxis);
  
  Serial.println("Left Rudder down");
  delay(delayPeriod);
  lPedalDwnValue = analogRead(zAxis);
  
  Serial.println("Right Rudder down");
  delay(delayPeriod);
  rPedalDwnValue = analogRead(zAxis);
  
  Serial.println("Throttle down");
  delay(delayPeriod);
  minSliderValue = analogRead(slider);
  
  Serial.println("Throttle up");
  delay(delayPeriod);
  maxSliderValue = analogRead(slider);
  
  Serial.println("\n");
  
  //Print X axis data
  Serial.println("\n\tX Axis (Aileron)\n");
  
  Serial.print("Aileron left: ");
  Serial.print(maxLeftValue, DEC);
  voltage = maxLeftValue / 204.6;
  delta = xCenterVoltage - voltage;
  Serial.print("\tVoltage = ");
  Serial.print(voltage);
  Serial.print("\tLeft Delta: ");
  Serial.println(delta);
  
  Serial.print("Aileron center value is: ");
  Serial.print(xCenterValue);
  //voltage = xCenterValue / 204.6;
  Serial.print("\tVoltage = ");
  Serial.println(xCenterVoltage);
  
  Serial.print("Aileron Right: ");
  Serial.print(maxRightValue, DEC);
  voltage = maxRightValue / 204.6;
  delta = xCenterVoltage - voltage;
  Serial.print("\tVoltage = ");
  Serial.print(voltage);
  Serial.print("\tRight Delta: ");
  Serial.println(delta);
  
  //Print Y axis data
  Serial.println("\n\tY Axis (Elevator)\n");
  
  Serial.print("Elevator forward: ");
  Serial.print(maxUpValue, DEC);
  voltage = maxUpValue / 204.6;
  delta = yCenterVoltage - voltage;
  Serial.print("\tVoltage = ");
  Serial.print(voltage);
  Serial.print("\tUp Delta: ");
  Serial.println(delta);
  
  Serial.print("Elevator center value is: ");
  Serial.print(yCenterValue, DEC);
  //voltage = yCenterValue / 204.6;
  Serial.print("\tVoltage = ");
  Serial.println(yCenterVoltage);
  
  Serial.print("Elevator back: ");
  Serial.print(maxDownValue, DEC);
  voltage = maxDownValue / 204.6;
  delta = yCenterVoltage - voltage;
  Serial.print("\tVoltage = ");
  Serial.print(voltage);
  Serial.print("\tDown Delta: ");
  Serial.println(delta);
  
  Serial.println("\n");
  
   //Print Z axis data
  Serial.println("\n\tZ Axis (rudder)\n");
  
  Serial.print("Left Rudder: ");
  Serial.print(lPedalDwnValue, DEC);
  voltage = lPedalDwnValue / 204.6;
  delta = zCenterVoltage - voltage;
  Serial.print("\tVoltage = ");
  Serial.print(voltage);
  Serial.print("\tleft Rudder Delta: ");
  Serial.println(delta);
  
  Serial.print("Rudder center value is: ");
  Serial.print(zCenterValue, DEC);
  //voltage = yCenterValue / 204.6;
  Serial.print("\tVoltage = ");
  Serial.println(zCenterVoltage);
  
  Serial.print("Right Rudder: ");
  Serial.print(rPedalDwnValue, DEC);
  voltage = rPedalDwnValue / 204.6;
  delta = zCenterVoltage - voltage;
  Serial.print("\tVoltage = ");
  Serial.print(voltage);
  Serial.print("\tright Rudder Delta: ");
  Serial.println(delta);
  
  Serial.println("\n");
  
   //Print slider data
  Serial.println("\n\tThrottle\n");
  
  Serial.print("Throttle down: ");
  Serial.print(minSliderValue, DEC);
  voltage = minSliderValue / 204.6;
  Serial.print("\tVoltage = ");
  Serial.println(voltage);
  
  int throttleMid = (maxSliderValue - minSliderValue) / 2 + minSliderValue;
  Serial.print("Throttle center: ");
  Serial.println(throttleMid, DEC);
  
  Serial.print("Throttle up: ");
  Serial.print(maxSliderValue, DEC);
  voltage = maxSliderValue / 204.6;
  Serial.print("\tVoltage = ");
  Serial.println(voltage);
  
  
}


void loop() {
  
  /*
  sliderValue = analogRead(slider);
  throttleVoltage = sliderValue / 204.6;
  //int tst = throttleVoltage * 51;
  analogWrite(throttleOutPin, sliderValue);
  //Serial.println(throttleVoltage);
  //delay(5000);
  */
  
}

