var env = require('../env-config.js')();
var spark = require('sparknode');
var apn = require('apn');

var core = new spark.Core({
  accessToken: env.SPARK_CORE_TOKEN,
  id: env.SPARK_CORE_ID
});

core.on('notifyWatch', function(info) {
var note = new apn.Notification();

note.expiry = Math.floor(Date.now() / 1000) + 3600; // Expires 1 hour from now.
note.badge = 3;
note.sound = "ping.aiff";
note.alert = "Its On!";


var apnConnection = new apn.Connection(options);
var iosDevice = new apn.Device(env.IOS_TOKEN);
apnConnection.pushNotification(note, iosDevice);
  console.log(info);
  console.log(info.data);
})

core.on('error', function(err){
  console.log(err);
});



var options = {
    "batchFeedback": true,
    "interval": 300
};

var feedback = new apn.Feedback(options);
feedback.on("feedback", function(devices) {
	console.log(devices);
    devices.forEach(function(item) {
    });
});
