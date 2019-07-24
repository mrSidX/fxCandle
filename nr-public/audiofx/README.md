-
You will need to create a Publicly accessible folder for Node-Red to access and make available on the network.

/nr-public/

create a subfolder called audiofx and copy this git folder's contents (audio files).

Change your .node-red/settings.js file, uncomment the # and change settings to point to this public folder, /nr-public .

The UI Client will reference this address on the network and load the audio files.
