var env = require('./env-config.js')();
var spark = require('sparknode');
var apn = require('apn');

var core = new spark.Core({
  accessToken: env.SPARK_CORE_TOKEN,
  id: env.SPARK_CORE_ID
});


// log errors, do nothing
core.on('error', function(err){
  console.log(err);
});


// listen for events from the spark
core.on('notifyWatch', function(info) {
  var notification = new apn.Notification();
  notification.expiry = Math.floor(Date.now() / 1000) + 3600;
  notification.badge = 3;
  notification.sound = "ping.aiff";
  notification.alert = "Its On!";

  var apnConnection = new apn.Connection();
  var iosDevice = new apn.Device("d7a9d3f9 9fc48fe9 12123193 d35381b1 c04f5119 a2b99ef6 f9ee3783 21abb2c1");

  apnConnection.pushNotification(notification, iosDevice);

  // try and pick up failures in the APN service
  var feedback = new apn.Feedback();
  feedback.on('feedback', function(devices){
    devices.forEach(function(item){
      console.log('push notification feedback:', item);
    });
  });
});


console.log('listening for spark', env.SPARK_CORE_ID);
