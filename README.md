# SoundChange
Two simple C programs that removes vocals from stereo PCM sounds stored in the canonical WAV format, and takes an existing WAV file and creates a new WAV file with an echo added.

To run use:
- remvocals sourcewav destwav
  - sourcewav is the stereo wav sound file that already exists
  - destwav is the new stereo output wav file to create that has vocals removed
  - The program takes two commandline parameters: the first is an input wav file, and the second is an output wav file
- addecho [-d delay] [-v volume_scale] sourcewav destwav 
  - delay is how many samples to wait before mixing-in the echoing copy of the sound with the original copy of the sound
  - volume_scale is the amount by which to scale down the volume of the echoing copy of the sound
  - sourcewav is the mono wav sound file that already exists
  - destwav is the new mono output wav file to create that includes the echo
