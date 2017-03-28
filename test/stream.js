let path = require('path'),
    openal = require('../openal'),
    spawn = require('child_process').spawn;

var device = new openal.Device(),
    context = new openal.Context(device),
    file = path.resolve(__dirname, "../sounds/Palisades.mp3"),
    args = ["-af", "resample=44100:0:1,channels=2,format=s16le",
            "-nogui", "-novideo", "-noconsolecontrols", "-really-quiet",
            "-ao", "pcm:nowaveheader:file=/dev/stdout", file];

openal.MakeContextCurrent(context);
openal.SetListenerOrientation(1, 0, 0);

var data = new openal.WavData(path.resolve(__dirname, "../sounds/synth.wav")),
    synth = new openal.Source(data),
    stream = new openal.Stream(2, 16, 44100),
    mplayer = spawn('mplayer', args);

console.log('mplayer', args.join(' '));

mplayer.stdout.on('data', function (data) {
  do {
  } while( !stream.Ready() );
  stream.Buffer(data);
});

mplayer.stderr.on('data', function (data) {
  console.log('stderr: ' + data);
});

mplayer.on('close', function (code) {
  console.log('mplayer process exited with code ' + code);
});


var gain = 1;
var x=0, y=0, z=0;
var start = new Date();
setInterval(function(){
  console.log('interval');
  x+=0.05;
  gain *= 0.8;

  stream.SetGain(gain);
  stream.SetPosition(-x, y, z);
  console.log( stream.GetPosition() );

  openal.SetListenerPosition(x, y, z);
  synth.Play();
  var seconds = (new Date() - start)/1000;
  console.log(seconds, 'secs');
}, 1000);

