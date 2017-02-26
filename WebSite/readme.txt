This code is a Product Registration page to generate the Authorisation code as well as authorize your device for replenishment. PubNub Javascript file is added so as to send the Authorization code via PubNub to our device (ESP8266) it is working now and sending the Authorization code successfully but for time being i am manually entering the authorization code when the device is in the AP mode!!

You just need PubNub Publish and Subscribe key to send Authorization code:- 
var pubnub = new PubNub({
publishKey : 'pub-c-2daff9e3-8db4-85e4-f1696e23ec65', 
subscribeKey : 'sub-c-6a3ea956-c04a-b38f-02ee2ddab7fe' })
