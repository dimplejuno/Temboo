#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information, as described below

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(115200);
  
  // For debugging, wait until the serial console is connected
  delay(5000);
  
  while(!Serial);
  Bridge.begin();
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running SetStatus - Run #" + String(numRuns++));
    
    TembooChoreo SetStatusChoreo;

    // Invoke the Temboo client
    SetStatusChoreo.begin();

    // Set Temboo account credentials
    SetStatusChoreo.setAccountName(TEMBOO_ACCOUNT);
    SetStatusChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SetStatusChoreo.setAppKey(TEMBOO_APP_KEY);
    
    float voltage, degreesC, degreesF;
    
    // Read analog pin
    voltage = getVoltage(A0);
    
     // Now we'll convert the voltage to degrees Celsius.
    // This formula comes from the temperature sensor datasheet:

    degreesC = (voltage - 0.5) * 100.0;
  
    // While we're at it, let's convert degrees Celsius to Fahrenheit.
    // This is the classic C to F conversion formula:
  
    degreesF = degreesC * (9.0/5.0) + 32.0;
     
    String message = "Current Temperature : ";
    message = message + degreesC;
    // Set Choreo inputs
    SetStatusChoreo.addInput("AccessToken", "CAABhJAsaRMYBANbcJAPSx99VEfjPivmc8F13g4rLX9uZB4h3PA8suroqZCgcZCyuctk1nZCDO0w59qnZBvcEfUXAgSvVmpDVHLYxYXcnYQWamMYvn8l5t9TPtElFpDRvqNXrnEFgZB2hSNdCF007OZCegcON2cvwEN9cpum0103hXqZCkCmLy2JxBpIpVbZByVZCH9C0GecHT3jGnwpV4cFOMK");
    SetStatusChoreo.addInput("Message", message);
    
    // Identify the Choreo to run
    SetStatusChoreo.setChoreo("/Library/Facebook/Publishing/SetStatus");
    
    // Run the Choreo; when results are available, print them to serial
    SetStatusChoreo.run();
    
    while(SetStatusChoreo.available()) {
      char c = SetStatusChoreo.read();
      Serial.print(c);
    }
    SetStatusChoreo.close();
  }

  Serial.println("Waiting...");
  delay(60000*60); // wait 1 hour between SetStatus calls
}

float getVoltage(int pin)
{
  // This function has one input parameter, the analog pin number
  // to read. You might notice that this function does not have
  // "void" in front of it; this is because it returns a floating-
  // point value, which is the true voltage on that pin (0 to 5V).
  
  // You can write your own functions that take in parameters
  // and return values. Here's how:
  
    // To take in parameters, put their type and name in the
    // parenthesis after the function name (see above). You can
    // have multiple parameters, separated with commas.
    
    // To return a value, put the type BEFORE the function name
    // (see "float", above), and use a return() statement in your code
    // to actually return the value (see below).
  
    // If you don't need to get any parameters, you can just put
    // "()" after the function name.
  
    // If you don't need to return a value, just write "void" before
    // the function name.

  // Here's the return statement for this function. We're doing
  // all the math we need to do within this statement:
  
  return (analogRead(pin) * 0.004882814);
  
  // This equation converts the 0 to 1023 value that analogRead()
  // returns, into a 0.0 to 5.0 value that is the true voltage
  // being read at that pin.
}

