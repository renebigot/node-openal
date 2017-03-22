
var openal = require('../openal')
var device = new openal.Device();
var context = new openal.Context( device );

openal.MakeContextCurrent( context );
openal.SetListenerOrientation(1, 0, 0);

var synthData = new openal.WavData(__dirname+"/../sounds/synth.wav");
var synth = new openal.Source(synthData);

var x=-1;

setInterval(() => {
  if (x > 0) {
    x = -1;
    console.log('>>>');
  } else {
    x = 1;
    console.log('<<<');
  }

  synth.SetPosition(0, 0, x);
  synth.Play();
}, 1000);

