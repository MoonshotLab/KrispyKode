var spark = require('sparknode');
var apn = require('apn');

var core = new spark.Core({
  accessToken: "4b739ed05c46656233f63d3521129f0777ffe3bd",
  id: "55ff6d065075555327220287"
});

core.on('notifyWatch', function(info) {
var note = new apn.Notification();

note.expiry = Math.floor(Date.now() / 1000) + 3600; // Expires 1 hour from now.
note.badge = 3;
note.sound = "ping.aiff";
note.alert = "Its On!";

apnConnection.pushNotification(note, myDevice);
  console.log(info);
  console.log(info.data);
})

core.on('error', function(info)	{
});


var options = { };

var apnConnection = new apn.Connection(options);

var myDevice = new apn.Device("d7a9d3f9 9fc48fe9 12123193 d35381b1 c04f5119 a2b99ef6 f9ee3783 21abb2c1");




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
