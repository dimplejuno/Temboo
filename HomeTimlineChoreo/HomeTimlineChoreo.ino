/*
  ReadATweet

  Demonstrates retrieving the most recent Tweet from a user's home timeline 
  using the Temboo Arduino Yun SDK.
  
  This example code is in the public domain.
*/

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information

/*** SUBSTITUTE YOUR VALUES BELOW: ***/

// Note that for additional security and reusability, you could
// use #define statements to specify these values in a .h file.
const String TWITTER_ACCESS_TOKEN = "113391915-0pe5RkPigynOfmWFOeFQzSIAM1TjYcCK4QXRbrwT";
const String TWITTER_ACCESS_TOKEN_SECRET = "Ktc25ZZk9Xy1jybRxokuioCe9p1itLNSzlBgQXq3A";
const String TWITTER_API_KEY = "hhNb0Aafz9erHkgTUgKyew";
const String TWITTER_API_SECRET = "YqiEyxW084qQ0wk2RYJq51MWuKFpdLuiK9L8KL9hY";

int numRuns = 1;   // execution count, so this doesn't run forever
int maxRuns = 10;   // the max number of times the Twitter HomeTimeline Choreo should run

void setup() {
  Serial.begin(115200);
  
  // For debugging, wait until a serial console is connected.
  delay(4000);
  while(!Serial);
  Bridge.begin();
}
void loop()
{
  // while we haven't reached the max number of runs...
  if (numRuns <= maxRuns) {
    Serial.println("Running ReadATweet - Run #" + String(numRuns++));
    
    TembooChoreo HomeTimelineChoreo;

    // invoke the Temboo client.
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    HomeTimelineChoreo.begin();
    
    // set Temboo account credentials
    HomeTimelineChoreo.setAccountName(TEMBOO_ACCOUNT);
    HomeTimelineChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    HomeTimelineChoreo.setAppKey(TEMBOO_APP_KEY);

    // tell the Temboo client which Choreo to run (Twitter > Timelines > HomeTimeline)
    HomeTimelineChoreo.setChoreo("/Library/Twitter/Timelines/HomeTimeline");
    
    
    // set the required choreo inputs
    // see https://www.temboo.com/library/Library/Twitter/Timelines/HomeTimeline/
    // for complete details about the inputs for this Choreo

    HomeTimelineChoreo.addInput("Count", "5"); // the max number of Tweets to return from each request
    HomeTimelineChoreo.addInput("AccessToken", TWITTER_ACCESS_TOKEN);
    HomeTimelineChoreo.addInput("AccessTokenSecret", TWITTER_ACCESS_TOKEN_SECRET);
    HomeTimelineChoreo.addInput("ConsumerKey", TWITTER_API_KEY);    
    HomeTimelineChoreo.addInput("ConsumerSecret", TWITTER_API_SECRET);

    // next, we'll define two output filters that let us specify the 
    // elements of the response from Twitter that we want to receive.
    // see the examples at http://www.temboo.com/arduino
    // for more on using output filters
   
    // we want the text of the tweet
    HomeTimelineChoreo.addOutputFilter("tweet", "/[1]/text", "Response");
    
    // and the name of the author
    HomeTimelineChoreo.addOutputFilter("author", "/[1]/user/screen_name", "Response");


    // tell the Process to run and wait for the results. The 
    // return code will tell us whether the Temboo client 
    // was able to send our request to the Temboo servers
    unsigned int returnCode = HomeTimelineChoreo.run();
    
   // a response code of 0 means success; print the API response
    if(returnCode == 0) {
      
      String author; // a String to hold the tweet author's name
      String tweet; // a String to hold the text of the tweet


      // choreo outputs are returned as key/value pairs, delimited with 
      // newlines and record/field terminator characters, for example:
      // Name1\n\x1F
      // Value1\n\x1E
      // Name2\n\x1F
      // Value2\n\x1E      
      
      // see the examples at http://www.temboo.com/arduino for more details
      // we can read this format into separate variables, as follows:
      
      while(HomeTimelineChoreo.available()) {
        // read the name of the output item
        String name = HomeTimelineChoreo.readStringUntil('\x1F');
        name.trim();

        // read the value of the output item
        String data = HomeTimelineChoreo.readStringUntil('\x1E');
        data.trim();

        // assign the value to the appropriate String
        if (name == "tweet") {
          tweet = data;
        } else if (name == "author") {
          author = data;
        }
      }
     
      Serial.println("@" + author + " - " + tweet);
    
    } else {
      // there was an error
      // print the raw output from the choreo
      while(HomeTimelineChoreo.available()) {
        char c = HomeTimelineChoreo.read();
        Serial.print(c);
      }
    }

    HomeTimelineChoreo.close();

  }

  Serial.println("Waiting...");
  delay(90000); // wait 90 seconds between HomeTimeline calls
}
